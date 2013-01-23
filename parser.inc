proc preg_match mat, str, str2, bool
	local mem:DWORD
	local start1:DWORD
	local flag:DWORD
	xor esi,esi
	mov [flag],0
	mov [start1],0
	.if [bool]=0
		invoke lstrlenA,[str]
		invoke GlobalAlloc,GPTR,eax
	.else
		mov eax,[str2]
		mov dword [eax],0
		mov eax,[str2]
	.endif
	mov [mem],eax
	mov ecx,[mem]
	mov edx,[mat]
	mov ebx,[str]
	mov al, byte [edx]
     pmmetka1:
	.if byte [ebx]=al & byte [edx]<>0 & al<>'*'
		inc [flag]
		inc edx
		inc ebx
		mov al, byte [edx]
		jmp pmmetka1
	.elseif byte [edx]<>0
		.if al='*'
			mov [start1],1
			inc edx
			mov al, byte [edx]
			jmp pmmetka1
		.elseif [start1]=1 & esi<=64
			push eax
			inc esi
			mov al,byte [ebx]
			mov byte [ecx],al
			pop eax
			inc ecx
			inc ebx
			jmp pmmetka1
		.elseif byte [edx]<>0 & byte [ebx]<>0
			.if [flag]<>0
				sub edx,[flag]
				mov [flag],0
				mov al,byte [edx]
			.else
				inc ebx
			.endif
			jmp pmmetka1
		.endif
	.endif
	mov byte [ecx],0
	.if [bool]=0
		invoke lstrcmpA,[mem],[str2]
		.if eax=-1 | eax>0
			mov eax,0
		.elseif eax=0
			mov eax,1
		.endif
		push eax
		invoke GlobalFree,[mem]
		pop eax
	.else
		mov eax,[str2]
		mov eax,dword [eax]
	.endif
	ret
endp