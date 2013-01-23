proc get_content hwnd
	local hsock:DWORD
	local WSA:DWORD
	local port:DWORD
	local sin:DWORD
	local str:DWORD
	local buf:DWORD
	local qmem:DWORD
	local size:DWORD
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
	mov [port],7777
	invoke GlobalAlloc,GPTR,BUFFER_SIZE
	test eax,eax
	je err_mem
	mov [buf],eax
	invoke GlobalAlloc,GPTR,sizeof.sockaddr_in
	test eax,eax
	je err_mem
	mov [sin],eax
	invoke GlobalAlloc,GPTR,sizeof.WSADATA
	test eax,eax
	je err_mem
	mov [WSA],eax
	invoke WSAStartup,101h,[WSA]
	invoke socket,AF_INET,SOCK_STREAM,0
	cmp eax,-1
	je err_connect
	mov [hsock],eax
	invoke htons,[port]
	mov esi,[sin]
	mov word [esi+sin_port],ax
	mov word [esi+sin_family],AF_INET
	invoke gethostbyname,host
	test eax,eax
	je err_connect_ip
	mov eax,[eax+12]	 ;h_addr_list список указателей на IP
	mov eax,[eax+8] 	   ;берём первый IP
	mov esi,[sin]
	mov dword [esi+sin_addr],eax
	invoke connect,[hsock],[sin],sizeof.sockaddr_in
	test eax,eax
	jne err_connect
	invoke lstrlenA,[buff_login]
	mov [size],eax
	invoke lstrlenA,[buff_pass]
	add [size],eax
	invoke lstrlenA,post_login
	add [size],eax
	invoke lstrlenA,post_pass
	add [size],eax
	invoke lstrlenA,post_
	add [size],eax
	invoke GlobalAlloc,GPTR,512
	mov [qmem],eax
	push eax
	invoke wsprintfA,[qmem],qry,[size]
	invoke lstrlenA,[qmem]
	add eax,[qmem]
	invoke lstrcpyA,eax,[buff_login]
	invoke lstrlenA,[qmem]
	add eax,[qmem]
	invoke lstrcpyA,eax,post_pass
	invoke lstrlenA,[qmem]
	add eax,[qmem]
	invoke lstrcpyA,eax,[buff_pass]
	invoke lstrlenA,[qmem]
	add eax,[qmem]
	invoke lstrcpyA,eax,post_
	invoke lstrlenA,[qmem]
	push eax
	invoke send,[hsock],[qmem],eax,0
	pop ecx
	cmp eax,ecx
	jne err_no_send
	invoke GlobalAlloc,GPTR,64
	test eax,eax
	je err_mem
	mov [str],eax
	mov ecx,5
	mov eax,cons
      clcons:
	mov byte [eax],0
	inc eax
	loop clcons
	mov edi,[buf]
      sread3: ;цикл чтения сокета
	invoke recv,[hsock],edi,512,0 ;читаем по 512 байт за раз
	cmp eax,-1 ;ошибка чтения?
	je err_no_send ;если да, выведем ошибку

	add edi,eax ;перемещаем указатель на память на количество считанных байт

	mov edx,edi ;определяем, не возникнет ли переполнение нашей памяти на следующем шаге чтения
	sub edx,[buf]
	cmp edx,BUFFER_SIZE-512 ;если возникнет, то выведем ошибку. Если такое часто возникает, то просто нужно увеличить BUFFER_SIZE
	jae err_no_send
	cmp dword [cons],01010101h
	je end_recv
	pushad
	.if byte [cons+4]=0
		invokex preg_match,fail_teg,[buf],fail_mess,0
		.if eax=1
			invoke GlobalFree,[str]
			jmp err_login
		.endif
		invokex preg_match,fail_teg,[buf],fail_mess2,0
		.if eax=1
			invoke GlobalFree,[str]
			jmp err_login
		.endif
		invokex preg_match,fail_teg2,[buf],fail_mess3,0
		.if eax=1
			invokex preg_match,fail_teg3,[buf],[str],1
			.if eax<>0
				invokex DialogPOPUP,[str],0
				invokex setstat,[hwnd],[str]
				invoke Sleep,10
				invoke GlobalFree,[str]
				jmp disconnect
			.endif
		.endif
	.endif
	.if byte [cons]=0
		invokex preg_match,preg1,[buf],[str],1
		.if eax<>0
			invoke SetDlgItemTextA,[hwnd],IDL_MONEY,[str]
			.if [flag]=1
				invokex DialogPOPUP,[str],1
				pop ecx
				pop ecx
				invoke Sleep,10
				mov [flag],0
			.endif
			mov byte [cons],1
			mov byte [cons+4],1
		.endif
	.endif
	.if byte [cons+1]=0
		invokex preg_match,preg2,[buf],[str],1
		.if eax<>0
			invoke SetDlgItemTextA,[hwnd],IDL_DATA,[str]
			 mov byte [cons+1],1
			 mov byte [cons+4],1
		.endif
	.endif
	.if byte [cons+2]=0
		invokex preg_match,preg3,[buf],[str],1
		.if eax<>0
			invoke SetDlgItemTextA,[hwnd],IDL_DOGOVOR,[str]
			mov byte [cons+2],1
			mov byte [cons+4],1
		.endif
	.endif
	.if byte [cons+3]=0
		invokex preg_match,preg4,[buf],[str],1
		.if eax<>0
			invoke SetDlgItemTextA,[hwnd],IDL_TARIF,[str]
			mov byte [cons+3],1
			mov byte [cons+4],1
		.endif
	.endif
	popad
	cmp eax,0 ;все ли считано?
	jne sread3 ;если нет, то продолжаем чтение
	pop ecx
	.if dword [cons]=01010101h
		invokex setstat,[hwnd],stat_ok
	.endif
      end_recv:
	invokex setstat,[hwnd],stat_ok
	invoke GlobalFree,[str]
  disconnect:
	invoke EnableMenuItem,[hMenu],IDM_START,MF_ENABLED
	invoke SetDlgItemTextA,[hwnd],ID_GET,"СТАРТ"
	invoke GlobalFree,[buf]
	invoke GlobalFree,[buff_pass]
	invoke GlobalFree,[buff_login]
	invoke closesocket,[hsock]
	invoke WSACleanup
	invoke GlobalFree,[qmem]
	invoke GlobalFree,[WSA]
	invoke GlobalFree,[sin]
	invoke GetDlgItem,[hwnd],ID_GET
	invoke EnableWindow,eax,1
	invoke GetDlgItem,[hwnd],ID_LOGIN
	invoke EnableWindow,eax,1
	invoke ExitThread,0
	ret

 err_no_send:
	invokex DialogPOPUP,err_connect1,0
	invokex setstat,[hwnd],err_connect1
	jmp disconnect
 err_connect_ip:
	invokex DialogPOPUP,err_connect2,0
	invokex setstat,[hwnd],err_connect2
	jmp disconnect
 err_connect:
	invokex DialogPOPUP,err_connect3,0
	invokex setstat,[hwnd],err_connect3
	jmp disconnect
     err_mem:
	invokex DialogPOPUP,err_mem1,0
	invokex setstat,[hwnd],err_mem1
	jmp disconnect
 err_login:
	invokex DialogPOPUP,err_connect4,0
	invokex setstat,[hwnd],err_connect4
	jmp disconnect
endp