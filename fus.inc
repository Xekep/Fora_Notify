ID_CANCEL = 1002
ID_STATIC = 1010
ID_PROGRESS = 1000
ID_TIME = 1003
ID_SPEED = 1004
ID_STATUS = 1005
ID_UPD = 1006
ID_STATIC2 = 1011
ID_STATIC3 = 1012
ID_TIMER = 1

proc fus hwnd, msg, wparam, lparam
	push ebx edi esi
	cmp [msg],WM_LBUTTONDOWN
	je  .move
	cmp [msg],WM_INITDIALOG
	je .wminitdialog
	cmp [msg],WM_COMMAND
	je  .wmcommand
	cmp [msg],WM_CTLCOLORSTATIC
	je .wmctlcolorstatic
	cmp [msg],WM_TIMER
	je .timer
	cmp [msg],WM_CLOSE
	je .wmclose
	xor eax,eax
	jmp .finish
	ret
    .wminitdialog:
	call set_seh
	invoke GetDlgItem,[hwnd],ID_UPD
	push eax
	invoke ShowWindow,eax,SW_HIDE
	pop eax
	stdcall AddTooltip,eax,'Использовать только на NTFS!'
	invoke SetWindowTextA,[hwnd],.title
	invoke GetDlgItem,[hwnd],ID_STATIC3
	invoke SetWindowLong,eax,GWL_WNDPROC,WindowProcWWW
	mov [fOldWndProc],eax
	mov eax,[lparam]
	mov [url],eax
	mov [fflag],0
	invoke SetTimer,[hwnd],ID_TIMER,1000,0
	invoke CreateThread,0,0,.download,[hwnd],0,0
	jmp .processed
    .wmctlcolorstatic:
	invoke GetDlgCtrlID,[lparam]
	.if eax=ID_STATIC
		invoke SetBkColor,[wparam],-1
		invoke GetStockObject,WHITE_BRUSH
		jmp .finish
	.elseif eax=ID_STATIC2 | eax=ID_STATIC3
		invoke GetStockObject,WHITE_BRUSH
		jmp .finish
	.endif
	xor eax,eax
	jmp .finish
    .timer:
	invoke GetDlgItem,[hwnd],ID_STATIC3
	.if [fflag]=1
		push SW_HIDE
		dec [fflag]
	.else
		inc [fflag]
		push SW_SHOW
	.endif
	invoke ShowWindow,eax
	jmp .processed
    .wmcommand:
	cmp [wparam],BN_CLICKED shl 16 + ID_CANCEL
	je .wmclose
	cmp [wparam],BN_CLICKED shl 16 + ID_UPD
	je .upd
	jmp .processed
    .wmclose:
	invoke KillTimer,[hwnd],ID_TIMER
	invoke EndDialog,[hwnd],0
    .processed:
	mov eax,1
    .finish:
	pop edi esi ebx
	ret
    .move:
	invoke SendMessage,[hwnd],WM_NCLBUTTONDOWN,2,0
	jmp .processed

.upd:
	invoke GetModuleFileNameA,[mhandle],perem7,512
	invoke lstrcpyA,perem6,perem7
	invoke lstrcatA,perem6,':upd'
	invoke CopyFileA,perem6,perem7
	invoke wsprintfA,perem6,'-upd $s',fbuff
	invoke EndDialog,[hwnd],0
	invoke ShellExecuteA,0,'open',perem7,perem6,0,SW_SHOW
	invoke ExitProcess,0

    .title db 'FORA Notify Update System v1.0',0
endp

