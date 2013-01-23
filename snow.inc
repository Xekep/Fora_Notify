proc snow hwnd
	local count:DWORD

	invoke WaitForSingleObject,[snow_mutex],0
	.if eax<>WAIT_OBJECT_0
		invoke ExitThread,0
	.endif
	invoke GetClientRect,[hwnd],rect
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
	invoke BeginPaint,[hwnd],ps
	invoke GetDC,[hwnd]
	mov [hdc],eax
	invoke CreateCompatibleDC,[hdc]
	mov [hMemDC],eax
	invoke SelectObject,[hMemDC],[hBitmap]
	invoke BitBlt,[hdc],0,0,[rect.right],[rect.bottom],[hMemDC],0,0,SRCCOPY
	invoke DeleteDC,[hMemDC]
	invoke ReleaseDC,[hwnd],[hdc]
	invoke EndPaint,[hwnd],ps
	dec [count]
	.if [count]=0
		invoke DeleteObject,[hBitmap]
		mov eax,ID_LOGO2
		jmp .metka1
	.elseif [count]=-1
		jmp .exit
	.endif
	invoke Sleep,70
	jmp @b
       .exit:
	invoke ReleaseMutex,[snow_mutex]
	invoke ExitThread,0
	ret
endp