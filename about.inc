proc aboutproc hwnd, msg, wparam, lparam
	push ebx esi edi
	cmp [msg],WM_INITDIALOG
	je .wminitdialog
	cmp [msg],WM_CLOSE
	je .wmclose
	cmp [msg],WM_TIMER
	je .wmtimer
	cmp [msg],WM_RBUTTONUP
	je .endwnd
	cmp [msg],WM_LBUTTONDOWN
	je .move
	cmp [msg],WM_COMMAND
	je .wmcommand
	xor eax,eax
	jmp .finish
  .wmcommand:
	cmp [wparam],BN_CLICKED shl 16 + IDCANCEL
	je .wmclose
	jmp .processed
  .wminitdialog:
	invokex sound,2
	invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,0,0,SWP_NOMOVE+SWP_NOSIZE
	call .wminit
	jmp .processed
  .wmclose:
	invokex sound,3
	invoke EndDialog,[hwnd],0
	ret
  .processed:
	mov eax,1
  .finish:
	pop edi esi ebx
	ret
  .wmtimer:
	call .callmet_1
	call .callmet_2
	call .callmet_3
	call .callmet_4
	jmp .processed
  .endwnd:
	invoke DeleteObject,[perem10]
	invoke DeleteObject,[perem12]
	invoke KillTimer,[hwnd],123
	invoke KillTimer,[hwnd],143
	invoke SendMessageA,[hwnd],WM_CLOSE,0,0
	jmp .processed
  .move:
	invoke SendMessageA,[hwnd],WM_NCLBUTTONDOWN,2,0
	jmp .processed
     .wminit:
	mov edi,text
	or ecx,0FFFFFFFFh
	xor eax,eax
	xor edx,edx
	mov esi,1
	repne scas byte [edi]
	not ecx
	dec ecx
	mov dword [perem1],esi
	je .metka1
     .metka2:
	cmp byte [edx+text],0Ah
	jnz .metka3
	inc esi
     .metka3:
	mov edi,text
	or ecx,0FFFFFFFFh
	xor eax,eax
	inc edx
	repne scas byte [edi]
	not ecx
	dec ecx
	cmp edx,ecx
	jb .metka2
	mov dword [perem1],esi
     .metka1:
	mov dword [perem2],-28h
	mov dword [perem3],0A0h
	mov byte [perem4],0
	mov dword [perem5],0
	call .callmet2
	xor esi,esi
     .metka4:
	call .callmet1
	cdq
	mov ecx,15Eh
	idiv ecx
	mov edi,edx
	call .callmet1
	cdq
	mov ecx,0AFh
	idiv ecx
	lea edx,dword [edx+edi-15Eh]
	mov dword [esi*4+perem6],edx
	call .callmet1
	cdq
	mov ecx,0A0h
	idiv ecx
	mov edi,edx
	call .callmet1
	cdq
	mov ecx,50h
	idiv ecx
	lea eax,dword [esi+4Bh]
	mov dword [esi*4+perem7],eax
	inc esi
	cmp esi,1F4h
	lea edx,dword [edx+edi-0A0h]
	mov dword [esi*4+perem8],edx
	jl .metka4
	invoke SetTimer,[hwnd],123,5,0
	invoke SetTimer,[hwnd],143,143000,0
	retn
     .callmet1:
	mov eax,dword [perem9]
	imul eax,eax,343FDh
	add eax,269EC3h
	mov dword [perem9],eax
	sar eax,10h
	and eax,7FFFh
	retn
     .callmet2:
	invoke GetDC,[hwnd]
	mov dword [perem10],eax
	invoke GetWindowRect,[hwnd],perem11
	invoke CreateCompatibleDC,[perem10]
	mov dword [perem12],eax
	invoke CreateCompatibleBitmap,[perem10],[perem11.left],[perem11.top]
	mov dword [perem13],eax
	invoke SelectObject,[perem12],[perem13]
	invoke DeleteObject,[perem13]
	invoke DeleteObject,perem11
	retn
    .callmet_1:
	invoke GetWindowRect,[hwnd],perem11
	invoke BeginPaint,[hwnd],perem14
	invoke BitBlt,[perem12],0,0,[perem11.right],[perem11.bottom],0,0,0,BLACKNESS
	invoke EndPaint,[hwnd],perem14
	invoke DeleteObject,perem11
	invoke DeleteObject,perem14
	retn
    .callmet_2:
	push esi
	push edi
	xor edi,edi
    .metka8:
	mov eax,dword [edi*4+perem6]
	mov esi,dword [edi*4+perem7]
	push 0
	lea eax,dword [eax+eax*2]
	lea eax,dword [eax+eax*4]
	lea eax,dword [eax+eax*4]
	shl eax,1
	cdq
	idiv esi
	mov ecx,eax
	mov eax,dword [edi*4+perem15]
	imul eax,eax,64h
	cdq
	idiv esi
	add ecx,0AFh
	mov dword [perem16],ecx
	add eax,50h
	mov dword [perem17],eax
	push eax
	mov eax,dword [perem12]
	push ecx
	push eax
	invoke SetPixelV
	mov eax,dword [edi*4+perem7]
	lea ecx,dword [eax-1]
	mov eax,dword [edi*4+perem6]
	mov dword [edi*4+perem7],ecx
	lea eax,dword [eax+eax*2]
	lea eax,dword [eax+eax*4]
	lea eax,dword [eax+eax*4]
	shl eax,1
	cdq
	idiv ecx
	mov esi,eax
	mov eax,dword [edi*4+perem15]
	imul eax,eax,64h
	cdq
	idiv ecx
	add esi,0AFh
	mov dword [perem16],esi
	add eax,50h
	cmp ecx,0FFh
	mov dword [perem17],eax
	jl .metka5
	mov edx,32h
	jmp .metka6
    .metka5:
	mov edx,131h
	sub edx,ecx
    .metka6:
	mov ecx,edx
	mov dword [perem18],edx
	and ecx,0FFh
	mov edx,ecx
	shl edx,8
	or edx,ecx
	shl edx,8
	;or edx,ecx    ; <-------цвет
	push edx
	push eax
	mov eax,dword [perem12]
	push esi
	push eax
	invoke SetPixelV
	cmp dword [edi*4+perem7],1
	jg .metka7
	call .callmet1
	cdq
	mov ecx,15Eh
	idiv ecx
	mov esi,edx
	call .callmet1
	cdq
	mov ecx,0AFh
	idiv ecx
	lea edx,dword [edx+esi-15Eh]
	mov dword [edi*4+perem6],edx
	call .callmet1
	cdq
	mov ecx,0A0h
	idiv ecx
	mov esi,edx
	call .callmet1
	cdq
	mov ecx,50h
	idiv ecx
	lea eax,dword [edi+4Bh]
	mov dword [edi*4+perem7],eax
	lea edx,dword [edx+esi-0A0h]
	mov dword [edi*4+perem15],edx
    .metka7:
	inc edi
	cmp edi,1F4h
	jl .metka8
	pop edi
	pop esi
	retn
    .callmet_3:
	mov ecx,dword [perem19]
	mov edx,dword [perem2]
	mov eax,dword [perem3]
	add ecx,edx
	mov dl,byte [perem4]
	mov dword [perem11.left],ecx
	mov ecx,dword [perem1]
	mov dword [perem11.top],eax
	test dl,dl
	lea ecx,dword [ecx+ecx*4+28h]
	mov dword [perem11.right],15Eh
	lea ecx,dword [eax+ecx*4]
	mov dword [perem11.bottom],ecx
	jnz .metka15
	mov edx,dword [perem5]
	cmp edx,3
	je .metka9
	test edx,edx
	jnz .metka10
     .metka9:
	dec eax
	mov dword [perem5],1
	mov dword [perem3],eax
	jmp .metka11
    .metka10:
	inc edx
	mov dword [perem5],edx
    .metka11:
	neg ecx
	cmp eax,ecx
	jge .metka12
	mov byte [perem4],1
	jmp .metka12
    .metka15:
	mov ecx,dword [perem5]
	cmp ecx,3
	je .metka13
	test ecx,ecx
	jnz .metka14
    .metka13:
	inc eax
	mov dword [perem5],1
	mov dword [perem3],eax
	jmp .metka16
    .metka14:
	inc ecx
	mov dword [perem5],ecx
    .metka16:
	cmp eax,0A0h
	jle .metka12
	mov byte [perem4],0
    .metka12:
	mov edx,dword [perem12]
	push esi
	push edi
	invoke GetDeviceCaps,edx,5Ah,48h
	invoke GetDeviceCaps,[perem12],5ah ;LOGPIXELSY
	invoke MulDiv,8,eax
	neg eax
	mov dword [perem21],eax
	invoke CreateFontA,eax,0,0,0,FW_BOLD,0,0,0,ANSI_CHARSET or DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Verdana"
	mov dword [perem22],eax
	invoke SelectObject,[perem12],[perem22]
	invoke SetTextColor,[perem12],0FFFFFFh
	invoke SetBkColor,[perem12],6D5F52h
	invoke SetBkMode,[perem12],1
	mov edi,text
	or ecx,0FFFFFFFFh
	xor eax,eax
	repne scas byte [edi]
	not ecx
	dec ecx
	invoke DrawTextA,[perem12],text,ecx,perem11,1
	invoke DeleteObject,[perem22]
	invoke DeleteObject,perem11
	pop edi
	pop esi
	retn
    .callmet_4:
	invoke GetDC,[hwnd]
	mov dword [perem10],eax
	invoke GetWindowRect,[hwnd],perem11
	invoke BeginPaint,[hwnd],perem14
	invoke BitBlt,[perem10],0,0,[perem11.right],[perem11.bottom],[perem12],0,0,SRCCOPY
	invoke EndPaint,[hwnd],perem14
	invoke DeleteObject,perem14
	invoke DeleteObject,perem11
	invoke DeleteObject,[perem10]
	retn
endp