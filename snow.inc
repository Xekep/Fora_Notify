proc snow hwnd
	local count:DWORD
	local hBuffDC:DWORD
	local hBuff:DWORD
	local hdc:DWORD
	local hMemDC:DWORD

	call set_seh
	invoke WaitForSingleObject,[snow_mutex],0
	.if eax=WAIT_TIMEOUT
		jmp .exit
	.endif
	mov [count],285
	@@:
	invoke DeleteObject,[hBitmap]
	mov eax,[nbitmap]
	.if eax>3
		mov [nbitmap],0
		xor eax,eax
	.else
		inc eax
		add [nbitmap],1
	.endif
	add eax,ID_LOGO+2
       .metka1:
	invoke LoadBitmap,[mhandle],eax
	mov [hBitmap],eax
	invoke GetDC,[hwnd]
	mov [hdc],eax
	invoke CreateCompatibleDC,[hdc]
	mov [hMemDC],eax
	invoke SelectObject,[hMemDC],[hBitmap]
	stdcall dword [TransparentBlt],[hdc],0,0,[rect.right],[rect.bottom],[hMemDC],0,0,[rect.right],[rect.bottom],0FFFFFFh
	invoke DeleteDC,[hMemDC]
	invoke ReleaseDC,[hwnd],[hdc]
	dec [count]
	.if [count]=0
		invoke DeleteObject,[hBitmap]
		mov eax,ID_LOGO2
		jmp .metka1
	.elseif [count]=-1
		invoke ReleaseMutex,[snow_mutex]
		jmp .exit
	.endif
	invoke Sleep,70
	jmp @b
       .exit:
	invoke ExitThread,0
	ret
endp