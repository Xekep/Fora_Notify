data	    9
	    dd a ; StartAddressOfRawData;
	    dd a ; EndAddressOfRawData
	    dd a ; AddressOfIndex
	    dd c ; AddressOfCallBacks
a	    dd 0 ;
c	    dd callback ; Array Of Callbacks
d	    dd 0
	    dd 0	; NULL - end of Array Of Callbacks
end data

proc	callback,handle,reason,reserved
	cmp [reason],DLL_PROCESS_ATTACH
	jnz end.
	xor ebx,ebx
	invoke CreateMutexA,ebx,ebx,'muttexx'
	invoke GetLastError
	.if eax<>0b7h
		mov [d],new_EP
		mov eax,dword [fs:18h]
		mov eax,[eax+30h]
		movzx eax,byte [eax+2]
		test eax,eax
		je @f
		;jmp @f
	       ; mov ecx,esp
	    ; .metke1:
	       ; inc ecx
	       ; cmp dword [ecx],start
	       ; jne .metke1
	       ; mov dword [ecx],new_EP
		ret
		@@:
		mov ebp,s13
		push ebp
		invoke GetStartupInfoA,s13
		invoke GetCommandLineA
		sub esp,sizeof.PROCESS_INFORMATION
		invoke CreateProcessA,ebx,eax,ebx,ebx,ebx,ebx,ebx,ebx,ebp,esp
		pop eax
		invoke WaitForSingleObject,eax,-1
		invoke ExitProcess,0
	.endif
	xor ebx,ebx
	invoke CreateMutexA,ebx,ebx,'muttexx2'
	invoke GetLastError
	.if eax<>0b7h
		mov [d],new_EP
		mov eax,dword [fs:18h]
		mov eax,[eax+30h]
		movzx eax,byte [eax+2]
		test eax,eax
		je @f
		;jmp @f
	      ;  mov ecx,esp
	     ;.metke2:
	      ;  inc ecx
	      ;  cmp dword [ecx],start
	      ;  jne .metke2
	      ;  mov dword [ecx],new_EP
		ret
		@@:
		invoke GetStartupInfoA,s13
		invoke GetCommandLineA
		invoke CreateProcessA,ebx,eax,ebx,ebx,ebx,DEBUG_PROCESS,ebx,ebx,s13,s14
		mov ebx,s15
		jmp l2
		l1:
		invoke ContinueDebugEvent,dword [s15+4],dword [s15+8],10002h;DBG_CONTINUE      [s14.dwProcessId]  [s14.dwThreadId]
		l2:
		invoke WaitForDebugEvent,ebx,-1
		cmp byte [ebx],5
		;EXIT_PROCESS_DEBUG_EVENT
		jne   l1
		invoke ExitProcess,0
	.endif
	mov [d],start
      ;  mov eax,dword [fs:18h]
      ;  mov eax,[eax+30h]
      ;  movzx eax,byte [eax+2]
      ;  test eax,eax
      ;  je @f
      ;  ;jmp @f
      ;  mov ecx,esp
      ;.metke:
      ;  inc ecx
      ;  cmp dword [ecx],start
      ;  jne .metke
      ;  mov dword [ecx],new_EP
end.:
	ret

	s13 STARTUPINFO
	s14 PROCESS_INFORMATION;
	s15 rb 60h;DEBUG_EVENT;
endp
new_EP:
	    invoke MessageBoxA,0,'Отладчик детектед (:',0,MB_ICONWARNING+MB_OK
	    int 3
	    invoke ExitProcess,0
	    ret