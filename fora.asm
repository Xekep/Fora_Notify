format pe gui 4.0
include '%fasm_inc%\WIN32AX.INC'

display 'Компилим fora.inc',13,10

macro invokex proc,[arg]
{
reverse pushd <arg>
common call proc
}

TIMER_ID = 666
IDD_MAIN = 666
IDD_LOGIN = 667
IDD_POPUP = 668
ID_GET = 1
ID_LOGIN = 2
IDL_MONEY = 3
IDL_DATA = 4
IDL_DOGOVOR = 5
IDL_TARIF = 6
ID_AUTORUN = 7
ID_NOTIFY = 8
IDE_NOTIFYMONEY = 9
ID_UPDOWN = 10
ID_OPTIONS = 11
IDL_STAT = 12
ID_ONOFF = 13
ID_UPDATE = 14
ID_HOTKEY = 15
ID_SAVE = 1
IDE_LOGIN = 2
IDE_PASS = 3
ID_TEXT = 1

BUFFER_SIZE = 50000
UDM_SETRANGE32 = 046fh
key_root = HKEY_CURRENT_USER
WM_SHELLNOTIFY equ WM_USER+5
IDI_TRAY equ 0
IDM_RESTORE equ 1000
IDM_START equ 1001
IDM_MAIN equ 1002
IDM_EXIT equ 1003
IDM_SITE equ 1004
IDM_UPDATE equ 1005

.data
;---
coord	RECT	; Размеры окна
screen	RECT	; Размеры экрана
;---
notes NOTIFYICONDATA
trayname db 'FORA NOTIFY',0
hMenu dd ?
hMenu2 dd ?
pt POINT
;---
stat_err_autorun1 db 'Ошибка добавления в автозапуск',0
stat_err_autorun2 db 'Ошибка удаления из автозапуска',0
stat_err_reg db 'Ошибка работы с реестром',0
err_acc db 'Введите данные',0
err_connect1 db 'Не могу подключиться',0
err_connect2 db 'Не могу получить IP адрес',0
err_connect3 db 'Ошибка подключения',0
err_mem1 db 'Не могу выделить память',0
err_connect4 db 'Необходимо указать рабочий аккаунт',0
stat_ok db 'OK',0
fail_teg db 'alert ("*");',0
fail_teg2 db 'alert ("Error WWW-00013: *. Осталось',0
fail_teg3 db 'alert ("Error WWW-00013: *");',0
fail_mess db 'Error WWW-00013: Неверный пароль',0
fail_mess2 db 'Error WWW-00013: Неверный логин',0
fail_mess3 db 'Заблокировано',0
preg1 db '<TD>Остаток средств:<TD>*<TD>',0
preg2 db '<TD>Создан:<TD>*<TD>',0
preg3 db '<TD>Договор:<TD>*<TD>',0
preg4 db '<TD>Тарифный план:<TD>*<TD>',0
host db 'bs.foratec.net',0
qry db 'POST /pls/fastcom25/w3_p_Main.showform HTTP/1.1',13,10
Host db 'Host: bs.foratec.net:7777',13,10
content_len db 'Content-Length: %u',13,10,13,10
post_login db 'P1=USERNAME&V1=',0
post_pass db '&P2=PASSWORD&V2=',0
post_ db '&P3=FORMNAME&V3=IP_LOGIN_INFO%3BFORMNAME%3AIDENTIFICATION%3BSID%3A%3BNLS%3ARUS%3BFIRST%3A%3BFORM_NAME%3AIP_LOGIN_INFO%3B&P20=BUTTON&V20=%C2%F5%EE%E4',0
subkey db 'Software\Microsoft\Windows\CurrentVersion\Run',0
subkey2 db 'Software\fora notify',0
key1 db 'login',0
key2 db 'pass',0
key_name db 'fora notify',0
key_name2 db 'flag1',0
key_name3 db 'flag2',0
key_name4 db 'update',0
key_name5 db 'HOTKEY',0
command db ' -autorun',0
str1 db 'СТАРТ',0
str2 db 'Главное окно',0
str3 db '&Выход',0
str4 db 'Сайт разработчика',0
str5 db 'Проверить обновления',0
site db 'http://www.zone66.su',0
host2 db 'www.soft.zone66.su',0
flagg db 1
buffer rb MAX_PATH+10
vsize dd ?
mhandle dd ?
buff_pass dd ?
buff_login dd ?
buff_trash dd ?
HKey dd ?
lpType dd ?
cons rb 5
notify_flag db 0
notify_flag2 db ?
update_flag db 0
tray_flag db 0
flag db 0
strg dd ?
idhkey dd ?
vkey dd 0
hkey_flag db 0
death dd 1

