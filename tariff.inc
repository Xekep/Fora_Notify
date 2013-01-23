MAX_FILE_SIZE = 50000
ID_WWW = 20

proc tarifproc hwnd, msg, wparam, lparam
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
	je  .exit
	cmp [msg],WM_SETCURSOR
	je .cursor_over_window
	cmp [msg],WM_CTLCOLORSTATIC
	je .wmctlcolorstatic
	xor eax,eax
	jmp .finish
   .wminitdialog:
	call set_seh
	mov [tprogress],0
	mov [cur],0
	mov [tselect],0
	mov [press],0
	invoke CreateWindowExA,0,'STATIC',site+7,SS_LEFT + SS_NOTIFY + WS_CHILD + WS_GROUP + WS_VISIBLE,100,100,90,14,[hwnd],0,0,0
	push eax
	invoke CreateFontW,14,0,0,0,0,0,0,0,ANSI_CHARSET or DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
	mov [font3],eax
	mov ecx,dword [esp]
	invoke SendMessage,ecx,WM_SETFONT,eax,TRUE
	pop eax
	invoke SetWindowLongA,eax,GWL_ID,ID_WWW,0
	invoke GetDlgItem,[hwnd],ID_WWW
	mov [thHyper],eax
	invoke SetWindowLongA,eax,GWL_WNDPROC,tWindowProc
	mov [tOldWndProc],eax
	invoke GetSysColor,COLOR_BTNFACE
	invoke CreateSolidBrush,eax
	mov [thUrlBrush],eax
	mov [tsscolor],clrMain
	mov eax,[hwnd]
	mov [tOldhwnd],eax
	invoke SetTimer,[hwnd],666,500,0
	invoke SendDlgItemMessage,[hwnd],ID_PROGRESS,PBM_SETRANGE32,0,100
	invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,0,0,SWP_NOMOVE+SWP_NOSIZE
	invoke SetWindowLongA,[hwnd],GWL_EXSTYLE,WS_EX_LAYERED,0
	invoke SetLayeredWindowAttributes,[hwnd],0,240,LWA_ALPHA
	invoke CreateThread,0,0,.process,[hwnd],0,0
	invoke CreateThread,0,0,get_tarif,[hwnd],0,0
	jmp .processed
    .cursor_over_window:
	cmp [tsscolor],clrMain
	je  @f
	mov [tsscolor],clrMain
	invoke InvalidateRect,[thHyper],NULL,TRUE
	jmp .processed
      @@:
	xor eax,eax
	jmp .finish
    .wmctlcolorstatic:
	invoke GetDlgCtrlID,[lparam]
	mov ecx,[tselect]
	add ecx,100
	.if eax=ecx
		invoke SetBkMode,[wparam],TRANSPARENT
		invoke SetTextColor,[wparam],255
	.endif
	xor eax,eax
	mov ecx,[lparam]
	cmp ecx,[thHyper]
	jne @f
	cmp [tsscolor],clrMain
	mov eax,[tsscolor]
	invoke SetTextColor,[wparam],eax
       @@:
	invoke SetBkMode,[wparam],TRANSPARENT
	mov eax,[thUrlBrush]
	jmp .finish
   .wmcommand:
	mov ecx,[wparam]
	sub ecx,BN_CLICKED shl 16
	.if ecx>100
		mov eax,[cur]
		add eax,100
		.if eax<>ecx & [press]<>ecx
			mov [press],ecx
			invoke GetDlgItem,[hwnd],ID_TSAVE
			invoke EnableWindow,eax,1
		.elseif [press]<>ecx
			mov [press],ecx
			invoke GetDlgItem,[hwnd],ID_TSAVE
			invoke EnableWindow,eax,0
		.endif
	.endif
	cmp [wparam],BN_CLICKED shl 16 + ID_TSAVE
	jne .processed
	invoke CreateThread,0,0,save_tarif,[hwnd],0,0
	jmp .processed
    .exit:
	invoke GlobalFree,[tarifi]
	invoke DeleteObject,[font1]
	invoke DeleteObject,[font2]
	invoke DeleteObject,[font3]
    .wmclose:
	invoke EndDialog,[hwnd],0
	ret
    .processed:
	mov eax,1
    .finish:
	pop edi esi ebx
	ret
    .move:
	.if [tprogress]=100
		invoke SendMessage,[hwnd],0a1h,2,0
	.endif
	jmp .processed
     .err_mem:
	invokex DialogPOPUP,err_mem1,1
	jmp .wmclose
     .timer:
	.if [wparam]=666 & [tprogress]=100
		invoke KillTimer,[hwnd],666
		invoke CreateFontW,16,0,0,0,0,0,0,0,ANSI_CHARSET or DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
		mov [font1],eax
		invoke CreateFontW,16,0,0,0,800,0,0,0,ANSI_CHARSET or DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
		mov [font2],eax
		invoke GlobalAlloc,GPTR,100
		mov [tmem],eax
		mov ecx,[count]
		mov eax,100
		mov esi,110
		xor edi,edi
	     .create_loop:
		inc eax
		push edi
		push ecx
		push esi
		push eax
		xor esi,esi
		mov eax,[tarifi]
	      .c_meetka1:
		.if esi<>edi
		  .c_meetka2:
			inc eax
			cmp word [eax],0
			jne .c_meetka2
			inc esi
			add eax,2
			jmp .c_meetka1
		.else
		   .c_meetka3:
			inc eax
			cmp byte [eax],0
			jne .c_meetka3
			inc eax
			mov ecx,eax
		   .c_meetka4:
			inc eax
			cmp byte [eax],0
			jne .c_meetka4
			inc eax
			mov esi,eax
		.endif
		invokex ConcatStrings,[tmem],4,ecx,' - Ежемесячный платёж: ',esi,' руб.'
		add eax,esi
		mov byte [eax],0
		pop eax
		pop esi
		push esi
		push eax
		invokex .createwin,[hwnd],eax,[font1],[tmem],16,esi,375,30
		mov eax,[tmem]
		mov byte [eax],0
		pop eax
		pop esi
		pop ecx
		pop edi
		inc edi
		add esi,30
		.if ecx=1
			dec ecx
		.elseif ecx>1
			dec ecx
			jmp .create_loop
		.endif
		invoke GlobalFree,[tmem]
		mov eax,[cur]
		add eax,100
		push eax
		invoke SendDlgItemMessage,[hwnd],eax,BM_CLICK,0,0
		pop eax
		invoke SendDlgItemMessage,[hwnd],eax,WM_SETFONT,[font2],TRUE
		.if [tselect]<>0
			mov eax,[tselect]
			add eax,100
			invoke SendDlgItemMessage,[hwnd],eax,WM_SETFONT,[font2],TRUE
		.endif
	.elseif [tprogress]=-1
		jmp .wmclose
	.endif
	jmp .processed
