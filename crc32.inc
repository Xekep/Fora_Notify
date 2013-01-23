proc scrc32 hwnd,msg,timer_id,time
	; Проверка контрольной суммы текст строк
	fake_ff
	mov eax,-1
	mov esi,crc32strings_start
	mov ecx,crc32strings_end-crc32strings_start
	call .crc32
	cmp [crc32str],eax
	jne .err1
	; Проверка контрольной суммы ресурсов
	mov eax,-1
	mov esi,crc32res_start
	mov ecx,crc32res_end-crc32res_start
	call .crc32
	cmp [crc32res],eax
	jne .err1
	; Проверка контрольной суммы кода
	mov eax,-1
	mov esi,start
	mov ecx,crc32code_end-start
	call .crc32
	cmp [crc32code],eax
	jne .err1
	; Проверка контрольной суммы TLS CallBack
	mov eax,-1
	mov esi,crc32adebug_start
	mov ecx,crc32adebug_end-crc32adebug_start
	call .crc32
	cmp [crc32adebug],eax
	jne .err1
	ret
	.err1:
	invoke KillTimer,[hwnd],[timer_id]
	xor eax,eax
	push eax
	push eax
	push 2000
	push eax
	push eax
	push eax
	push fail_X
	push eax
	push [PostQuitMessage]
	push [MessageBoxTimeoutA]
	push main-1
	retn
  .crc32:
     .ComputeLoop:
	xor ebx,ebx
	xor al,[esi]
	mov bl,al
	shr eax,8
	xor eax,dword [4*ebx+CrtTable]
	inc esi
	loop .ComputeLoop
	xor eax,0FFFFFFFFh
	retn
endp