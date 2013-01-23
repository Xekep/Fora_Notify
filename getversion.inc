proc getversion hwnd
	local hsock:DWORD
	local WSA:DWORD
	local port:DWORD
	local sin:DWORD
	local str:DWORD
	local buf:DWORD
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
	mov eax,[eax+8] 	   ;берЄм первый IP
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
	invoke GlobalAlloc,GPTR,64
	test eax,eax
	je .err_mem
	mov [str],eax
	mov edi,[buf]
      .sread3: ;цикл чтени€ сокета
	invoke recv,[hsock],edi,256,0 ;читаем по 256 байт за раз
	cmp eax,-1 ;ошибка чтени€?
	je .err_no_send ;если да, выведем ошибку

	add edi,eax ;перемещаем указатель на пам€ть на количество считанных байт

	mov edx,edi ;определ€ем, не возникнет ли переполнение нашей пам€ти на следующем шаге чтени€
	sub edx,[buf]
	cmp edx,BUFFER_SIZE-256 ;если возникнет, то выведем ошибку. ≈сли такое часто возникает, то просто нужно увеличить BUFFER_SIZE
	jae .err_no_send
	cmp eax,0 ;все ли считано?
	jne .sread3 ;если нет, то продолжаем чтение
	invokex preg_match,ver_teg,[buf],cur_ver,0
	.if eax<>1
		invokex preg_match,preg_update,[buf],[str],1
		.if eax<>0
			invoke MessageBoxA,[hwnd],upmessage,uptitle,MB_ICONINFORMATION+MB_YESNO
			.if eax=6
				invoke ShellExecuteA,0,'Open',[str],0,0,0
				invokex setstat,[hwnd],stat_ok
			.endif
		.endif
	.else
		.if [update_flag]=0
			invokex DialogPOPUP,'¬аша верси€ ещЄ актуальна',1
			invokex setstat,[hwnd],stat_ok
		.endif
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
	jmp disconnect
 .err_connect_ip:
	.if [update_flag]=0
		invokex DialogPOPUP,err_connect2,0
	.endif
		invokex setstat,[hwnd],err_connect2
	jmp disconnect
 .err_connect:
	 .if [update_flag]=0
		invokex DialogPOPUP,err_connect3,0
	 .endif
		invokex setstat,[hwnd],err_connect3
	jmp disconnect
     .err_mem:
	.if [update_flag]=0
		invokex DialogPOPUP,err_mem1,0
	.endif
		invokex setstat,[hwnd],err_mem1
	jmp disconnect

zaprver db 'GET /update.php?fora HTTP/1.1',13,10,'User-Agent: XEKEP',13,10,'Host: www.soft.zone66.su',13,10,'Cache-Control: no-cache',13,10,13,10,0
cur_ver db '0.9',0
ver_teg db '[ver=*]',0
preg_update db '[update=*]',0
uptitle db 'ќбновление',0
upmessage db 'ќбнаружена нова€ верси€!',13,10,'¬ы хотите скачать обновлЄнную версию?',0
endp