endp

proc .process hwnd
	call set_seh
	local w_right:DWORD
	local w_bottom:DWORD
	local gb_right:DWORD
	local gb_bottom:DWORD
	invoke GetWindowRect,[hwnd],tcoord
	mov eax,[tcoord.right]
	mov [w_right],eax
	mov eax,[tcoord.bottom]
	mov [w_bottom],eax
	mov eax,[tcoord.top]
	sub [w_bottom],eax
	mov eax,[tcoord.left]
	sub [w_right],eax
	invoke GetWindowLongA,[hwnd],GWL_STYLE
	and eax,not WS_CAPTION or WS_SYSMENU
	invoke SetWindowLongA,[hwnd],GWL_STYLE,eax,0
	invoke ShowWindow,[hwnd],SW_SHOW
	mov eax,[w_right]
	sub eax,5
	invoke SetWindowPos,[hwnd],-1,0,0,eax,80,SWP_NOMOVE
	invoke GetDlgItem,[hwnd],ID_NAME
	invoke ShowWindow,eax,SW_HIDE
	invoke GetDlgItem,[hwnd],ID_CURTARIF
	invoke ShowWindow,eax,SW_HIDE
	invoke GetDlgItem,[hwnd],ID_MONEY
	invoke ShowWindow,eax,SW_HIDE
	invoke GetDlgItem,[hwnd],ID_1
	invoke ShowWindow,eax,SW_HIDE
	invoke GetDlgItem,[hwnd],ID_2
	invoke ShowWindow,eax,SW_HIDE
	invoke GetDlgItem,[hwnd],ID_3
	invoke ShowWindow,eax,SW_HIDE
	invoke GetDlgItem,[hwnd],ID_TSAVE
	invoke ShowWindow,eax,SW_HIDE
	invoke GetDlgItem,[hwnd],ID_WWW
	invoke ShowWindow,eax,SW_HIDE
	invoke GetDlgItem,[hwnd],ID_4
	push eax
	invoke GetClientRect,eax,tcoord
	mov eax,[tcoord.right]
	mov [gb_right],eax
	mov eax,[tcoord.bottom]
	mov [gb_bottom],eax
	pop eax
	invoke SetWindowPos,eax,HWND_TOP,11,10,0,0,SWP_NOSIZE
	invoke GetDlgItem,[hwnd],ID_PROGRESS
	invoke SetWindowPos,eax,HWND_TOP,20,30,0,0,SWP_NOSIZE
      .metka1:
	mov eax,[tprogress]
	invoke SendDlgItemMessage,[hwnd],ID_PROGRESS,PBM_SETPOS,eax,0
	.if [tprogress]=40
		invoke GetDlgItem,[hwnd],ID_4
		invoke SetWindowPos,eax,HWND_TOP,11,90,0,0,SWP_NOSIZE
		invoke GetDlgItem,[hwnd],ID_PROGRESS
		invoke SetWindowPos,eax,HWND_TOP,20,110,0,0,SWP_NOSIZE
		mov eax,[w_right]
		sub eax,5
		invoke SetWindowPos,[hwnd],-1,0,0,eax,160,SWP_NOMOVE
		invoke GetDlgItem,[hwnd],ID_NAME
		invoke ShowWindow,eax,SW_SHOW
		invoke GetDlgItem,[hwnd],ID_CURTARIF
		invoke ShowWindow,eax,SW_SHOW
		invoke GetDlgItem,[hwnd],ID_MONEY
		invoke ShowWindow,eax,SW_SHOW
		invoke GetDlgItem,[hwnd],ID_1
		invoke ShowWindow,eax,SW_SHOW
		invoke GetDlgItem,[hwnd],ID_2
		invoke ShowWindow,eax,SW_SHOW
		invoke GetDlgItem,[hwnd],ID_3
		invoke ShowWindow,eax,SW_SHOW
		jmp .metka1
	.elseif [tprogress]=100
		invoke GetDlgItem,[hwnd],ID_TSAVE
		push eax
		mov eax,[count]
		mov ebx,30
		mul ebx
		add eax,125
		mov ecx,dword [esp]
		invoke SetWindowPos,ecx,HWND_TOP,145,eax,0,0,SWP_NOSIZE
		mov eax,dword [esp]
		invoke ShowWindow,eax,SW_SHOW
		pop eax
		invoke EnableWindow,eax,0
		invoke GetDlgItem,[hwnd],ID_WWW
		push eax
		mov eax,[count]
		mov ebx,30
		mul ebx
		add eax,135
		mov ecx,dword [esp]
		invoke SetWindowPos,ecx,HWND_TOP,280,eax,0,0,SWP_NOSIZE
		pop eax
		invoke ShowWindow,eax,SW_SHOW
		invoke SetWindowPos,[hwnd],-1,0,0,[w_right],220,SWP_NOMOVE
		invoke GetDlgItem,[hwnd],ID_PROGRESS
		invoke ShowWindow,eax,SW_HIDE
		invoke SetDlgItemTextA,[hwnd],ID_4,'Доступные тарифы'
		invoke SetWindowLongA,[hwnd],GWL_STYLE,WS_SYSMENU+WS_CAPTION,0
		invoke LoadIcon,[mhandle],ID_ICON
		invoke SendMessageA,[hwnd],WM_SETICON,ICON_BIG,eax
		invoke ShowWindow,[hwnd],SW_SHOW
		mov eax,[count]
		dec eax
		mov ebx,30
		mul ebx
		add eax,30
		add [w_bottom],eax
		sub eax,30
		add [gb_bottom],eax
		invoke SetWindowPos,[hwnd],-1,0,0,[w_right],[w_bottom],SWP_NOMOVE
		invoke GetDlgItem,[hwnd],ID_4
		invoke SetWindowPos,eax,HWND_TOP,0,0,[gb_right],[gb_bottom],SWP_NOMOVE
		invokex SetWindowCenter,[hwnd]
		invoke ExitThread,0
	.elseif [tprogress]=-1
		invoke ExitThread,0
	.else
		invoke Sleep,100
		jmp .metka1
	.endif
