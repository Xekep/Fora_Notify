proc get_content hwnd,pty
	call set_seh
	local hsock:DWORD
	local WSA:WSADATA
	local sin:sockaddr_in
	local str:DWORD
	local buf:DWORD
	local qmem:DWORD
	local size:DWORD
	local cflag:BYTE
	local buff_login:DWORD
	local buff_pass:DWORD
	local vsize:DWORD
	local lpType:DWORD
	local HKey:DWORD

	invoke SetDlgItemTextA,[hwnd],IDL_MONEY,0
	invoke SetDlgItemTextA,[hwnd],IDL_DATA,0
	invoke SetDlgItemTextA,[hwnd],IDL_DOGOVOR,0
	invoke SetDlgItemTextA,[hwnd],IDL_TARIF,0
	mov [vsize],64
	lea eax,[HKey]
	invoke RegOpenKeyExA,key_root,subkey2,0,KEY_QUERY_VALUE,eax
	.if eax<>0
		stdcall DialogPOPUP,err_acc,0
		xor eax,eax
		inc eax
		ret
	.endif
	invoke GlobalAlloc,GPTR,64
	mov [buff_login],eax
	lea eax,[vsize]
	lea ecx,[lpType]
	invoke RegQueryValueExA,[HKey],key1,0,ecx,[buff_login],eax
	.if eax<>0
		invoke RegCloseKey,[HKey]
		stdcall DialogPOPUP,err_acc,0
		invoke GlobalFree,[buff_login]
		xor eax,eax
		inc eax
		ret
	.endif
	stdcall TEAdecrypt,[buff_login],[vsize]
	mov eax,[buff_login]
	.if byte [eax]=0 | byte [eax]<>'f'
		invoke RegCloseKey,[HKey]
		stdcall DialogPOPUP,err_acc,0
		invoke GlobalFree,[buff_login]
		xor eax,eax
		inc eax
		ret
	.endif
	mov [vsize],64
	invoke GlobalAlloc,GPTR,64
	mov [buff_pass],eax
	lea eax,[vsize]
	lea ecx,[lpType]
	invoke RegQueryValueExA,[HKey],key2,0,ecx,[buff_pass],eax
	.if eax<>0
		invoke RegCloseKey,[HKey]
		stdcall DialogPOPUP,err_acc,0
		invoke GlobalFree,[buff_pass]
		invoke GlobalFree,[buff_login]
		xor eax,eax
		inc eax
		ret
	.endif
	stdcall TEAdecrypt,[buff_pass],[vsize]

	invoke RegCloseKey,[HKey]
	invoke GetDlgItem,[hwnd],ID_GET
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_LOGIN
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_TARIF
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hMenu],IDM_TARIF
	invoke EnableWindow,eax,0
	invoke EnableMenuItem,[hMenu],IDM_START,MF_DISABLED
	invoke EnableMenuItem,[hMenu],IDM_TARIF,MF_DISABLED
	invoke SetDlgItemTextA,[hwnd],ID_GET,"Wait"

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
	invoke htons,7777
	mov [sin.sin_port],ax
	mov [sin.sin_family],AF_INET
	invoke gethostbyname,host
	test eax,eax
	je .err_connect_ip
	mov eax,[eax+12]	 ;h_addr_list список указателей на IP
	mov eax,[eax+8] 	   ;берём первый IP
	mov [sin.sin_addr],eax
	lea eax,[sin]
	invoke connect,[hsock],eax,sizeof.sockaddr_in
	test eax,eax
	jne .err_connect
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
	cinvoke wsprintfA,[qmem],qry,[size]
	stdcall ConcatStrings,[qmem],4,[buff_login],post_pass,[buff_pass],post_
	push eax
	invoke send,[hsock],[qmem],eax,0
	pop ecx
	cmp eax,ecx
	jne .err_no_send
	invoke GlobalAlloc,GPTR,64
	test eax,eax
	je .err_mem
	mov [str],eax
	mov [cflag],0
	mov edi,[buf]
      .sread3: ;цикл чтения сокета
	cmp [cflag],00011111b
	je .end_recv
	invoke recv,[hsock],edi,512,0 ;читаем по 512 байт за раз
	cmp eax,-1 ;ошибка чтения?
	je .err_no_send ;если да, выведем ошибку

	add edi,eax ;перемещаем указатель на память на количество считанных байт

	mov edx,edi ;определяем, не возникнет ли переполнение нашей памяти на следующем шаге чтения
	sub edx,[buf]
	cmp edx,BUFFER_SIZE-512 ;если возникнет, то выведем ошибку. Если такое часто возникает, то просто нужно увеличить BUFFER_SIZE
	jae .err_no_send
	pushad
	mov al,byte [cflag]
	and al,00010000b
	jpo @f
	jne @f
		stdcall preg_match,fail_teg,[buf],fail_mess,0,0,-1
		.if eax=1
			invoke GlobalFree,[str]
			jmp .err_login
		.endif
		stdcall preg_match,fail_teg,[buf],fail_mess2,0,0,-1
		.if eax=1
			invoke GlobalFree,[str]
			jmp .err_login
		.endif
		stdcall preg_match,fail_teg2,[buf],fail_mess3,0,0,-1
		.if eax=1
			stdcall preg_match,fail_teg3,[buf],[str],1,64,-1
			.if eax<>0
				stdcall DialogPOPUP,[str],0
				stdcall setstat,[hwnd],[str]
				invoke GlobalFree,[str]
				jmp .disconnect
			.endif
		.endif
	@@:
	mov al,byte [cflag]
	and al,00000001b
	jpo @f
	jne @f
		stdcall preg_match,preg4,[buf],[str],1,64,-1
		.if eax<>0
			invoke SetDlgItemTextA,[hwnd],IDL_TARIF,[str]
			or byte [cflag],00010001b
		.endif
	@@:
	mov al,byte [cflag]
	and al,00000010b
	jpo @f
	jne @f
		stdcall preg_match,preg1,[buf],[str],1,64,-1
		.if eax<>0
			invoke SetDlgItemTextA,[hwnd],IDL_MONEY,[str]
			.if [pty]=2
				stdcall DialogPOPUP,[str],1
			.elseif [pty]=1
				stdcall preg_match,preg6,[buf],[str],1,64,-1
				mov esi,[str]
				.if dword [esi]='-0'
					xor eax,eax
					dec eax
				.else
					mov ecx,10
					call atol
				.endif
				push eax
				mov esi,notify_flag2
				mov ecx,10
				call atol
				pop ecx
				cmp ecx,eax
				jg .negativ
				je .negativ
				stdcall getwinfullscr
				.if [sound_flag]=1 & eax =0
					invoke CreateThread,0,0,sound,1,0,0
				.endif
				stdcall DialogPOPUP,money1,1
			      .negativ:
			.endif
			or byte [cflag],00010010b
		.endif
	@@:
	mov al,byte [cflag]
	and al,00000100b
	jpo @f
	jne @f
		stdcall preg_match,preg2,[buf],[str],1,64,-1
		.if eax<>0
			invoke SetDlgItemTextA,[hwnd],IDL_DATA,[str]
			or byte [cflag],00010100b
		.endif
	@@:
	mov al,byte [cflag]
	and al,00001000b
	jpo @f
	jne @f
		stdcall preg_match,preg3,[buf],[str],1,64,-1
		.if eax<>0
			invoke SetDlgItemTextA,[hwnd],IDL_DOGOVOR,[str]
			or byte [cflag],00011000b
		.endif
	@@:
	popad
	cmp eax,0 ;все ли считано?
	jne .sread3 ;если нет, то продолжаем чтение
     .end_recv:
	and byte [cflag],00001111b
	jpo @f
	je @f
		stdcall setstat,[hwnd],stat_ok
	@@:
	invoke GlobalFree,[str]
  .disconnect:
	invoke EnableMenuItem,[hMenu],IDM_START,MF_ENABLED
	invoke EnableMenuItem,[hMenu],IDM_TARIF,MF_ENABLED
	invoke SetDlgItemTextA,[hwnd],ID_GET,"СТАРТ"
	invoke GlobalFree,[buf]
	invoke GlobalFree,[buff_pass]
	invoke GlobalFree,[buff_login]
	invoke closesocket,[hsock]
	invoke WSACleanup
	invoke GlobalFree,[qmem]
	invoke GetDlgItem,[hwnd],ID_GET
	invoke EnableWindow,eax,1
	invoke GetDlgItem,[hwnd],ID_LOGIN
	invoke EnableWindow,eax,1
	invoke GetDlgItem,[hwnd],ID_TARIF
	invoke EnableWindow,eax,1
	xor eax,eax
	ret

 .err_no_send:
	stdcall DialogPOPUP,err_connect1,0
	stdcall setstat,[hwnd],err_connect1
	jmp .disconnect
 .err_connect_ip:
	stdcall DialogPOPUP,err_connect2,0
	stdcall setstat,[hwnd],err_connect2
	jmp .disconnect
 .err_connect:
	stdcall DialogPOPUP,err_connect3,0
	stdcall setstat,[hwnd],err_connect3
	jmp .disconnect
     .err_mem:
	stdcall DialogPOPUP,err_mem1,0
	stdcall setstat,[hwnd],err_mem1
	jmp .disconnect
 .err_login:
	stdcall DialogPOPUP,err_connect4,0
	stdcall setstat,[hwnd],err_connect4
	jmp .disconnect
endp