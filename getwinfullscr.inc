proc getwinfullscr
	local hwnd:DWORD
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
	invoke GetWindowPlacement,eax,wplmt
	.if [wplmt.rcNormalPosition.left]<>0 | [wplmt.rcNormalPosition.top]<>0
		mov eax,0
		jmp .exit
	.endif
	invoke GetSystemMetrics,SM_CXSCREEN
	.if eax>[wplmt.rcNormalPosition.right]
		mov eax,0
		jmp .exit
	.endif
	invoke GetSystemMetrics,SM_CYSCREEN
	.if eax>[wplmt.rcNormalPosition.bottom]
		mov eax,0
		jmp .exit
	.endif
	mov eax,1
      .exit:
	ret
endp