endp

proc .createwin hwnd, id,font, text, x, y, w, h
	invoke CreateWindowExA,0,'BUTTON',[text],BS_CENTER + WS_CHILD + WS_VISIBLE + WS_TABSTOP + BS_FLAT + BS_AUTORADIOBUTTON,[x],[y],[w],[h],[hwnd],0,0,0
	push eax
	invoke SendMessage,eax,WM_SETFONT,[font],TRUE
	pop eax
	invoke SetWindowLongA,eax,GWL_ID,[id],0
	ret
endp

proc get_tarif hwnd
	call set_seh
	local buff_login:DWORD
	local buff_pass:DWORD
	local buff:DWORD
	local post:DWORD
	local mem:DWORD
	local mem2:DWORD
	local curtarif:DWORD
	local selecttarif:DWORD
	mov [vsize],64
	invoke RegOpenKeyExA,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax<>0
		invokex DialogPOPUP,err_acc,1
		mov [tprogress],-1
		jmp .exit
	.endif
	invoke GlobalAlloc,GPTR,64
	mov dword [buff_login],eax
	invoke RegQueryValueExA,[HKey],key1,0,lpType,dword [buff_login],vsize
	.if eax<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,1
		invoke GlobalFree,dword [buff_login]
		mov [tprogress],-1
		jmp .exit
	.endif
	mov ecx,[vsize]
	mov eax,dword [buff_login]
	.if byte [eax]<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,1
		invoke GlobalFree,dword [buff_login]
		mov [tprogress],-1
		jmp .exit
	.endif
      .metka1:
	xor byte [eax],66h
	inc eax
	loop .metka1
	mov [vsize],64
	invoke GlobalAlloc,GPTR,64
	mov dword [buff_pass],eax
	invoke RegQueryValueExA,[HKey],key2,0,lpType,dword [buff_pass],vsize
	.if eax<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,1
		invoke GlobalFree,dword [buff_pass]
		invoke GlobalFree,dword [buff_login]
		mov [tprogress],-1
		jmp .exit
	.endif
	mov ecx,[vsize]
	dec eax
	mov eax,dword [buff_pass]
      .metka2:
	xor byte [eax],66h
	inc eax
	loop .metka2
	invoke RegCloseKey,[HKey]
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE
	.if eax=0
		jmp .err_mem
	.endif
	mov [post],eax
	invokex ConcatStrings,[post],4,'password=',[buff_pass],'&login=',[buff_login]
	add eax,esi
	invoke GlobalFree,[buff_pass]
	invoke GlobalFree,[buff_login]
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE
	test eax,eax
	je .err_mem
	mov [buff],eax
	invokex request,[buff],thost,'/session',tuseragent,'POST',[post],MAX_FILE_SIZE,1
	.if eax=0
		jmp .err_connect
	.endif
	add [tprogress],10
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE*2
	mov [mem],eax
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE*2
	mov [mem2],eax
	invoke MultiByteToWideChar,CP_UTF8,0,[buff],-1,[mem],MAX_FILE_SIZE*2
	invoke WideCharToMultiByte,1251,0,[mem],-1,[mem2],MAX_FILE_SIZE*2,0,0
	invoke GlobalFree,[mem]
	invokex preg_match,tpreg_message,[mem2],.message_hello,0,0,-1
	.if eax<>1
		invokex preg_match,tpreg_message,[mem2],[post],1,64,0
		.if eax<>0
			invokex DialogPOPUP,[post],1
			invoke Sleep,2000
		.endif
		invoke GlobalFree,[mem2]
		jmp .err_connect
	.else
		invokex preg_match,.preg_name,[mem2],[post],1,64,0
		.if eax=0
			invoke GlobalFree,[mem2]
			jmp .err_connect
		.else
			invoke SetDlgItemTextA,[hwnd],ID_NAME,[post]
			add [tprogress],10
		.endif
		invoke GlobalAlloc,GPTR,100
		test eax,eax
		je .err_mem
		mov [curtarif],eax
		invokex preg_match,.preg_curtarif,[mem2],[curtarif],1,64,0
		.if eax=0
			invoke GlobalFree,[mem2]
			invoke GlobalFree,[curtarif]
			jmp .err_connect
		.else
			invoke SetDlgItemTextA,[hwnd],ID_CURTARIF,[curtarif]
			add [tprogress],10
		.endif
		invokex preg_match,.preg_money,[mem2],[post],1,64,0
		.if eax=0
			invoke GlobalFree,[mem2]
			invoke GlobalFree,[curtarif]
			jmp .err_connect
		.else
			invoke lstrcatA,[post],'руб.'
			invoke SetDlgItemTextA,[hwnd],ID_MONEY,[post]
			add [tprogress],10
		.endif
	.endif
	invoke GlobalFree,[mem2]
	invoke GlobalFree,[buff]
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE
	test eax,eax
	je .err_mem
	mov [buff],eax
	invokex request,[buff],thost,'/tariff/new',tuseragent,'GET',0,MAX_FILE_SIZE,1
	.if eax=0
		invoke GlobalFree,[curtarif]
		jmp .err_connect
	.endif
	;
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE*2
	mov [mem],eax
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE*2
	mov [mem2],eax
	invoke MultiByteToWideChar,CP_UTF8,0,[buff],-1,[mem],MAX_FILE_SIZE*2
	invoke WideCharToMultiByte,1251,0,[mem],-1,[mem2],MAX_FILE_SIZE*2,0,0
	invoke GlobalFree,[mem]
	invokex preg_match,.preg_tarif,[mem2],[post],2,MAX_FILE_SIZE,0
	.if eax=0
		invoke GlobalFree,[mem2]
		invoke GlobalFree,[curtarif]
		jmp .err_connect
	.endif
	invoke GlobalAlloc,GPTR,100
	test eax,eax
	je .err_mem
	mov [selecttarif],eax
	invokex preg_match,.preg_selecttarif,[mem2],[selecttarif],1,64,0
	.if eax=0
		invoke GlobalFree,[mem2]
		invoke GlobalFree,[curtarif]
		invoke GlobalFree,[selecttarif]
		jmp .err_connect
	.endif
	mov eax,[post]
	mov [tarifi],eax
	xor ecx,ecx
	xor ebx,ebx
	mov esi,1;
      .metka_count:
	 ;
	.if esi=1 & ebx=0
		pushad
	  .metka_count!:
		inc eax
		cmp byte [eax],0
		jne .metka_count!
		inc eax
		invoke lstrcmpA,eax,[curtarif]
		.if eax=0
			popad
			pushad
			inc ecx
			mov [cur],ecx
			popad
			mov ebx,1
		.else
			popad
			xor esi,esi
		.endif
	.endif
	;
	inc eax
	cmp word [eax],0
	jne .metka_count
	mov esi,1;
	inc ecx
	add eax,2
	cmp word [eax],0
	jne .metka_count
	mov [count],ecx
	add [tprogress],60
	invoke lstrcmpA,[selecttarif],[curtarif]
	.if eax=0
		mov edi,1
	.else
		mov edi,0
	.endif;
	mov [tselect],0
	mov eax,[post]
	xor ecx,ecx
  .metka_count2:
	.if edi=0
		pushad
	  .metka_count!2:
		inc eax
		cmp byte [eax],0
		jne .metka_count!2
		inc eax
		invoke lstrcmpA,eax,[selecttarif]
		.if eax=0
			popad
			mov edi,1
			pushad
			inc ecx
			mov [tselect],ecx
		.endif
		popad
	  .metka_count3:
		inc eax
		cmp word [eax],0
		jne .metka_count3
		add eax,2
		inc ecx
		jmp .metka_count2
	.endif
	invoke GlobalFree,[curtarif]
	invoke GlobalFree,[selecttarif]
	invoke GlobalFree,[mem2]
	invoke GlobalFree,[buff]
	invoke ExitThread,0
     .exit:
	invoke ExitThread,0
 .err_login:
	invokex DialogPOPUP,err_connect4,1
	mov [tprogress],-1
	jmp .exit
  .err_connect:
	invokex DialogPOPUP,err_connect3,1
	invoke GlobalFree,[buff]
	invoke GlobalFree,[post]
	mov [tprogress],-1
	jmp .exit
     .err_mem:
	invokex DialogPOPUP,err_mem1,1
	mov [tprogress],-1
	jmp .exit

