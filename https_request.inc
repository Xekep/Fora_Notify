INTERNET_OPEN_TYPE_PRECONFIG = 0
INTERNET_DEFAULT_HTTPS_PORT = 443
INTERNET_DEFAULT_HTTP_PORT = 80
INTERNET_SERVICE_HTTP = 3
HTTP_VERSION equ 'HTTP/1.0'
INTERNET_FLAG_SECURE = 800000h
INTERNET_FLAG_IGNORE_CERT_CN_INVALID = 1000h
INTERNET_FLAG_IGNORE_CERT_DATE_INVALID = 2000h
INTERNET_FLAG_RELOAD = 80000000h
INTERNET_OPTION_SECURITY_FLAGS = 31
SECURITY_FLAG_IGNORE_UNKNOWN_CA = 100h
SECURITY_FLAG_IGNORE_REVOCATION  = 00000080h
ERROR_INTERNET_INVALID_CA = 12045
ERROR_INTERNET_SEC_CERT_REV_FAILED  = 12057

proc request buff,host,reqstring,useragent,method,post,buff_leng,ssl
	local ihandle:DWORD
	local hserver:DWORD
	local hpage:DWORD
	local flags:DWORD
	local leng:DWORD
	local readed:DWORD
	invoke GlobalAlloc,GPTR,64
	test eax,eax
	je .err_connect
	mov [flags],eax
	add eax,4
	mov [leng],eax
	invoke InternetOpenA,[useragent],INTERNET_OPEN_TYPE_PRECONFIG,0,0,0
	test eax,eax
	je .err_connect
	mov [ihandle],eax
	.if [ssl]=0
		mov eax,INTERNET_DEFAULT_HTTP_PORT
	.else
		mov eax,INTERNET_DEFAULT_HTTPS_PORT
	.endif
	invoke InternetConnectA,[ihandle],[host],eax,0,0,INTERNET_SERVICE_HTTP,0,0
	test eax,eax
	je .err_connect
	mov [hserver],eax
	.if [ssl]=0
		mov eax,INTERNET_FLAG_RELOAD
	.else
		mov eax,INTERNET_FLAG_SECURE or INTERNET_FLAG_IGNORE_CERT_CN_INVALID or INTERNET_FLAG_IGNORE_CERT_DATE_INVALID or INTERNET_FLAG_RELOAD
	.endif
	invoke HttpOpenRequestA,[hserver],[method],[reqstring],HTTP_VERSION,0,0,eax,0
	test eax,eax
	je .err_connect
	mov [hpage],eax
	invoke lstrlenA,.content_type
	mov esi,eax
	mov eax,[method]
	.if dword [eax]<>'GET'
		invoke lstrlenA,[post]
		mov ecx,[post]
	.else
		mov eax,0
		mov ecx,0
	.endif
	invoke HttpSendRequestA,[hpage],.content_type,esi,ecx,eax
	.if eax=0
		invoke GetLastError
		.if eax=ERROR_INTERNET_INVALID_CA
			mov eax,[leng]
			mov dword [eax],4
			invoke InternetQueryOptionA,[hpage],INTERNET_OPTION_SECURITY_FLAGS,[flags],[leng]
			test eax,eax
			je .err_connect
			mov eax,[flags]
			add dword [eax],SECURITY_FLAG_IGNORE_UNKNOWN_CA or SECURITY_FLAG_IGNORE_REVOCATION
			invoke InternetSetOptionA,[hpage],INTERNET_OPTION_SECURITY_FLAGS,eax,4
			invoke lstrlenA,.content_type
			mov esi,eax
			mov eax,[method]
			.if dword [eax]<>'GET'
				invoke lstrlenA,[post]
				mov ecx,[post]
			.else
				mov eax,0
				mov ecx,0
			.endif
			invoke HttpSendRequestA,[hpage],.content_type,esi,ecx,eax
			test eax,eax
			je .err_connect
		.else
			jmp .err_connect
		.endif
	.endif
	mov edi,[buff]
	mov [readed],0
      .read:
	mov eax,[buff_leng]
	dec eax
	sub eax,[readed]
	add edi,[readed]
	invoke InternetReadFile,[hpage],edi,eax,[leng]
	.if dword [leng]<>0 | eax<>FALSE
		mov eax,[leng]
		add [readed],eax
		mov eax,[buff_leng]
		.if [readed]<eax
			jmp .read
		.endif
	.endif
	mov esi,1
    .disconnect:
	invoke GlobalFree,[flags]
	invoke InternetCloseHandle,[hpage]
	invoke InternetCloseHandle,[ihandle]
	mov eax,esi
	ret
 .err_connect:
	mov esi,0
	jmp .disconnect
.content_type db 'Content-Type: application/x-www-form-urlencoded',0
endp