ID_SEND = 1
ID_SEND2  = 5
ID_TEXT = 2
ID_EMAIL = 3
ID_NAME = 4
TIME = 30

proc ppost hwnd, msg, wparam, lparam
	cmp [msg],WM_TIMER
	je .timer
	cmp [msg],WM_INITDIALOG
	je .wminitdialog
	cmp [msg],WM_COMMAND
	je  .wmcommand
	cmp [msg],WM_LBUTTONDOWN
	je  .move
	cmp [msg],WM_CLOSE
	je  .wmclose
	xor eax,eax
	jmp .finish
      .wminitdialog:
	invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,0,0,SWP_NOMOVE+SWP_NOSIZE
	invoke LoadIcon,[mhandle],ID_ICON
	invoke SendMessageA,[hwnd],WM_SETICON,ICON_BIG,eax
	.if [post_time]<>0
		 push [post_time]
		 cinvoke time,post_time
		 pop ecx
		 sub eax,ecx
		 mov ecx,[post_count]
		 sub ecx,eax
		 .if ecx<TIME & ecx<>0
			 mov [post_count],ecx
			 invoke GetDlgItem,[hwnd],ID_SEND
			 push eax
			 invoke EnableWindow,eax,0
			 pop eax
			 invoke SetWindowLongA,eax,GWL_ID,ID_SEND2,0
			 invoke SetTimer,[hwnd],TIMER_POST,TIME_POST,0
			 dec [post_count]
		 .endif
	.endif
	jmp .processed
    .timer:
	.if [post_count]<>0
		dec [post_count]
	.else
		invoke SetDlgItemTextA,[hwnd],ID_SEND2,'Отправить'
		invoke GetDlgItem,[hwnd],ID_SEND2
		push eax
		invoke EnableWindow,eax,1
		pop eax
		invoke SetWindowLongA,eax,GWL_ID,ID_SEND,0
		invoke KillTimer,[hwnd],TIMER_POST
		mov [post_time],0
		jmp .processed
	.endif
	cinvoke wsprintfA,post_button,'%u', [post_count]
	invoke SetDlgItemTextA,[hwnd],ID_SEND2,post_button
	jmp .processed
    .move:
	invoke SendMessage,[hwnd],0a1h,2,0
	jmp .processed
    .wmcommand:
	cmp [wparam],BN_CLICKED shl 16 + ID_SEND
	je .send
	jmp .processed
    .wmclose:
	invoke EndDialog,[hwnd],0
    .processed:
	mov eax,1
    .finish:
	pop edi esi ebx
	ret
    .send:
	invoke GetDlgItem,[hwnd],ID_TEXT
	invoke GetWindowTextLengthA,eax
	.if eax=0
		stdcall DialogPOPUP,'Необходимо напечатать текст сообщения!',1
		jmp .processed
	.elseif eax>1024
		stdcall DialogPOPUP,'Слишком длинное сообщение!',1
		jmp .processed
	.endif
	invoke GetDlgItem,[hwnd],ID_NAME
	invoke GetWindowTextLengthA,eax
	.if eax=0
		stdcall DialogPOPUP,'Необходимо ввести своё имя/ник!',1
		jmp .processed
	.elseif eax>21
		stdcall DialogPOPUP,'Слишком длинное имя!',1
		jmp .processed
	.endif
	invoke GetDlgItem,[hwnd],ID_EMAIL
	invoke GetWindowTextLengthA,eax
	.if eax=0
		stdcall DialogPOPUP,'Необходимо ввести свой контактный E-mail!',1
		jmp .processed
	.elseif eax>35
		stdcall DialogPOPUP,'Слишком длинный E-mail!',1
		jmp .processed
	.endif
	invoke CreateThread,0,0,thr_send,[hwnd],0,0
	jmp .processed
endp

proc thr_send hwnd
	local post_mem:DWORD
	local post_msg:DWORD
	invoke GetDlgItem,[hwnd],ID_SEND
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_TEXT
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_EMAIL
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_NAME
	invoke EnableWindow,eax,0
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE
	mov [post_mem],eax
	invoke lstrcpyA,[post_mem],'name='
	dec edx
	invoke GetDlgItemTextA,[hwnd],ID_NAME,edx,21
	invoke lstrcatA,[post_mem],'&email='
	invoke lstrlenA,[post_mem]
	add eax,[post_mem]
	invoke GetDlgItemTextA,[hwnd],ID_EMAIL,eax,35
	invoke lstrcatA,[post_mem],'&text='
	invoke lstrlenA,[post_mem]
	add eax,[post_mem]
	invoke GetDlgItemTextA,[hwnd],ID_TEXT,eax,1024
	stdcall request,[post_mem],host2,'/update.php?fora&msg','XEKEP','POST',[post_mem],MAX_FILE_SIZE,0
	.if eax=1
		invoke GlobalAlloc,GPTR,MAX_FILE_SIZE
		mov [post_msg],eax
		stdcall preg_match,'[msg=*]',[post_mem],[post_msg],1,MAX_FILE_SIZE,-1
		.if eax<>0
			invoke lstrcmpA,[post_msg],'OK!'
			.if eax=0
				stdcall DialogPOPUP,'Отзыв отправлен!',1
				mov [post_count],TIME
				cinvoke time,post_time
				invoke GetDlgItem,[hwnd],ID_SEND
				invoke SetWindowLongA,eax,GWL_ID,ID_SEND2,0
				invoke SetTimer,[hwnd],TIMER_POST,TIME_POST,0
			.else
				stdcall DialogPOPUP,[post_msg],1
				stdcall preg_match,'[st=*]',[post_mem],'1',0,0,-1
				.if eax<>0
					mov [post_count],TIME
					cinvoke time,post_time
					invoke GetDlgItem,[hwnd],ID_SEND
					invoke SetWindowLongA,eax,GWL_ID,ID_SEND2,0
					invoke SetTimer,[hwnd],TIMER_POST,TIME_POST,0
				.else
					invoke GetDlgItem,[hwnd],ID_SEND
					invoke EnableWindow,eax,1
				.endif
			.endif
		.else
			invoke GetDlgItem,[hwnd],ID_SEND
			invoke EnableWindow,eax,1
			stdcall DialogPOPUP,'ERR!',1
		.endif
		invoke GlobalFree,[post_msg]
	.else
		invoke GetDlgItem,[hwnd],ID_SEND
		invoke EnableWindow,eax,1
		stdcall DialogPOPUP,'ERR!',1
	.endif
	invoke GetDlgItem,[hwnd],ID_TEXT
	invoke EnableWindow,eax,1
	invoke GetDlgItem,[hwnd],ID_EMAIL
	invoke EnableWindow,eax,1
	invoke GetDlgItem,[hwnd],ID_NAME
	invoke EnableWindow,eax,1
	invoke GlobalFree,[post_mem]
	invoke ExitThread,0
endp