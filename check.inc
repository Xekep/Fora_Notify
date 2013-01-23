proc check hwnd
	call set_seh
     .metka:
	invoke WaitForSingleObject,[death],0
	.if eax=WAIT_TIMEOUT
		invoke Sleep,1
		jmp .metka
	.endif
	invoke SetDlgItemTextA,[hwnd],IDL_MONEY,0
	invoke SetDlgItemTextA,[hwnd],IDL_DATA,0
	invoke SetDlgItemTextA,[hwnd],IDL_DOGOVOR,0
	invoke SetDlgItemTextA,[hwnd],IDL_TARIF,0
	mov [vsize],64
	invoke RegOpenKeyExA,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax<>0
		invokex DialogPOPUP,err_acc,0
		jmp .exit
	.endif
	invoke GlobalAlloc,GPTR,64
	mov [buff_login],eax
	invoke RegQueryValueExA,[HKey],key1,0,lpType,[buff_login],vsize
	.if eax<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,0
		invoke GlobalFree,[buff_login]
		jmp .exit
	.endif
	mov ecx,[vsize]
	mov eax,[buff_login]
      .metka1:
	xor byte [eax],66h
	inc eax
	loop .metka1
	mov [vsize],64
	invoke GlobalAlloc,GPTR,64
	mov [buff_pass],eax
	invoke RegQueryValueExA,[HKey],key2,0,lpType,[buff_pass],vsize
	.if eax<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,0
		invoke GlobalFree,[buff_pass]
		invoke GlobalFree,[buff_login]
		jmp .exit
	.endif
	mov ecx,[vsize]
	dec eax
	mov eax,[buff_pass]
      .metka2:
	xor byte [eax],66h
	inc eax
	loop .metka2
	invoke RegCloseKey,[HKey]
	invoke GetDlgItem,[hwnd],ID_GET
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_LOGIN
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_TARIF
	invoke EnableWindow,eax,0
	invoke EnableMenuItem,[hMenu],IDM_START,MF_GRAYED
	invoke EnableMenuItem,[hMenu],IDM_TARIF,MF_GRAYED
	invoke SetDlgItemTextA,[hwnd],ID_GET,"Wait"
	invoke Sleep,2000
	mov [flag2],1
	invoke CreateThread,0,0,get_content,[hwnd],0,0
	mov [death],eax
     .exit:
	invoke ExitThread,0
endp