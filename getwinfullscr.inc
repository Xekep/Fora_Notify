proc getwinfullscr
	local hwnd:DWORD
	local wplmt:WINDOWPLACEMENT
	invoke FindWindowA,'Progman','Program Manager'
	mov [hwnd],eax
	invoke GetForegroundWindow
	.if eax=[hwnd]
		mov eax,0
		jmp .exit
	.endif
     ;   push eax
     ;   invoke GetWindowLongA,eax,GWL_STYLE
     ;   or eax,WS_CAPTION + WS_BORDER
     ;   jne .metka1
     ;           pop eax
     ;           mov eax,0
     ;           jmp .exit
     ;.metka1:
     ;   pop eax
	lea ecx,[wplmt]
	invoke GetWindowPlacement,eax,ecx
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
	mov eax,1
      .exit:
	ret
endp