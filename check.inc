proc check hwnd
	call set_seh
	invoke WaitForSingleObject,[gcontent_mutex],-1
	.if eax=WAIT_TIMEOUT
		jmp .exit
	.endif
	stdcall get_content,[hwnd],1
	invoke ReleaseMutex,[gcontent_mutex]
     .exit:
	invoke ExitThread,0
endp

proc check_money hwnd
	call set_seh
	invoke WaitForSingleObject,[gcontent_mutex],0
	.if eax=WAIT_TIMEOUT
		jmp .exit
	.endif
	stdcall get_content,[hwnd],2
	invoke ReleaseMutex,[gcontent_mutex]
     .exit:
	invoke ExitThread,0
endp