.message_hello db 'Добро пожаловать!',0
.preg_tarif db 'name="tariff_code" type="radio" value="*" /></td>'
	    db 13,10,'            <td style="border-left:none;">'
	    db 13,10,'              *<br>'
	    db 13,10,'              <span style="font-size: xx-small;">Ежемесячный платеж: <b>*</b> руб.</span>',0
.preg_name db 'ФИО абонента</td><td>*</td>',0
.preg_curtarif db 'Тариф</td><td>*</td>',0
.preg_money db 'Остаток: <b>*RUR</b>',0
.preg_selecttarif db "<tr class='selected' >",10,"      <td>*</td>",0

endp
proc save_tarif hwnd
	call set_seh
	local post:DWORD
	local buff:DWORD
	local mem:DWORD
	local mem2:DWORD
	local buff_login:DWORD
	local buff_pass:DWORD
	local token:DWORD
	invoke GetDlgItem,[hwnd],ID_TSAVE
	invoke EnableWindow,eax,0
	mov ecx,[count]
	mov eax,100
    .disable:
	inc eax
	push ecx
	push eax
	invoke GetDlgItem,[hwnd],eax
	invoke EnableWindow,eax,0
	pop eax
	pop ecx
	loop .disable
	invoke GlobalAlloc,GPTR,100
	test eax,eax
	je .err_mem
	mov [token],eax
	mov [vsize],64
	invoke RegOpenKeyExA,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax<>0
		invokex DialogPOPUP,err_acc,1
		jmp .exit
	.endif
	invoke GlobalAlloc,GPTR,64
	mov dword [buff_login],eax
	invoke RegQueryValueExA,[HKey],key1,0,lpType,dword [buff_login],vsize
	.if eax<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,1
		invoke GlobalFree,dword [buff_login]
		jmp .exit
	.endif
	mov ecx,[vsize]
	mov eax,dword [buff_login]
	.if byte [eax]<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,1
		invoke GlobalFree,dword [buff_login]
		jmp .exit
	.endif
      .metka66:
	xor byte [eax],66h
	inc eax
	loop .metka66
	mov [vsize],64
	invoke GlobalAlloc,GPTR,64
	mov dword [buff_pass],eax
	invoke RegQueryValueExA,[HKey],key2,0,lpType,dword [buff_pass],vsize
	.if eax<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,1
		invoke GlobalFree,dword [buff_pass]
		invoke GlobalFree,dword [buff_login]
		jmp .exit
	.endif
	mov ecx,[vsize]
	dec eax
	mov eax,dword [buff_pass]
      .metka67:
	xor byte [eax],66h
	inc eax
	loop .metka67
	invoke RegCloseKey,[HKey]
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE;
	test eax,eax
	je .err_mem
	mov [buff],eax;
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE
	.if eax=0
		invoke GlobalFree,[buff];
		invoke GlobalFree,dword [buff_pass]
		invoke GlobalFree,dword [buff_login]
		jmp .err_mem
	.endif
	mov [post],eax
	invokex request,[buff],thost,'/session/new',tuseragent,'GET',0,MAX_FILE_SIZE,1;
	.if eax=0
		invoke GlobalFree,dword [buff_pass]
		invoke GlobalFree,dword [buff_login]
		jmp .err_connect
	.endif
	invokex preg_match,tpreg_token,[buff],[token],1,100,-1
	.if eax=0
		invoke GlobalFree,[buff_pass]
		invoke GlobalFree,[buff_login]
		jmp .err_connect
	.endif;
	invokex ConcatStrings,[post],6,'password=',[buff_pass],'&login=',[buff_login],ttoken_name,[token];4
	invoke GlobalFree,[buff];
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE
	test eax,eax
	je .err_mem
	mov [buff],eax
	invokex request,[buff],thost,'/session',tuseragent,'POST',[post],MAX_FILE_SIZE,1
	.if eax=0
		jmp .err_connect
	.endif
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE*2
	mov [mem],eax
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE*2
	mov [mem2],eax
	invoke MultiByteToWideChar,CP_UTF8,0,[buff],-1,[mem],MAX_FILE_SIZE*2
	invoke WideCharToMultiByte,1251,0,[mem],-1,[mem2],MAX_FILE_SIZE*2,0,0
	invoke GlobalFree,[mem]
	invokex preg_match,tpreg_message,[mem2],'Добро пожаловать!',0,0,-1
	.if eax<>1
		invokex preg_match,tpreg_message,[mem2],[post],1,64,0
		.if eax<>0
			invokex DialogPOPUP,[post],1
			invoke Sleep,2000
		.endif
		invoke GlobalFree,[mem2]
		jmp .err_connect
	.endif
	invoke GlobalFree,[mem2]
	invoke GlobalFree,[buff]
	mov eax,[post]
	mov dword [eax],0
	mov eax,[tarifi]
	mov ecx,[press]
	sub ecx,100
	mov esi,1
     .metka2:
	.if esi<>ecx
	     .metka1:
		inc eax
		cmp word [eax],0
		jne .metka1
		inc esi
		add eax,2
		jmp .metka2
	.endif
	invokex ConcatStrings,[post],4,'tariff_code=',eax,ttoken_name,[token]
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE
	test eax,eax
	je .err_mem
	mov [buff],eax
	invokex request,[buff],thost,'/tariff',tuseragent,'POST',[post],MAX_FILE_SIZE,1
	.if eax=0
		jmp .err_connect
	.endif
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE*2
	mov [mem],eax
	invoke GlobalAlloc,GPTR,MAX_FILE_SIZE*2
	mov [mem2],eax
	invoke MultiByteToWideChar,CP_UTF8,0,[buff],-1,[mem],MAX_FILE_SIZE*2
	invoke WideCharToMultiByte,1251,0,[mem],-1,[mem2],MAX_FILE_SIZE*2,0,0
	invoke GlobalFree,[mem]
	invokex preg_match,tpreg_message2,[mem2],'Выбранный тарифный ',0,0,-1
	.if eax=1
		invokex DialogPOPUP,'Тариф успешно изменён!',1
		mov eax,[tselect]
		push eax
		mov eax,[press]
		mov [tselect],eax
		sub [tselect],100
		invoke SendDlgItemMessage,[hwnd],eax,WM_SETFONT,[font2],TRUE
		pop eax
		.if eax<>0
			add eax,100
			invoke SendDlgItemMessage,[hwnd],eax,WM_SETFONT,[font1],TRUE
		.endif
	.else
		invoke GetDlgItem,[hwnd],ID_TSAVE
		invoke EnableWindow,eax,1
		invokex preg_match,tpreg_message,[mem2],[post],1,100,-1
		.if eax<>0
			invokex DialogPOPUP,[post],1
		.else
			invokex DialogPOPUP,'Неизвестная ошибка!',1
		.endif
	.endif
	invoke GlobalFree,[mem2]
	invoke GlobalFree,[buff]
	invoke GlobalFree,[post]
     .exit:
	mov ecx,[count]
	mov eax,100
    .enable:
	inc eax
	push ecx
	push eax
	invoke GetDlgItem,[hwnd],eax
	invoke EnableWindow,eax,1
	pop eax
	pop ecx
	loop .enable
	invoke GlobalFree,[token]
	invoke ExitThread,0
 .err_login:
	invokex DialogPOPUP,err_connect4,1
	invoke GlobalFree,[buff]
	invoke GlobalFree,[post]
	jmp .exit
  .err_connect:
	invokex DialogPOPUP,err_connect3,1
	invoke GlobalFree,[buff]
	invoke GlobalFree,[post]
	jmp .exit
     .err_mem:
	invokex DialogPOPUP,err_mem1,1
	invoke GlobalFree,[buff]
	invoke GlobalFree,[post]
	jmp .exit
