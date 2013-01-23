proc getlogin hwnd, msg, wparam, lparam
	push ebx edi esi
	cmp [msg],WM_INITDIALOG
	je .wminitdialog
	cmp [msg],WM_COMMAND
	je .wmcommand
	cmp [msg],WM_LBUTTONDOWN
	je .move
	cmp [msg],WM_CLOSE
	je .exit
	cmp [msg],WM_COMMAND
	je .wmcommand
	xor eax,eax
	jmp .finish
     .wmcommand:
	cmp [wparam],BN_CLICKED shl 16 + IDCANCEL
	je .exit2
	cmp [wparam],BN_CLICKED shl 16 + ID_SAVE
	je .save
	jmp .processed
     .processed:
	mov eax,1
     .finish:
	pop edi esi ebx
	ret
     .wminitdialog:
	invoke GetDlgItem,[hwnd],IDE_LOGIN
	push eax
	stdcall AddTooltip,eax,'№ Договора'
	pop eax
	invoke SetWindowLong,eax,GWL_WNDPROC,WindowProcLogin
	mov [lOldWndProc],eax
	invoke GetDlgItem,[hwnd],IDE_PASS
	push eax
	stdcall AddTooltip,eax,'Пароль'
	invoke RegOpenKeyExA,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke GlobalAlloc,GPTR,64
		mov [lhwnd],eax
		mov [vsize],64
		invoke RegQueryValueExA,[HKey],key1,0,lpType,[lhwnd],vsize
		.if eax=0
			stdcall TEAdecrypt,[lhwnd],[vsize]
			mov eax,[lhwnd]
			.if byte [eax]='f'
				invoke SetDlgItemTextA,[hwnd],IDE_LOGIN,[lhwnd]
				pop eax
				invoke SetWindowLong,eax,GWL_WNDPROC,WindowProcPass
				invoke GetDlgItem,[hwnd],ID_SAVE
				mov [lhwnd],eax
				invoke EnableWindow,eax,0
				invoke SetDlgItemTextA,[hwnd],IDE_PASS,'********'
			.endif
		.endif
		invoke RegCloseKey,[HKey]
		invoke GlobalFree,[lhwnd]
	.else
		add esp,4
	.endif
	invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,0,0,SWP_NOMOVE+SWP_NOSIZE
	invoke LoadIcon,[mhandle],ID_ICON
	invoke SendMessageA,[hwnd],WM_SETICON,ICON_BIG,eax
	jmp .processed
     .move:
	invoke SendMessage,[hwnd],0a1h,2,0
	jmp .processed
     .save:
	invoke GetDlgItem,[hwnd],IDE_LOGIN
	invoke GetWindowTextLengthA,eax
	.if eax=0
		invoke MessageBoxTimeoutA,[hwnd],'Введите логин',0,MB_ICONINFORMATION+MB_OK,0,3000
		jmp .processed
	.endif
	inc eax
	push eax
	invoke GlobalAlloc,GPTR,64
	mov [buff_login],eax
	pop ecx
	invoke GetDlgItemTextA,[hwnd],IDE_LOGIN,eax,ecx
	invoke GetLastError
	invoke GetDlgItem,[hwnd],IDE_PASS
	invoke GetWindowTextLengthA,eax
	.if eax=0
		invoke MessageBoxTimeoutA,[hwnd],'Введите пароль',0,MB_ICONINFORMATION+MB_OK,0,3000
		invoke GlobalFree,[buff_login]
		jmp .processed
	.endif
	inc eax
	push eax
	invoke GlobalAlloc,GPTR,64
	mov [buff_pass],eax
	pop ecx
	invoke GetDlgItemTextA,[hwnd],IDE_PASS,eax,ecx
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_WRITE,HKey
	.if eax<>0
		invoke RegCreateKeyA,key_root,subkey2,HKey
		.if eax<>0
			invoke MessageBoxTimeoutA,[hwnd],.errmsg,0,MB_ICONINFORMATION+MB_OK,0,3000
			invoke GlobalFree,[buff_pass]
			jmp .exit
		.endif
	.endif
	mov eax,[buff_login]
	.if byte [eax]<>'f'
		stdcall DialogPOPUP,"Логин должен начинаться с буквы 'f'",1
		invoke GlobalFree,[buff_login]
		invoke GlobalFree,[buff_pass]
		invoke RegCloseKey,[HKey]
		jmp .processed
	.endif
	invoke lstrlenA,[buff_login]
	stdcall TEAencrypt,[buff_login],eax
	invoke lstrlenA,[buff_login]
	invoke RegSetValueEx,[HKey],key1,0,REG_BINARY,[buff_login],eax
	.if eax<>0
		invoke MessageBoxTimeoutA,[hwnd],.errmsg,0,MB_ICONINFORMATION+MB_OK,0,3000
		invoke GlobalFree,[buff_pass]
		invoke GlobalFree,[buff_login]
		jmp .exit
	.endif
	invoke lstrlenA,[buff_pass]
	stdcall TEAencrypt,[buff_pass],eax
	invoke lstrlenA,[buff_login]
	invoke RegSetValueEx,[HKey],key2,0,REG_BINARY,[buff_pass],eax
	.if eax<>0
		invoke MessageBoxTimeoutA,[hwnd],.errmsg,0,MB_ICONINFORMATION+MB_OK,0,3000
	.endif
	@@:
	invoke GlobalFree,[buff_login]
	invoke GlobalFree,[buff_pass]
     .exit:
	invoke RegCloseKey,[HKey]
     .exit2:
	invoke EndDialog,[hwnd],0
	ret
   .errmsg db 'Ошибка сохранения данных',0
endp

proc WindowProcLogin hwnd, msg, wparam, lparam
	cmp [msg],WM_CHAR
	je .char
      .end!:
	invoke CallWindowProc,[lOldWndProc],[hwnd],[msg],[wparam],[lparam]
	ret
     .char:
	.if [wparam]='f' | [wparam]='F' | [wparam]='А' | [wparam]='а'
		 mov [wparam],'f'
		 jmp .end!
	.elseif [wparam]=VK_BACK
		 jmp .end!
	.elseif [wparam]>=30h & [wparam]<=39h
		 jmp .end!
	.endif
     .finish:
	xor eax,eax
	ret
endp

proc WindowProcPass hwnd, msg, wparam, lparam
       cmp [msg],WM_SETCURSOR
       je .clear
       cmp [msg],WM_SETFOCUS
       je .clear
     .end!:
       invoke CallWindowProc,[lOldWndProc],[hwnd],[msg],[wparam],[lparam]
       ret
     .clear:
       invoke SetWindowTextA,[hwnd],0
       invoke SetWindowLong,[hwnd],GWL_WNDPROC,[lOldWndProc]
       invoke EnableWindow,[lhwnd],1
       jmp .end!
endp