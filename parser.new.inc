;PARSER by Xekep
proc preg_match mat, str, str2, bool, leng, sym
	local mem:DWORD
	local start1:DWORD
	local flag:DWORD
	local leng2:DWORD
	local flag2:DWORD
	mov [flag2],0
	xor esi,esi
	mov [flag],0
	mov [start1],0
	.if [bool]=0
		invoke lstrlenA,[str]
		mov [leng2],eax
		invoke GlobalAlloc,GPTR,eax
	.else
		mov eax,[leng]
		mov [leng2],eax
		mov eax,[str2]
		mov dword [eax],0
		mov eax,[str2]
	.endif
	mov [mem],eax
	mov ecx,[mem]
	mov edx,[mat]
	mov ebx,[str]
	mov al,byte [edx]
	xor edi,edi;
     pmmetka1:
	.if byte [ebx]=al & byte [edx]<>0 & al<>'*' & edi<>2
		inc [flag]
		inc edx
		inc ebx
		inc edi;
		mov al, byte [edx]
		.if edi<>0 & byte [ebx]<>al ;
			inc edi
			mov [start1],1
			dec edx
			dec ebx
			dec [flag]
		.else
			xor edi,edi
			mov [start1],0
		.endif;
		jmp pmmetka1
	.elseif byte [edx]<>0
		.if al='*'
			.if [bool]>0 & ecx<>[mem] & [sym]<>-1 & [flag2]=0
			       mov eax,[sym]
			       .if eax<00000100h
				       mov byte [ecx],al
				       inc esi
				       inc ecx
			       .elseif eax<00010000h
				       mov word [ecx],ax
				       add esi,2
				       add ecx,2
			       .endif
			.endif
			mov [flag2],0
			mov [start1],1
			inc edx
			mov al, byte [edx]
			jmp pmmetka1
		.elseif [start1]=1 & esi<=[leng2] & byte [ebx]<>0
			xor edi,edi
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
		.elseif byte [ebx]=0 & [flag2]=0
			.if [bool]=0
				invoke GlobalFree,[mem]
			.endif
			xor eax,eax
			ret
		.endif
	.elseif [bool]=2 & byte [ebx]<>0
		mov [flag2],1
		mov word [ecx],0
		add ecx,2
		add esi,2
		mov [flag],0
		mov [start1],0
		mov edx,[mat]
		mov al,byte [edx]
		jmp pmmetka1
	.endif
	mov dword [ecx],0
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