.code
start:
	invoke GetCommandLineA
	push eax
	invoke lstrlenA,eax
	pop ecx
	sub eax,9
	add eax,ecx
	invoke lstrcmpA,eax,command
	.if eax=0
		mov [tray_flag],1
	.endif
	invoke CreateMutexA,0,0,"fora notify"
	invoke GetLastError
	.if eax=0B7h
		jmp exit
	.endif
	invoke InitCommonControls
	invoke GetModuleHandleA,0
	mov [mhandle],eax
	invoke DialogBoxParamA,eax,IDD_MAIN,HWND_DESKTOP,main,0
       exit:
	invoke ExitProcess,0

proc main hwnd, msg, wparam, lparam
	push ebx edi esi
	cmp [msg],WM_HOTKEY
	je .hotkey
	cmp [msg],WM_INITDIALOG
	je  .wminitdialog
	cmp [msg],WM_SHELLNOTIFY
	je  .wnmax
	cmp [msg],WM_SIZE
	je  .wmsize_tray
	cmp [msg],WM_COMMAND
	je  .wmcommand
	cmp [msg],WM_SYSCOMMAND
	je  .syscommand
	cmp [msg],WM_LBUTTONDOWN
	je  .move
	cmp [msg],WM_CLOSE
	je  .wmclose
	xor eax,eax
	jmp .finish
    .wminitdialog:
	.if [tray_flag]=1
		invoke ShowWindow,[hwnd],SW_HIDE
		invoke PostMessageA,[hwnd],WM_SIZE,SIZE_MINIMIZED,0
	.endif
	invoke CreatePopupMenu
	mov [hMenu],eax
	invoke AppendMenu,[hMenu],MF_STRING,IDM_START,str1
	invoke AppendMenu,[hMenu],MF_STRING,IDM_MAIN,str2
	invoke AppendMenu,[hMenu],MF_STRING,IDM_SITE,str4
	invoke AppendMenu,[hMenu],MF_STRING,IDM_UPDATE,str5
	invoke AppendMenu,[hMenu],MF_SEPARATOR,0,0
	invoke AppendMenu,[hMenu],MF_STRING,IDM_EXIT,str3
	invoke GetSystemMenu,[hwnd],0
	mov [hMenu2],eax
	invoke AppendMenu,[hMenu2],MF_SEPARATOR,0,0
	invoke AppendMenu,[hMenu2],MF_STRING,IDM_SITE,str4
	mov [vsize],MAX_PATH
	invoke GlobalAlloc,GPTR,MAX_PATH+10
	mov [buff_pass],eax
	invoke GetModuleFileNameA,[mhandle],[buff_pass],MAX_PATH+10
	add eax,[buff_pass]
	invoke lstrcpyA,eax,command
	invoke RegOpenKeyEx,key_root,subkey,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke GlobalAlloc,GPTR,MAX_PATH+10
		mov [buff_login],eax
		invoke RegQueryValueExA,[HKey],key_name,0,lpType,[buff_login],vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			invoke lstrcmpA,[buff_login],[buff_pass]
			.if eax=0
				 invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_CLICK,0,0
			.endif
		.else
			invoke RegCloseKey,[HKey]
		.endif
		invoke GlobalFree,[buff_login]
	.endif
	mov [vsize],1
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke RegQueryValueExA,[HKey],key_name2,0,lpType,notify_flag,vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			.if [notify_flag]=1
				invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_CLICK,0,0
			.endif
		.else
			invoke RegCloseKey,[HKey]
			invoke GetDlgItem,[hwnd],ID_ONOFF
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
			invoke EnableWindow,eax,0
		.endif
	.endif
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke RegQueryValueExA,[HKey],key_name3,0,lpType,notify_flag2,vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			.if [notify_flag2]=1
				invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_CLICK,0,0
			.endif
		.else
			invoke RegCloseKey,[HKey]
		.endif
	.endif
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke RegQueryValueExA,[HKey],key_name4,0,lpType,update_flag,vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			.if [update_flag]=1
				invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_CLICK,0,0
				invoke CreateThread,0,0,getversion,[hwnd],0,0
			.endif
		.else
			invoke RegCloseKey,[HKey]
		.endif
	.endif
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
	       invoke RegQueryValueExA,[HKey],key_name5,0,lpType,hkey_flag,vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			.if [hkey_flag]=1
				invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_CLICK,0,0
				mov [idhkey],1
				invoke RegisterHotKey,[hwnd],1,MOD_ALT,4Dh
			.endif
		.else
			invoke RegCloseKey,[HKey]
		.endif
	.endif
	invoke GlobalFree,[buff_pass]
	invoke GetDlgItem,[hwnd],ID_UPDOWN
	invoke SendMessage,eax,UDM_SETRANGE32,0,15000
	invoke CreateFontW,26,0,0,0,1000,0,0,0,DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
	push eax
	invoke GetDlgItem,[hwnd],ID_GET
	pop ecx
	invoke SendMessage,eax,WM_SETFONT,ecx,TRUE
	invoke CreateFontW,20,0,0,0,1000,0,0,0,DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
	push eax
	invoke GetDlgItem,[hwnd],99
	pop ecx
	invoke SendMessage,eax,WM_SETFONT,ecx,TRUE
	invoke CreateFontW,15,0,0,0,400,0,0,0,DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
	push eax
	invoke GetDlgItem,[hwnd],IDL_STAT
	pop ecx
	invoke SendMessage,eax,WM_SETFONT,ecx,TRUE
	invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,0,0,SWP_NOMOVE+SWP_NOSIZE
	invoke SetWindowLongA,[hwnd],GWL_EXSTYLE,WS_EX_LAYERED,0
	invoke SetLayeredWindowAttributes,[hwnd],0,240,LWA_ALPHA
	invoke LoadIcon,[mhandle],17
	invoke SendMessageA,[hwnd],WM_SETICON,1,eax
	invokex setstat,[hwnd],stat_ok
	jmp .processed
    .wmcommand:
	.if [lparam]=0
		mov eax,[wparam]
		.if ax=IDM_RESTORE | ax=IDM_MAIN
			mov [tray_flag],0
			invoke ShowWindow,[hwnd],SW_SHOWDEFAULT
			invoke SetActiveWindow,[hwnd]
		.elseif ax=IDM_START
			mov [flag],1
			jmp .get_info
		.elseif ax=IDM_EXIT
			invoke Shell_NotifyIcon,NIM_DELETE,notes
			jmp .wmclose
		.elseif ax=IDM_SITE
			invoke ShellExecuteA,0,'Open',site,0,0,0
			jmp .processed
		.elseif ax=IDM_UPDATE
			invoke CreateThread,0,0,getversion,[hwnd],0,0
			jmp .processed
		.else
			invoke DestroyWindow,[hwnd]
		.endif
		invoke Shell_NotifyIcon,NIM_DELETE,notes
	.endif
	cmp [wparam],BN_CLICKED shl 16 + ID_GET
	je .get_info
	cmp [wparam],BN_CLICKED shl 16 + ID_LOGIN
	je .get_login
	cmp [wparam],BN_CLICKED shl 16 + ID_OPTIONS
	je .options
	cmp [wparam],BN_CLICKED shl 16 + ID_AUTORUN
	je .autorun
	cmp [wparam],BN_CLICKED shl 16 + ID_NOTIFY
	je .notifys
	cmp [wparam],BN_CLICKED shl 16 + ID_ONOFF
	je .onoff
	cmp [wparam],BN_CLICKED shl 16 + ID_UPDATE
	je .update1
	cmp [wparam],BN_CLICKED shl 16 + ID_HOTKEY
	je .sethotkey
	jmp .processed
    .wmclose:
	.if [vkey]<>0
		invoke UnregisterHotKey,[hwnd],[idhkey]
	.endif
	invoke	EndDialog,[hwnd],0
    .processed:
	mov eax,1
    .finish:
	pop edi esi ebx
	ret
    .move:
	invoke SendMessage,[hwnd],0a1h,2,0
	jmp .processed
