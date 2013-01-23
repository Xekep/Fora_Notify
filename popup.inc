DialogPOPUP:
	.if [notify_flag]=1 | dword [esp+8]=1
		mov eax,[esp+4]
		mov [strg],eax
		invoke CreateThread,0,0,dpmetka1,0,0,0
	.endif
	ret
      dpmetka1:
	invoke	DialogBoxParam,[mhandle],IDD_POPUP,HWND_DESKTOP,POPUP,0
	invoke	ExitThread,0

proc POPUP hwnddlg,msg,wparam,lparam
	push	ebx esi edi
	cmp	[msg],WM_INITDIALOG
	je	.wminitdialog
	cmp	[msg],WM_COMMAND
	je	.wmcommand
	cmp	[msg],WM_TIMER
	je	.wmclose
	cmp	[msg],WM_LBUTTONDOWN
	je	.popdown_window
	xor	eax,eax
	jmp	.finish
.wminitdialog:
	invoke	SetDlgItemTextA,[hwnddlg],ID_TEXT,[strg]
	; �������� ������ ������� ������� ������
	invoke	SystemParametersInfo,SPI_GETWORKAREA,NULL,screen,FALSE

	; �������� ������ ����
	invoke	GetClientRect,[hwnddlg],coord

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

	; ���������� ������ �������� ���� �� 2 �������
	invoke	SetTimer,[hwnddlg],TIMER_ID,2000,NULL

	jmp	.processed
.wmcommand:
	jmp	.processed
.wmclose:
	; ��� ��� ������?
	cmp	[wparam],TIMER_ID
	jne	.processed

.popdown_window:
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
	invoke	ExitThread,0
.processed:
	mov	eax,1
.finish:
	pop	edi esi ebx
	ret
endp