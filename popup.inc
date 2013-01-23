proc DialogPOPUP arg1, arg2
	invokex getwinfullscr
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
	;;;
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
		mov ebx,13
		mul ebx
		mov [param],eax
		invoke GetWindowRect,[hwnddlg],coord
		mov eax,[coord.top]
		sub [coord.bottom],eax
		mov eax,[coord.left]
		sub [coord.right],eax
		mov eax,[param]
		add [coord.bottom],eax
		invoke SetWindowPos,[hwnddlg],-1,0,0,[coord.right],[coord.bottom],SWP_NOMOVE
		invoke GetDlgItem,[hwnddlg],ID_TEXT
		push eax
		invoke GetClientRect,eax,coord
		mov eax,[param]
		add [coord.bottom],eax
		pop eax
		invoke SetWindowPos,eax,HWND_TOP,0,0,[coord.right],[coord.bottom],SWP_NOMOVE
		invoke GetDlgItem,[hwnddlg],0
		push eax
		invoke GetClientRect,eax,coord
		mov eax,[param]
		add [coord.bottom],eax
		pop eax
		invoke SetWindowPos,eax,HWND_TOP,0,0,[coord.right],[coord.bottom],SWP_NOMOVE
	.endif
	;;;
	; Получить размер рабочей области экрана
	invoke	SystemParametersInfo,SPI_GETWORKAREA,NULL,screen,FALSE

	; Получить размер окна
	invoke	GetClientRect,[hwnddlg],coord

	; Вычислить отступ от правой границы экрана
	mov	eax,[screen.right]
	sub	eax,[coord.right]
	dec	eax
	mov	[screen.right],eax

	; Высота окна
	mov	ecx,[coord.bottom]
	; Цикл всплытия окна
@@:
	; Сохранить значение счетчика
	push	ecx

	; Заполняем стек для MoveWindow
	push	TRUE
	mov	eax,[coord.bottom]
	sub	eax,ecx
	push	eax
	push	[coord.right]
	mov	ecx,[screen.bottom]
	sub	ecx,eax
	push	ecx
	invoke	MoveWindow,[hwnddlg],[screen.right]

	; Так как сейчас только инициализация, то придется принудительно
	; показать окно и обновить его содержимое
	invoke	ShowWindow,[hwnddlg], SW_SHOW
	invoke	UpdateWindow,[hwnddlg]
	; Небольшая пауза
	invoke	Sleep,2

	; Восстановить счетчик
	pop	ecx
	loop	@b
	.if [flag_sec]=1
		invoke InterlockedDecrement,flag_sec
		mov eax,TIME_POPUP2
	.else
		mov eax,TIME_POPUP
	.endif
	; Установить таймер закрытия окна на 2 секунды
	invoke	SetTimer,[hwnddlg],TIMER_ID,eax,NULL

	jmp	.processed
.wmcommand:
	jmp	.processed
.wmclose:
	; Это наш таймер?
	cmp	[wparam],TIMER_ID
	jne	.processed
.popdown_window:
	; Высота окна
	mov	ecx,[coord.bottom]
	; Цикл закрытия окна
@@:
	; Сохранить значение счетчика
	push	ecx

	; Заполняем стек для MoveWindow
	push	TRUE
	push	ecx
	push	[coord.right]
	mov	eax,[screen.bottom]
	sub	eax,ecx
	push	eax
	invoke	MoveWindow,[hwnddlg],[screen.right]

	; Обновить содержимое окна
	invoke	UpdateWindow,[hwnddlg]

	; Небольшая пауза
	invoke	Sleep,2

	; Восстановить счетчик
	pop	ecx
	loop	@b

	; Удалить таймер
	invoke	KillTimer,[hwnddlg],TIMER_ID

	; Закрыть диалоговое окно
	invoke	EndDialog,[hwnddlg],0
	ret
.processed:
	mov	eax,1
.finish:
	pop	edi esi ebx
	ret
endp