.syscommand:
	.if [wparam]=IDM_SITE
		 invoke ShellExecuteA,0,'Open',site,0,0,0
		 jmp .processed
	.endif
	xor eax,eax
	jmp .finish
.hotkey:
	mov eax,[wparam]
	.if eax=[idhkey]
			mov [flag],1
			jmp .get_info
	.endif
	jmp .processed
.wmsize_tray:
	.if [wparam]=SIZE_MINIMIZED
		mov [tray_flag],1
		mov [notes.cbSize],sizeof.NOTIFYICONDATA
		push [hwnd]
		pop [notes.hWnd]
		mov [notes.uID],IDI_TRAY
		mov [notes.uFlags],NIF_ICON+NIF_MESSAGE+NIF_TIP
		mov [notes.uCallbackMessage],WM_SHELLNOTIFY
		invoke LoadIconA,[mhandle],17
		mov [notes.hIcon],eax
		invoke lstrcpy,notes.szTip,trayname
		invoke ShowWindow,[hwnd],SW_HIDE
		invoke Shell_NotifyIcon,NIM_ADD,notes
	.endif
	jmp .processed

.wnmax:
	.if [wparam]=IDI_TRAY
		.if [lparam]=WM_LBUTTONDBLCLK
			mov [tray_flag],0
			invoke SendMessage,[hwnd],WM_COMMAND,IDM_RESTORE,0
		.elseif [lparam]=WM_RBUTTONDOWN
			invoke GetCursorPos,pt
			invoke SetForegroundWindow,[hwnd]
			invoke TrackPopupMenu,[hMenu],TPM_RIGHTALIGN,pt.x,pt.y,NULL,[hwnd],NULL
			invoke PostMessageA,[hwnd],WM_NULL,0,0
		.endif
	.endif
	jmp .processed
