proc getversion hwnd
	call set_seh
	local hsock:DWORD
	local WSA:DWORD
	local port:DWORD
	local sin:DWORD
	local str:DWORD
	local buf:DWORD
	local msg:DWORD
	local mem1:DWORD
	local mem2:DWORD
	;;;;; sockaddr_in
	sin_family equ 0   ;dw
	sin_port   equ 2   ;dw
	sin_addr   equ 4   ;dd
	sin_zero   equ 8   ;db
	;;;;; WSADATA
	wVersion       equ 0	;dw
	wHighVersion   equ 2	;dw
	szDescription  equ 4	;db
	szSystemStatus equ 261	;db
	iMaxSockets    equ 390	;dw
	iMaxUdpDg      equ 392	;dw
	_padding_      equ 394	;db
	lpVendorInfo   equ 396	;dd
	;;;
	invoke EnableMenuItem,[hMenu],IDM_UPDATE,MF_GRAYED
	mov [port],80
	invoke GlobalAlloc,GPTR,BUFFER_SIZE
	test eax,eax
	je .err_mem
	mov [buf],eax
	invoke GlobalAlloc,GPTR,sizeof.sockaddr_in
	test eax,eax
	je .err_mem
	mov [sin],eax
	invoke GlobalAlloc,GPTR,sizeof.WSADATA
	test eax,eax
	je .err_mem
	mov [WSA],eax
	invoke WSAStartup,101h,[WSA]
	invoke socket,AF_INET,SOCK_STREAM,0
	cmp eax,-1
	je .err_connect
	mov [hsock],eax
	invoke htons,[port]
	mov esi,[sin]
	mov word [esi+sin_port],ax
	mov word [esi+sin_family],AF_INET
	invoke gethostbyname,host2
	test eax,eax
	je .err_connect_ip
	mov eax,[eax+12]	 ;h_addr_list список указателей на IP
	mov eax,[eax+8] 	   ;берём первый IP
	mov esi,[sin]
	mov dword [esi+sin_addr],eax
	invoke connect,[hsock],[sin],sizeof.sockaddr_in
	test eax,eax
	jne .err_connect
	invoke lstrlenA,zaprver
	push eax
	invoke send,[hsock],zaprver,eax,0
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

	invokex preg_match,ver_teg,[buf],cur_ver,0,0,-1
	.if eax<>1
		invokex preg_match,preg_update,[buf],[str],1,256,-1
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
			invokex preg_match,ver_teg,[buf],[mem1],1,64,-1
			add [msg],esi
			invokex preg_match,preg_new,[buf],[mem2],1,1024,-1
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
				invokex setstat,[hwnd],stat_ok
			.endif
			invoke GlobalFree,[msg]
			invoke GlobalFree,[mem1]
			invoke GlobalFree,[mem2]
		.else
			.if [update_flag]=0
				invokex preg_match,fail_connect,[buf],preg7,0,0,-1
				.if eax<>1
					invokex DialogPOPUP,fail_connect,1
					invokex setstat,[hwnd],fail_connect
				.else
					invokex DialogPOPUP,fail_X,1
					invokex setstat,[hwnd],fail_X
				.endif
			.endif
		.endif
	.else
		.if [update_flag]=0
			invokex DialogPOPUP,'Ваша версия ещё актуальна',1
			invokex setstat,[hwnd],stat_ok
			invoke Sleep,2000
		.endif
	.endif
	invokex preg_match,preg_msg,[buf],[str],1,256,-1
	.if eax<>0
		invoke InterlockedIncrement,flag_sec
		invokex DialogPOPUP,[str],1
	.endif
	invoke GlobalFree,[str]
  .disconnect:
	mov [update_flag],0
	invoke EnableMenuItem,[hMenu],IDM_UPDATE,MF_ENABLED
	invoke GlobalFree,[buf]
	invoke closesocket,[hsock]
	invoke WSACleanup
	invoke GlobalFree,[WSA]
	invoke GlobalFree,[sin]
	invoke ExitThread,0

 .err_no_send:
	.if [update_flag]=0
		invokex DialogPOPUP,err_connect1,0
	.endif
		invokex setstat,[hwnd],err_connect1
	jmp .disconnect
 .err_connect_ip:
	.if [update_flag]=0
		invokex DialogPOPUP,err_connect2,0
	.endif
		invokex setstat,[hwnd],err_connect2
	jmp .disconnect
 .err_connect:
	 .if [update_flag]=0
		invokex DialogPOPUP,err_connect3,0
	 .endif
		invokex setstat,[hwnd],err_connect3
	jmp .disconnect
     .err_mem:
	.if [update_flag]=0
		invokex DialogPOPUP,err_mem1,0
	.endif
		invokex setstat,[hwnd],err_mem1
	jmp .disconnect

zaprver db 'GET /update.php?fora HTTP/1.1',13,10,'User-Agent: XEKEP',13,10,'Host: www.soft.zone66.su',13,10,'Cache-Control: no-cache',13,10,13,10,0
cur_ver db '0.9.4.1',0
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
	invoke GlobalAlloc,GPTR,15
	mov [baton],eax
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
		invoke GlobalFree,[baton]
		invoke KillTimer,[hwnd],999
		jmp .endp
	.endif
	invoke wsprintf,[baton],.button,[sec]
	invoke SetDlgItemTextA,[hwnd],IDNO,[baton]
	jmp .exit_proc
.wmcommand:
	.if [wparam]=BN_CLICKED shl 16 + IDNO | [wparam]=BN_CLICKED shl 16 + IDYES
		invoke GlobalFree,[baton]
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