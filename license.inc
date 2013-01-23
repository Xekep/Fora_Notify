license FILE 'License.txt'
db 0

ID_ACCEPT = 1
ID_DECLINE = 2
IDE_LTEXT = 3
TIMER_LACCEPT = 77
TIME_LACCEPT = 1000
COUNT = 10

proc plicense hwnd, msg, wparam, lparam
	push ebx edi esi
	cmp [msg],WM_INITDIALOG
	je .wminitdialog
	cmp [msg],WM_COMMAND
	je  .wmcommand
	cmp [msg],WM_LBUTTONDOWN
	je  .move
	cmp [msg],WM_TIMER
	je .timer
	cmp [msg],WM_CLOSE
	je  .decline
	xor eax,eax
	jmp .finish
   .wminitdialog:
	call set_seh
	invoke SetDlgItemTextA,[hwnd],4,'Copyright © 2010, Xekep. All rights reserved.'
	invoke LoadIcon,[mhandle],ID_ICON
	invoke SendMessageA,[hwnd],WM_SETICON,ICON_BIG,eax
	.if [lparam]=0
		invoke GetDlgItem,[hwnd],ID_ACCEPT
		invoke EnableWindow,eax,0
		invoke SetTimer,[hwnd],TIMER_LACCEPT,TIME_LACCEPT,0
		invoke GlobalAlloc,GPTR,64
		mov [lmem],eax
		mov [time_count],COUNT
		invoke wsprintfA,[lmem],.button,[time_count]
		invoke SetDlgItemTextA,[hwnd],ID_ACCEPT,[lmem]
	.else
		invoke GetDlgItem,[hwnd],ID_DECLINE
		invoke EnableWindow,eax,0
	.endif
	invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,0,0,SWP_NOMOVE+SWP_NOSIZE
	invoke SetDlgItemTextA,[hwnd],IDE_LTEXT,license
	jmp .processed
   .wmcommand:
	cmp [wparam],BN_CLICKED shl 16 + ID_ACCEPT
	je .accept
	cmp [wparam],BN_CLICKED shl 16 + ID_DECLINE
	je .decline
	jmp .processed
    .processed:
	mov eax,1
    .finish:
	pop edi esi ebx
	ret
    .move:
	invoke SendMessage,[hwnd],0a1h,2,0
	jmp .processed
   .timer:
	dec [time_count]
	.if [time_count]=0
		invoke SetDlgItemTextA,[hwnd],ID_ACCEPT,'Принять'
		invoke GetDlgItem,[hwnd],ID_ACCEPT
		invoke EnableWindow,eax,1
		invoke KillTimer,[hwnd],TIMER_LACCEPT
		invoke GlobalFree,[lmem]
		jmp .processed
	.endif
	invoke wsprintfA,[lmem],.button,[time_count]
	invoke SetDlgItemTextA,[hwnd],ID_ACCEPT,[lmem]
	jmp .processed
.accept:
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
	.if eax<>0
		invokex DialogPOPUP,stat_err_reg,0
		jmp .decline
	.endif
	invoke RegSetValueEx,[HKey],key_name7,0,REG_BINARY,flagg,1
	.if eax<>0
		invokex DialogPOPUP,stat_err_reg,0
		invoke RegCloseKey,[HKey]
		jmp .decline
	.endif
	invoke RegCloseKey,[HKey]
	invoke EndDialog,[hwnd],0
	ret
.decline:
	invoke EndDialog,[hwnd],0
	.if [flag_license]<>1
		.if [time_count]<>0
			invoke GlobalFree,[lmem]
		.endif
		invoke ExitProcess,0
	.endif
	ret
	.button db 'Принять (%u)',0
endp

proc getlicense
	mov [vsize],1
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax<>0
		invoke RegCreateKeyA,key_root,subkey2,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
		.endif
	.else
		invoke RegQueryValueExA,[HKey],key_name7,0,lpType,flag_license,vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			.if [flag_license]=1
				mov eax,1
				ret
			.endif
		.else
			invoke RegCloseKey,[HKey]
		.endif
	.endif
	xor eax,eax
	mov [firstrun],2
	ret
endp