.onoff:
	invokex DialogPOPUP,'UNDERCONSTRUCTION'
	invokex setstat,[hwnd],'UNDERCONSTRUCTION'
	invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_CLICK,0,0
	jmp .processed
   .get_info:
	invoke WaitForSingleObject,[death],0
	.if eax=WAIT_TIMEOUT
		jmp .processed
	.endif
	invoke SetDlgItemTextA,[hwnd],IDL_MONEY,0
	invoke SetDlgItemTextA,[hwnd],IDL_DATA,0
	invoke SetDlgItemTextA,[hwnd],IDL_DOGOVOR,0
	invoke SetDlgItemTextA,[hwnd],IDL_TARIF,0
	mov [vsize],64
	invoke RegOpenKeyExA,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax<>0
		invokex DialogPOPUP,err_acc,0
		jmp .get_login
	.endif
	invoke GlobalAlloc,GPTR,64
	mov [buff_login],eax
	invoke RegQueryValueExA,[HKey],key1,0,lpType,[buff_login],vsize
	.if eax<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,0
		invoke GlobalFree,[buff_login]
		jmp .get_login
	.endif
	invoke GlobalAlloc,GPTR,64
	mov [buff_pass],eax
	invoke RegQueryValueExA,[HKey],key2,0,lpType,[buff_pass],vsize
	.if eax<>0
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,0
		invoke GlobalFree,[buff_pass]
		invoke GlobalFree,[buff_login]
		jmp .get_login
	.endif
	invoke RegCloseKey,[HKey]
	invoke GetDlgItem,[hwnd],ID_GET
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_LOGIN
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hMenu],IDM_START
	invoke EnableWindow,eax,0
	invoke EnableMenuItem,[hMenu],IDM_START,MF_GRAYED
	invoke SetDlgItemTextA,[hwnd],ID_GET,"Wait"
	invoke CreateThread,0,0,get_content,[hwnd],0,0
	mov [death],eax
	jmp .processed
   .get_login:
	.if [tray_flag]=0
		invoke ShowWindow,[hwnd],SW_HIDE
	.endif
	invoke DialogBoxParamA,[mhandle],IDD_LOGIN,HWND_DESKTOP,getlogin,0
	.if [tray_flag]=0
		invoke ShowWindow,[hwnd],SW_SHOW
	.endif
	jmp .processed
  .options:
	invoke IsDlgButtonChecked,[hwnd],ID_OPTIONS
	.if eax=BST_CHECKED
		invoke SetWindowPos,[hwnd],-1,0,0,321,386,2
	.else
		invoke SetWindowPos,[hwnd],-1,0,0,321,267,2
	.endif
	jmp .processed
  .autorun:
	invoke IsDlgButtonChecked,[hwnd],ID_AUTORUN
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_autorun1,0
			invokex setstat,[hwnd],stat_err_autorun1
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,0,0
			jmp .processed
		.endif
		invoke GetModuleFileNameA,[mhandle],buffer,MAX_PATH+10
		add eax,buffer
		invoke lstrcpyA,eax,command
		invoke lstrlenA,buffer
		invoke RegSetValueEx,[HKey],key_name,0,REG_SZ,buffer,eax
		.if eax<>0
			invokex DialogPOPUP,stat_err_autorun1,0
			invokex setstat,[hwnd],stat_err_autorun1
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,0,0
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,0,0
			invokex DialogPOPUP,stat_err_autorun2,0
			invokex setstat,[hwnd],stat_err_autorun2
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name
		.if eax<>0
			invokex DialogPOPUP,stat_err_autorun2,0
			invokex setstat,[hwnd],stat_err_autorun2
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,0,0
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
.update1:
	invoke IsDlgButtonChecked,[hwnd],ID_UPDATE
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,0,0
			jmp .processed
		.endif
		 invoke RegSetValueEx,[HKey],key_name4,0,REG_BINARY,flagg,1
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,0,0
		.else
			invokex setstat,[hwnd],stat_ok
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,0,0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name4
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,0,0
		.else
			invokex setstat,[hwnd],stat_ok
		.endif
	.endif
	jmp .processed
