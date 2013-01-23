proc getversion hwnd
	call set_seh
	local hsock:DWORD
	local WSA:WSADATA
	local sin:sockaddr_in
	local str:DWORD
	local buf:DWORD
	local msg:DWORD
	local mem1:DWORD
	local mem2:DWORD
	.if [UPD_flag]<>0
		invoke ExitThread,0
	.endif
	stdcall _InterlockedIncrement,UPD_flag,0
	invoke EnableMenuItem,[hMenu],IDM_UPDATE,MF_GRAYED
	lea eax,[sin]
	invoke RtlZeroMemory,eax,sizeof.sockaddr_in
	invoke GlobalAlloc,GPTR,BUFFER_SIZE
	test eax,eax
	je .err_mem
	mov [buf],eax
	lea eax,[WSA]
	invoke WSAStartup,101h,eax
	invoke socket,AF_INET,SOCK_STREAM,0
	cmp eax,-1
	je .err_connect
	mov [hsock],eax
	invoke htons,80
	mov [sin.sin_port],ax
	mov [sin.sin_family],AF_INET
	invoke gethostbyname,host2
	test eax,eax
	je .err_connect_ip
	mov eax,[eax+12]	 ;h_addr_list список указателей на IP
	mov eax,[eax+8] 	   ;берём первый IP
	mov [sin.sin_addr],eax
	lea eax,[sin]
	invoke connect,[hsock],eax,sizeof.sockaddr_in
	test eax,eax
	jne .err_connect
	invoke GlobalAlloc,GPTR,256
	mov [str],eax
	invoke lstrcpyA,[str],zaprver
	invoke lstrcatA,[str],cur_ver
	invoke lstrcatA,[str],'&user='
	invoke lstrlenA,[str]
	push eax
	invoke RegOpenKeyExA,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		mov [vsize],10
		mov eax,[esp]
		add eax,[str]
		invoke RegQueryValueExA,[HKey],key1,0,lpType,eax,vsize
		.if eax=0
			pop eax
			add eax,[str]
			stdcall TEAdecrypt,eax,[vsize]
		.else
			pop eax
			mov ecx,[str]
			mov dword [eax+ecx-6],0
		.endif
		invoke RegCloseKey,[HKey]
		invoke lstrcatA,[str],zaprver2
		invoke lstrlenA,[str]
	.else
		pop eax
		mov ecx,[str]
		mov dword [eax+ecx-6],0
		invoke lstrcatA,[str],zaprver2
		invoke lstrlenA,[str]
	.endif
	;;;
	push eax
	invoke send,[hsock],[str],eax,0
	push eax
	invoke GlobalFree,[str]
	pop eax
	pop ecx
	cmp eax,ecx
	jne .err_no_send
	invoke GlobalAlloc,GPTR,256
	test eax,eax
	je .err_mem
	mov [str],eax
	mov edi,[buf]
      .sread3: ;цикл чтения сокета
	invoke recv,[hsock],edi,256,0 ;читаем по 256 байт за раз
	cmp eax,-1 ;ошибка чтения?
	je .err_no_send ;если да, выведем ошибку

	add edi,eax ;перемещаем указатель на память на количество считанных байт

	mov edx,edi ;определяем, не возникнет ли переполнение нашей памяти на следующем шаге чтения
	sub edx,[buf]
	cmp edx,BUFFER_SIZE-256 ;если возникнет, то выведем ошибку. Если такое часто возникает, то просто нужно увеличить BUFFER_SIZE
	jae .err_no_send
	cmp eax,0 ;все ли считано?
	jne .sread3 ;если нет, то продолжаем чтение

	stdcall preg_match,ver_teg,[buf],cur_ver,0,0,-1
	.if eax<>1
		stdcall preg_match,preg_update,[buf],[str],1,256,-1
		.if eax<>0
			invoke lstrlenA,upmessage1
			mov [msg],eax
			invoke lstrlenA,upmessage2
			add [msg],eax
			invoke lstrlenA,upmessage3
			add [msg],eax
			invoke GlobalAlloc,GPTR,64
			.if eax=0
				invoke GlobalFree,[str]
				jmp .err_mem
			.endif
			mov [mem1],eax
			invoke GlobalAlloc,GPTR,1024
			.if eax=0
				invoke GlobalFree,[mem1]
				invoke GlobalFree,[str]
				jmp .err_mem
			.endif
			mov [mem2],eax
			stdcall preg_match,ver_teg,[buf],[mem1],1,64,-1
			add [msg],esi
			stdcall preg_match,preg_new,[buf],[mem2],1,1024,-1
			add [msg],esi
			invoke GlobalAlloc,GPTR,[msg]
			.if eax=0
				invoke GlobalFree,[mem1]
				invoke GlobalFree,[mem2]
				invoke GlobalFree,[str]
				jmp .err_mem
			.endif
			mov [msg],eax
			invoke lstrcpyA,[msg],upmessage1
			invoke lstrcatA,[msg],[mem1]
			invoke lstrcatA,[msg],upmessage2
			invoke lstrcatA,[msg],[mem2]
			invoke lstrcatA,[msg],upmessage3
			invoke GetCurrentThreadId
			invoke SetWindowsHookEx,WH_CBT,MBHookProc,NULL,eax
			mov [hMBHook],eax
			invoke MessageBoxA,[hwnd],[msg],uptitle,MB_ICONINFORMATION+MB_YESNO
			.if eax=IDYES
				invoke ShellExecuteA,0,'Open',[str],0,0,0
				stdcall setstat,[hwnd],stat_ok
			.elseif eax<>IDNO & [tray_flag]=1
				invoke RtlZeroMemory,notes2,sizeof._NOTIFYICONDATA
				mov [notes2.cbSize],sizeof._NOTIFYICONDATA
				push [hwnd]
				pop [notes2.hWnd]
				mov [notes2.uID],IDI_TRAY
				invoke lstrcpyA,notes2.szInfo,upmessage1
				invoke lstrcatA,notes2.szInfo,[mem1]
				invoke lstrcpyA,notes2.szInfoTitle,'UPDATE'
				mov [notes2.uTimeout],0
				mov [notes2.dwInfoFlags],NIIF_INFO
				mov [notes2.uFlags],NIF_INFO
				invoke Shell_NotifyIcon,NIM_MODIFY,notes2
			.endif
			invoke GlobalFree,[msg]
			invoke GlobalFree,[mem1]
			invoke GlobalFree,[mem2]
		.else
			.if [update_flag]=0
				stdcall preg_match,preg7,[buf],fail_connect,0,0,-1
				.if eax<>0
					stdcall DialogPOPUP,fail_connect,1
					stdcall setstat,[hwnd],fail_connect
				.else
					stdcall DialogPOPUP,fail_X,1
					stdcall setstat,[hwnd],fail_X
				.endif
			.endif
		.endif
	.else
		.if [update_flag]=0
			stdcall DialogPOPUP,'Ваша версия ещё актуальна',1
			stdcall setstat,[hwnd],stat_ok
			invoke Sleep,2000
		.endif
	.endif
	stdcall preg_match,preg_msg,[buf],[str],1,256,-1
	.if eax<>0
		stdcall _InterlockedIncrement,flag_sec,0
		stdcall DialogPOPUP,[str],1
	.endif
	invoke GlobalFree,[str]
  .disconnect:
	mov [update_flag],0
	invoke EnableMenuItem,[hMenu],IDM_UPDATE,MF_ENABLED
	invoke GlobalFree,[buf]
	invoke closesocket,[hsock]
	invoke WSACleanup
	stdcall _InterlockedDecrement,UPD_flag,0
	invoke ExitThread,0

 .err_no_send:
	.if [update_flag]=0
		stdcall DialogPOPUP,err_connect1,0
	.endif
		stdcall setstat,[hwnd],err_connect1
	jmp .disconnect
 .err_connect_ip:
	.if [update_flag]=0
		stdcall DialogPOPUP,err_connect2,0
	.endif
		stdcall setstat,[hwnd],err_connect2
	jmp .disconnect
 .err_connect:
	 .if [update_flag]=0
		stdcall DialogPOPUP,err_connect3,0
	 .endif
		stdcall setstat,[hwnd],err_connect3
	jmp .disconnect
     .err_mem:
	.if [update_flag]=0
		stdcall DialogPOPUP,err_mem1,0
	.endif
		stdcall setstat,[hwnd],err_mem1
	jmp .disconnect