proc .download hwnd
	local time:DWORD
	local ihandle:DWORD
	local hserver:DWORD
	local hpage:DWORD
	local mem:DWORD
	local readed:DWORD
	local ofn:OPENFILENAME
	local hfile:DWORD
	local fsize:DWORD
	local url_com rb 20h

	call set_seh
	invoke GlobalAlloc,GPTR,3000000
	test eax,eax
	je .err1
	mov [mem],eax
	xor ebx,ebx
	lea eax,[url_com]
	invoke InternetCrackUrlA,[url],ebx,ebx,eax
	test eax,eax
	je .err2
	invoke wsprintfA,fbuff,.stat1,host2
	invoke SetDlgItemTextA,[hwnd],ID_STATUS,fbuff
	invoke InternetOpenA
	test eax,eax
	je .err3
	mov [ihandle],eax
	invoke InternetConnectA
	test eax,eax
	je .err4
	mov [hserver],eax
	invoke HttpOpenRequestA
	test eax,eax
	je .err4
	mov [hpage],eax
	invoke HttpSendRequestA
	test eax,eax
	je .err4
	invoke HttpQueryInfoA
	test eax,eax
	je .err4
	mov [readed],0
	@@:
	invoke GetTickCount
	mov [time],eax
	invoke InternetReadFile

	invoke InternetCloseHandle,[hpage]
	invoke InternetCloseHandle,[hserver]
	invoke InternetCloseHandle,[ihandle]
	mov [ofn.lStructSize],sizeof.OPENFILENAME
	mov [ofn.hwndOwner],0
	mov [ofn.hInstance],0
	mov [ofn.lpstrFilter],.filter
	mov [ofn.lpstrCustomFilter],0
	mov [ofn.nMaxCustFilter],0
	mov [ofn.nFilterIndex],0
	invoke lstrcpyA,fbuff,.file
	mov [ofn.lpstrFile],fbuff
	mov [ofn.nMaxFile],512
	mov [ofn.lpstrFileTitle],0
	mov [ofn.nMaxFileTitle],0
	mov [ofn.lpstrInitialDir],0
	mov [ofn.lpstrTitle],main_title
	mov [ofn.Flags],OFN_FILEMUSTEXIST+OFN_PATHMUSTEXIST+OFN_LONGNAMES+OFN_HIDEREADONLY+OFN_EXPLORER
	mov [ofn.nFileOffset],0
	mov [ofn.nFileExtension],0
	mov [ofn.lpstrDefExt],0
	mov [ofn.lCustData],0
	mov [ofn.lpfnHook],0
	mov [ofn.lpTemplateName],0
	@@:
	invoke EnableWindow,[hwnd],0
	lea eax,[ofn]
	invoke GetSaveFileNameA,eax
	.if eax=0
		  invoke EnableWindow,[hwnd],1
		  invoke SetForegroundWindow,[hwnd]
		  invoke PostMessageA,[hwnd],WM_CLOSE,0,0
		  jmp .exit
	.endif
	invoke EnableWindow,[hwnd],1
	invoke SetForegroundWindow,[hwnd]
	invoke CreateFile,[ofn.lpstrFile],FILE_SHARE_READ+FILE_SHARE_WRITE,\
			  0,0,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,0
	.if eax=-1
		invoke SetDlgItemTextA,[hwnd],ID_STATUS,.stat3
		jmp @b
	.endif
	mov [hfile],eax
	invoke SetDlgItemTextA,[hwnd],ID_STATUS,.stat9
	lea eax,[readed]
	invoke WriteFile,[hfile],[mem],[fsize],eax,0
	invoke CloseHandle,[hfile]
	invoke SetDlgItemTextA,[hwnd],ID_STATUS,.stat8
	invoke GetDlgItem,[hwnd],ID_UPD
	invoke ShowWindow,eax,SW_SHOW
      .exit:
	invoke GlobalFree,[mem]
	invoke ExitThread,0
      .err1:
	invoke SetDlgItemTextA,[hwnd],ID_STATUS,.stat6
	invoke ExitThread,0
      .err2:
	invoke SetDlgItemTextA,[hwnd],ID_STATUS,.stat5
	jmp .exit
      .err3:
	invoke wsprintfA,fbuff,.stat7,host2
	invoke SetDlgItemTextA,[hwnd],ID_STATUS,fbuff
	jmp .exit
      .err4:
	invoke wsprintfA,fbuff,.stat7,host2
	invoke SetDlgItemTextA,[hwnd],ID_STATUS,fbuff
	invoke InternetCloseHandle,[hpage]
	invoke InternetCloseHandle,[ihandle]
	invoke InternetCloseHandle,[hserver]
	jmp .exit

 .file	db   'fora.exe',0
 .filter db  'EXE',0,'*.exe',0,'All',0,'*',0,0
 .stat1  db  'Устанавливается соединение с %s...',0
 .stat2  db  'Скачаивание...',0
 .stat3  db  'Ошибка при открытии файла...',0
 .stat4  db  'Подсчёт...',0
 .stat5  db  'Ошибка парсинга URL',0
 .stat6  db  'Недостаточно памяти',0
 .stat7  db  'Ошибка соединенися с %s...',0
 .stat8  db  'Закройте программу и обновите файлы',0
 .stat9  db  'Запись в файл...',0


endp

proc .speed
endp

proc WindowProcWWW hwnd, msg, wparam, lparam
	cmp [msg],WM_SETCURSOR	    ; Курсор над ссылкой
	je .cursor_over_hyperlink
	cmp [msg],WM_LBUTTONDOWN    ; Нажатие мышкой на ссылке
	je .open_link
	invoke CallWindowProc,[fOldWndProc],[hwnd],[msg],[wparam],[lparam]
	ret
.cursor_over_hyperlink:
	invoke LoadCursor,0,IDC_HAND
	invoke SetCursor,eax
	jmp .finish
.open_link:
	invoke ShellExecute,[hwnd],'open',site,0,0,SW_SHOW
.finish:
	xor eax,eax
	ret
endp