.sethotkey:
	invoke IsDlgButtonChecked,[hwnd],ID_HOTKEY
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,0,0
			jmp .processed
		.endif
		 invoke RegSetValueEx,[HKey],key_name5,0,REG_BINARY,flagg,1
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,0,0
		.else
			mov [idhkey],1
			invoke RegisterHotKey,[hwnd],1,MOD_ALT,4Dh
			invokex setstat,[hwnd],stat_ok
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,0,0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name5
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,0,0
		.else
			invoke UnregisterHotKey,[hwnd],[idhkey]
			invokex setstat,[hwnd],stat_ok
		.endif
	.endif
	jmp .processed
.notifys:
	invoke IsDlgButtonChecked,[hwnd],ID_NOTIFY
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,0,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name2,0,REG_BINARY,flagg,1
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,0,0
		.else
			mov [notify_flag],1
			invokex setstat,[hwnd],stat_ok
			invoke GetDlgItem,[hwnd],ID_ONOFF
			invoke EnableWindow,eax,1
			invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
			invoke EnableWindow,eax,1
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,0,0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name2
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,0,0
		.else
			mov [notify_flag],0
			invokex setstat,[hwnd],stat_ok
			invoke GetDlgItem,[hwnd],ID_ONOFF
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
			invoke EnableWindow,eax,0
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
endp

display 'Компилим getversion.inc',13,10

include 'getversion.inc'

display 'Компилим getcontent.inc',13,10

include 'getcontent.inc'

display 'Компилим getlogin.inc',13,10

include 'getlogin.inc'

display 'Компилим parser.inc',13,10

include 'parser.inc'

display 'Компилим stat.inc',13,10

include 'stat.inc'

display 'Компилим popup.inc',13,10

include 'popup.inc'

.end start
section '.res' readable data resource from 'res.res'

display 'Компилирование успешно завершено О_о',13,10