zaprver db 'GET /update.php?fora&ver=',0
zaprver2 db ' HTTP/1.1',13,10,'User-Agent: XEKEP',13,10,'Host: www.soft.zone66.su',13,10,'Cache-Control: no-cache',13,10,13,10,0
ver_teg db '[ver=*]',0
preg_update db '[update=*]',0
uptitle db 'Обновление',0
preg_new db '[new=*]',0
preg_msg db '[msg=*]',0
upmessage1 db 'Обнаружена новая версия FORA Notify ',0
upmessage2 db '!',13,10,13,10,'В новой версии:',13,10,0
upmessage3 db 13,10,13,10,'Вы хотите скачать обновлённую версию?',0
endp

proc MBHookProc ncode, wparam, lparam
	cmp [ncode],HCBT_ACTIVATE
	jne @f
	invoke LoadIcon,[mhandle],ID_ICON
	invoke SendMessage,[wparam],WM_SETICON,ICON_BIG,eax
	invoke SetDlgItemTextA,[wparam],IDYES,'Да, Скачать!'
	invoke SetDlgItemTextA,[wparam],IDNO,'Нет!'
	invoke SetTimer,[wparam],999,1000,0
	mov [sec],25
	invoke SetWindowLong,[wparam],GWL_WNDPROC,NewMBWindowProc
	mov [pOldProc2],eax
	invoke UnhookWindowsHookEx,[hMBHook]
	xor eax,eax
	ret
@@:
	invoke CallNextHookEx,[hMBHook],[ncode],[wparam],[lparam]
	ret
endp

proc NewMBWindowProc hwnd, msg, wparam, lparam
	local baton rb 15
	cmp [msg],WM_LBUTTONDOWN
	je .loc_drag
	cmp [msg],WM_COMMAND
	je .wmcommand
	cmp [msg],WM_TIMER
	je .timer
      .endp:
	invoke CallWindowProc,[pOldProc2],[hwnd],[msg],[wparam],[lparam]
	ret
.timer:
	dec [sec]
	.if [sec]=0
		invoke KillTimer,[hwnd],999
		jmp .endp
	.endif
	lea eax,[baton]
	push eax
	cinvoke wsprintf,eax,.button,[sec]
	pop eax
	invoke SetDlgItemTextA,[hwnd],IDNO,eax
	jmp .exit_proc
.wmcommand:
	.if [wparam]=BN_CLICKED shl 16 + IDNO | [wparam]=BN_CLICKED shl 16 + IDYES
		jmp .endp
	.endif
	jmp .exit_proc
.loc_drag:
	invoke SendMessage,[hwnd],0a1h,2,0
.exit_proc:
	xor eax,eax
	ret

.button db 'Нет! (%u)',0
endp