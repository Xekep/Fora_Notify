proc DialogPOPUP arg1, arg2
	stdcall getwinfullscr
	.if eax=1
		ret
	.endif
	.if [notify_flag]=1 | dword [arg2]=1
		invoke lstrlenA,[arg1]
		inc eax
		invoke GlobalAlloc,GPTR,eax
		push eax
		invoke lstrcpyA,eax,[arg1]
		pop eax
		invoke CreateThread,0,0,.metka1,eax,0,0
	.endif
	ret
endp
proc .metka1 arg1
	call set_seh
	invoke DialogBoxParam,[mhandle],IDD_POPUP,HWND_DESKTOP,POPUP,[arg1]
	invoke GlobalFree,[arg1]
	invoke ExitThread,0
endp

proc POPUP hwnddlg,msg,wparam,lparam
	local param:DWORD
	local screen:RECT  ; ������� ������
	local coord:RECT   ; ������� ����
	local lptm:TEXTMETRIC
	push	ebx esi edi
	cmp	[msg],WM_INITDIALOG
	je	.wminitdialog
	cmp	[msg],WM_COMMAND
	je	.wmcommand
	cmp	[msg],WM_TIMER
	je	.wmclose
	cmp	[msg],WM_LBUTTONDOWN
	je	.popdown_window
	cmp	[msg],WM_RBUTTONDOWN
	je	.popdown_window
	xor	eax,eax
	jmp	.finish
.wminitdialog:
	invoke	SetDlgItemTextA,[hwnddlg],ID_TEXT,[lparam]
	invoke GetDlgItem,[hwnddlg],ID_TEXT
	push eax
	invoke GetDC,eax
	push eax
	lea ecx,[lptm]
	invoke GetTextMetrics,eax,ecx
	pop eax
	pop ecx
	invoke ReleaseDC,ecx,eax
	invoke lstrlenA,[lparam]
	mov ecx,eax
	mov esi,[lparam]
	xor eax,eax
	xor edx,edx
      .metka1:
	.if edx>46
		inc eax
		xor edx,edx
	     .metka5:
		.if byte [esi]<>20h
			inc edx
			dec esi
			jmp .metka5
		.endif
		add esi,edx
	.elseif byte [esi]=0Ah
		inc eax
		xor edx,edx
	.elseif
		inc edx
	.endif
	inc esi
	loop .metka1
	.if eax>0
		xor edx,edx
		mov ebx,[lptm.tmHeight]
		sub ebx,[lptm.tmInternalLeading]
		mul ebx
		mov [param],eax
		lea eax,[coord]
		invoke GetWindowRect,[hwnddlg],eax
		mov eax,[coord.top]
		sub [coord.bottom],eax
		mov eax,[coord.left]
		sub [coord.right],eax
		mov eax,[param]
		add [coord.bottom],eax
		invoke SetWindowPos,[hwnddlg],-1,0,0,[coord.right],[coord.bottom],SWP_NOMOVE
		invoke GetDlgItem,[hwnddlg],ID_TEXT
		push eax
		lea ecx,[coord]
		invoke GetClientRect,eax,ecx
		mov eax,[param]
		add [coord.bottom],eax
		pop eax
		invoke SetWindowPos,eax,HWND_TOP,0,0,[coord.right],[coord.bottom],SWP_NOMOVE
		invoke GetDlgItem,[hwnddlg],0
		push eax
		lea ecx,[coord]
		invoke GetClientRect,eax,ecx
		mov eax,[param]
		add [coord.bottom],eax
		pop eax
		invoke SetWindowPos,eax,HWND_TOP,0,0,[coord.right],[coord.bottom],SWP_NOMOVE
	.endif
	; �������� ������ ������� ������� ������
	lea eax,[screen]
	invoke	SystemParametersInfo,SPI_GETWORKAREA,NULL,eax,FALSE

	; �������� ������ ����
	lea eax,[coord]
	invoke	GetClientRect,[hwnddlg],eax

	add [coord.right],4; FIX

	; ��������� ������ �� ������ ������� ������
	mov	eax,[screen.right]
	sub	eax,[coord.right]
	dec	eax
	mov	[screen.right],eax

	; ������ ����
	mov	ecx,[coord.bottom]
	; ���� �������� ����
@@:
	; ��������� �������� ��������
	push	ecx

	; ��������� ���� ��� MoveWindow
	push	TRUE
	mov	eax,[coord.bottom]
	sub	eax,ecx
	push	eax
	push	[coord.right]
	mov	ecx,[screen.bottom]
	sub	ecx,eax
	push	ecx
	invoke	MoveWindow,[hwnddlg],[screen.right]

	; ��� ��� ������ ������ �������������, �� �������� �������������
	; �������� ���� � �������� ��� ����������
	invoke	ShowWindow,[hwnddlg], SW_SHOW
	invoke	UpdateWindow,[hwnddlg]
	; ��������� �����
	invoke	Sleep,2

	; ������������ �������
	pop	ecx
	loop	@b

	.if [flag_sec]=1
		stdcall _InterlockedDecrement,flag_sec,0
		mov eax,TIME_POPUP2
	.else
		mov eax,TIME_POPUP
	.endif

	; ���������� ������ �������� ���� �� 2 �������
	invoke	SetTimer,[hwnddlg],TIMER_ID,eax,NULL
	jmp	.processed
.wmcommand:
	jmp	.processed
.wmclose:
	; ��� ��� ������?
	cmp	[wparam],TIMER_ID
	jne	.processed
.popdown_window:
	; �������� ������ ������� ������� ������
	lea eax,[screen]
	invoke	SystemParametersInfo,SPI_GETWORKAREA,NULL,eax,FALSE

	; �������� ������ ����
	lea eax,[coord]
	invoke	GetClientRect,[hwnddlg],eax

	add [coord.right],6; FIX

	; ��������� ������ �� ������ ������� ������
	mov	eax,[screen.right]
	sub	eax,[coord.right]
	dec	eax
	mov	[screen.right],eax

	; ������ ����
	mov	ecx,[coord.bottom]
	; ���� �������� ����
@@:
	; ��������� �������� ��������
	push	ecx

	; ��������� ���� ��� MoveWindow
	push	TRUE
	push	ecx
	push	[coord.right]
	mov	eax,[screen.bottom]
	sub	eax,ecx
	push	eax
	invoke	MoveWindow,[hwnddlg],[screen.right]

	; �������� ���������� ����
	invoke	UpdateWindow,[hwnddlg]

	; ��������� �����
	invoke	Sleep,2

	; ������������ �������
	pop	ecx
	loop	@b

	; ������� ������
	invoke	KillTimer,[hwnddlg],TIMER_ID

	; ������� ���������� ����
	invoke	EndDialog,[hwnddlg],0
	ret
.processed:
	mov	eax,1
.finish:
	pop	edi esi ebx
	ret
endp