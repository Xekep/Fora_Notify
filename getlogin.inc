proc getlogin hwnd, msg, wparam, lparam
	push ebx edi esi
	cmp [msg],WM_COMMAND
	je .wmcommand
	cmp [msg],WM_LBUTTONDOWN
	je .move
	cmp [msg],WM_CLOSE
	je .exit
	xor eax,eax
	jmp .finish
     .wmcommand:
	cmp [wparam],BN_CLICKED shl 16 + ID_SAVE
	je .save
	jmp .processed
     .processed:
	mov eax,1
     .finish:
	pop edi esi ebx
	ret
     .move:
	invoke SendMessage,[hwnd],0a1h,2,0
	jmp .processed
     .save:
	invoke GetDlgItem,[hwnd],IDE_LOGIN
	invoke GetWindowTextLengthA,eax
	.if eax=0
		invoke MessageBoxA,[hwnd],'Введите логин',0,MB_ICONINFORMATION+MB_OK
		jmp .processed
	.endif
	inc eax
	push eax
	invoke GlobalAlloc,GPTR,eax
	mov [buff_login],eax
	pop ecx
	invoke GetDlgItemTextA,[hwnd],IDE_LOGIN,eax,ecx
	invoke GetLastError
	invoke GetDlgItem,[hwnd],IDE_PASS
	invoke GetWindowTextLengthA,eax
	.if eax=0
		invoke MessageBoxA,[hwnd],'Введите пароль',0,MB_ICONINFORMATION+MB_OK
		invoke GlobalFree,[buff_login]
		jmp .processed
	.endif
	inc eax
	push eax
	invoke GlobalAlloc,GPTR,eax
	mov [buff_pass],eax
	pop ecx
	invoke GetDlgItemTextA,[hwnd],IDE_PASS,eax,ecx
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_WRITE,HKey
	.if eax<>0
		invoke RegCreateKeyA,key_root,subkey2,HKey
		.if eax<>0
			invoke MessageBoxA,[hwnd],'Ошибка сохранения данных',0,0
			invoke GlobalFree,[buff_pass]
			jmp .exit
		.endif
	.endif
	invoke lstrlenA,[buff_login]
	invoke RegSetValueEx,[HKey],key1,0,REG_SZ,[buff_login],eax
	.if eax<>0
		invoke MessageBoxA,[hwnd],'Ошибка сохранения данных',0,0
		invoke GlobalFree,[buff_pass]
		invoke GlobalFree,[buff_login]
		jmp .exit
	.endif
	invoke lstrlenA,[buff_pass]
	invoke RegSetValueEx,[HKey],key2,0,REG_SZ,[buff_pass],eax
	.if eax<>0
		invoke MessageBoxA,[hwnd],'Ошибка сохранения данных',0,0
	.endif
	invoke GlobalFree,[buff_login]
	invoke GlobalFree,[buff_pass]
     .exit:
	invoke RegCloseKey,[HKey]
	invoke EndDialog,[hwnd],0,0
	ret
endp