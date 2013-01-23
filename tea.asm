DELTASHL  =  0c6ef3720h
ROUND_TEA =  32
DELTA_TEA =  9e3779b9h

proc TEAencrypt text,size
     locals
	a	dd ?
	b	dd ?
	c	dd ?
	d	dd ?
	KeyB	rb 17
     endl

	pusha
	lea eax,[KeyB]
	invoke RtlZeroMemory,eax,17
	stdcall TEAgetkey
	lea esi,[KeyB]
	cinvoke wsprintfA,esi,'%0.8X%0.8X',ecx,eax

	xor edx,edx
	mov eax,[size]
	mov ebx,8
	div ebx
	.if edx<>0
	       inc eax
	.endif
	mov ecx,eax
      .start:
	push ecx

	mov	edx,[text]
	mov	ebx,dword[edx]
	mov	edx,dword[edx+4]

	bswap	ebx
	bswap	edx

	lea	esi,[KeyB]
	lea	edi,[a]
	mov	ecx,4
	rep	movsd

	xor	ecx,ecx

	mov	edi,ROUND_TEA
     @@:
	add	ecx,DELTA_TEA

	mov	eax,edx
	mov	esi,edx
	shl	eax,4
	shr	esi,5
	xor	esi,ecx
	add	eax,esi
	mov	esi,[a]
	xor	esi,edx
	add	eax,esi
	add	eax,[b]
	add	ebx,eax

	mov	eax,ebx
	mov	esi,ebx
	shl	eax,4
	shr	esi,5
	xor	esi,ecx
	add	eax,esi
	mov	esi,[c]
	xor	esi,ebx
	add	eax,esi
	add	eax,[d]
	add	edx,eax

	dec	edi
	jnz	@b

	mov	edi,[text]
	bswap	edx
	bswap	ebx

	mov	dword[edi],ebx
	mov	dword[edi+4],edx
	add [text],8
	pop ecx
	loop .start
	popa
	ret
endp

proc TEAdecrypt text,size
     locals
	a	dd ?
	b	dd ?
	c	dd ?
	d	dd ?
	KeyB	rb 17
     endl

	pusha
	lea eax,[KeyB]
	invoke RtlZeroMemory,eax,17
	stdcall TEAgetkey
	lea esi,[KeyB]
	cinvoke wsprintfA,esi,'%0.8X%0.8X',ecx,eax

	xor edx,edx
	mov eax,[size]
	mov ebx,8
	div ebx
	.if edx<>0
	       inc eax
	.endif
	mov ecx,eax
      .start:
	push ecx

	mov	edx,[text]
	mov	ebx,dword[edx]
	mov	edx,dword[edx+4]

	bswap	ebx
	bswap	edx

	lea	esi,[KeyB]
	lea	edi,[a]
	mov	ecx,4
	rep	movsd

	mov	ecx,DELTASHL

	mov	edi,ROUND_TEA
     @@:
	mov	eax,ebx
	mov	esi,ebx
	shl	eax,4
	shr	esi,5
	xor	esi,ecx
	add	eax,esi
	mov	esi,[c]
	xor	esi,ebx
	add	eax,esi
	add	eax,[d]
	sub	edx,eax

	mov	eax,edx
	mov	esi,edx
	shl	eax,4
	shr	esi,5
	xor	esi,ecx
	add	eax,esi
	mov	esi,[a]
	xor	esi,edx
	add	eax,esi
	add	eax,[b]
	sub	ebx,eax

	sub	ecx,DELTA_TEA

	dec	edi
	jnz	@b

	mov	edi,[text]
	bswap	edx
	bswap	ebx

	mov	dword[edi],ebx
	mov	dword[edi+4],edx

	add [text],8
	pop ecx
	loop .start
	popa
	ret
endp

proc TEAgetkey
	local wdir rb 256
	local arg1:DWORD
	local arg2:DWORD

	lea eax,[wdir]
	invoke GetWindowsDirectoryA,eax,256
	lea eax,[wdir]
	mov byte [eax+3],0
	lea ecx,[arg1]
	lea ebx,[arg2]
	invoke GetVolumeInformationA,eax,0,0,ebx,ecx,ecx,0,0
	imul ecx,[arg2],24924925h
	imul eax,[arg2],88C6E951h
	ret
endp				   