proc getwinfullscr
	local hwnd:DWORD
	local wplmt:WINDOWPLACEMENT
	invoke FindWindowA,'Progman','Program Manager'
	mov [hwnd],eax
	invoke GetForegroundWindow
	.if eax=[hwnd]
		xor eax,eax
		jmp .exit
	.endif

	push eax
	invoke GetWindowLongA,eax,GWL_STYLE
	mov ecx,eax
	mov esi,eax
	xor eax,eax
	pop ebx
	and ecx,0FFFFFFFFh - WS_THICKFRAME
	je @f
	jg @f
	mov ecx,esi
	and ecx,0FFFFFFFFh - WS_CAPTION
	je .exit
	jg .exit
	mov ecx,esi
	and ecx,0FFFFFFFFh - WS_BORDER
	je .exit
	jg .exit
	@@:

	lea ecx,[wplmt]
	invoke GetWindowPlacement,ebx,ecx
	.if [wplmt.rcNormalPosition.left]<>0 | [wplmt.rcNormalPosition.top]<>0
		xor eax,eax
		jmp .exit
	.endif
	invoke GetSystemMetrics,SM_CXSCREEN
	.if eax>[wplmt.rcNormalPosition.right]
	       xor eax,eax
		jmp .exit
	.endif
	invoke GetSystemMetrics,SM_CYSCREEN
	.if eax>[wplmt.rcNormalPosition.bottom]
		xor eax,eax
		jmp .exit
	.endif
	xor eax,eax
	inc eax
      .exit:
	ret
endp