endp

tuseragent db 'FORA Notify',0
thost db 'stat.fora.su',0
ttoken_name db '&authenticity_token=',0
tpreg_token db 'authenticity_token" type="hidden" value="*"',0
tpreg_message db '<span class="message">*</span>',0
tpreg_message2 db '<span class="message">*план будет назначен автоматически',0

proc tWindowProc hwnd, msg, wparam, lparam
	cmp [msg],WM_SETCURSOR	    ; Курсор над ссылкой
	je .cursor_over_hyperlink
	cmp [msg],WM_LBUTTONDOWN    ; Нажатие мышкой на ссылке
	je .open_link
	invoke CallWindowProc,[tOldWndProc],[hwnd],[msg],[wparam],[lparam]
	ret
.cursor_over_hyperlink:
	mov eax,[thHyper]
	.if [hwnd]<>eax
		jmp .finish
	.endif
	invoke LoadCursor,0,IDC_HAND
	invoke SetCursor,eax
	mov [tsscolor],clrActive
	invoke InvalidateRect,[thHyper],NULL,TRUE
	jmp .finish
.open_link:
	mov eax,[thHyper]
	.if [hwnd]=eax
		invoke ShellExecute,[hwnd],'open',site,0,0,SW_SHOW
	.else
		invoke ShowWindow,[tOldhwnd],SW_HIDE
		invoke DialogBoxParamA,[mhandle],IDD_ABOUT,HWND_DESKTOP,aboutproc,0
		invoke ShowWindow,[tOldhwnd],SW_SHOW
	.endif
.finish:
	xor eax,eax
	ret
endp