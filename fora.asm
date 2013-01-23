format pe gui 4.0
include '%fasm_inc%\WIN32AX.INC'
include 'fmod\#ufmod.inc'

display 'Компилим fora.inc',13,10

macro invokex proc,[arg]
{
reverse pushd <arg>
common call proc
}

clrMain 	=     0FF0000h	; Обычный цвет ссылки (синий)
clrActive	=     00000FFh	; Цвет активной ссылки (красный)

TIMER_ID = 667
TIME_POPUP = 4000
TIME_POPUP2 = 20000
TIMER_CHECK = 54
TIME_CHECK = 10*60*1000
TIMER_UPDATE = 666
TIME_UPDATE = 60*60*1000
TIMER_SNOW = 668
TIME_SNOW = 40*1000
TIMER_POST = 669
TIME_POST = 1000

ID_LOGO = 100
ID_LOGO2 = 101

IDD_MAIN = 666
IDD_LOGIN = 667
IDD_POPUP = 668
IDD_TARIF = 669
IDD_LICENSE = 670
IDD_POST = 671
ID_GET = 1
ID_LOGIN = 19
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
ID_APPLY = 16
ID_SOUND = 17
ID_TARIF = 18
IDL_LOGO = 200
IDL_WWW = 201
ID_SAVE = 1
IDE_LOGIN = 2
IDE_PASS = 3
ID_TEXT = 1
ID_ICON = 17
ID_NAME = 1
ID_CURTARIF = 2
ID_MONEY = 3
ID_PROGRESS = 666
ID_1 = 4
ID_2 = 5
ID_3 = 6
ID_4 = 7
ID_TSAVE = 10

IDK_MONEY = 1

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
IDM_ABOUT equ 1006
IDM_TARIF equ 1007
IDM_LICENSE equ 1008
IDM_POST equ 1009

.data

NIIF_NONE = 0
NIIF_INFO = 1
NIIF_WARNING = 2
NIIF_ERROR = 3

struct _NOTIFYICONDATA ;SHELL v5
  cbSize	   dd ?
  hWnd		   dd ?
  uID		   dd ?
  uFlags	   dd ?
  uCallbackMessage dd ?
  hIcon 	   dd ?
  szTip 	   TCHAR 128 dup (?)
  dwState	   dd ?
  dwStateMask	   dd ?
  szInfo	   TCHAR 256 dup (?)
  uTimeout	   dd ?
  szInfoTitle	   TCHAR 64 dup (?)
  dwInfoFlags	   dd ?
ends

notes _NOTIFYICONDATA
notes2 _NOTIFYICONDATA
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
stat_err_money db 'Необходимо ввести сумму',0
stat_ok db 'OK',0
fail_teg db 'alert ("*");',0
fail_teg2 db 'alert ("Error WWW-00013: *. Осталось',0
fail_teg3 db 'alert ("Error WWW-00013: *");',0
fail_mess db 'Error WWW-00013: Неверный пароль',0
fail_mess2 db 'Error WWW-00013: Неверный логин',0
fail_mess3 db 'Заблокировано',0
fail_connect db 'FORATEC: Обслуживание приостановлено',0
fail_X db 'Неизвестная ошибка!',0
preg1 db '<TD>Остаток средств:<TD>*<TD>',0
preg2 db '<TD>Создан:<TD>*<TD>',0
preg3 db '<TD>Договор:<TD>*<TD>',0
preg4 db '<TD>Тарифный план:<TD>*<TD>',0
preg6 db '<TD>Остаток средств:<TD>*.',0
preg7 db '<TITLE>*</TITLE>',0
money1 db 'На вашем счёте кончаются средства',0
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
key_name6 db 'flag3',0
key_name7 db 'license',0
command db ' -autorun',0
str1 db 'СТАРТ',0
str2 db 'Главное окно',0
str3 db '&Выход',0
str4 db 'Сайт разработчика',0
str5 db 'Проверить обновление',0
str6 db 'О программе...',0
str7 db 'Сменить тариф',0
str8 db 'Лицензия',0
str9 db 'Отправить отзыв',0
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
notify_flag2 rb 10
update_flag db 0
tray_flag db 0
flag db 0
flag2 db 0
firstrun db 0
hkey_flag db 0
death dd 1
sound_flag db 0
pOldProc2 dd ?
hMBHook dd ?
baton dd ?
sec dd ?
flag_sec db 0
music_flag db 0
;--- TARIF
tprogress dd ?
tcoord RECT
tscoord RECT
tarifi dd ?
count dd ?
press dd ?
tmem dd ?
cur dd ?
tselect dd ?
font1 dd ?
font2 dd ?
font3 dd ?
font4 dd ?
font5 dd ?
font6 dd ?
hmutex dd ?
p SYSTEMTIME
day db 31,-1,31,30,31,30,31,31,30,31,30,31,0
lOldWndProc dd ?
lhwnd dd ?
OldWndProc2 dd ?
;---LOGO BITMAP
hBitmap dd ?
ps PAINTSTRUCT
rect RECT
hdc dd ?
hMemDC dd ?
nbitmap dd 0
snow_mutex dd 0
;--- Отзывы
post_time dd 0
post_count dd 0
post_button rb 5
time dd ?

tOldWndProc dd ?
thUrlBrush  dd ?
tsscolor dd ?
thHyper dd ?
tOldhwnd dd ?

;--- ABOUT
text db 'FORA Notify 1.0 by Xekep'
;db 0ah,0ah,'Данная программа написана',0Ah,'для чуть большей юзабельности',0Ah,'говённого Новоуральского',0Ah,'интернета от ФОРАТЕК.'
db 0ah,0ah,'Данная программа предназначена',0Ah,'для мониторинга состояния лицевого',0Ah,'счёта оператора форатек.'
db 0ah,0ah,'Программа распространяется',0Ah,'по лицензии Donationware:'
db 0ah,'Если программа окажется для вас',0Ah,'полезной, то вы можете сделать',0Ah,'небольшое пожертвование',0Ah,'(ЯДеньги 41001430156154).'
db 0ah,0ah,'Особую благодарность, за неоценимую',0Ah,'помощь  в тестировании программы,',0Ah,'хочу выразить X ColdDeath X',27h,'у и Mallary.'
db 0Ah,0Ah,'Предложения и пожелания',0Ah,'пишите на soft.zone66.su.'
db 0ah,0ah,'I wish you a good day and fun :)',0

perem1 dd ?
perem2 dd ?
perem3 dd ?
perem4 db ?
perem5 dd ?
perem6 rb 2000
perem7 rb 2000
perem9 dd ?
perem10 dd ?
perem11 RECT
perem12 dd ?
perem13 dd ?
perem14 PAINTSTRUCT
perem8 dd ?
perem15 rb 2000
perem16 dd ?
perem17 dd ?
perem18 dd ?
perem19 dd 0
perem21 dd ?
perem22 dd ?
;---
OldWndProc dd ?
hUrlBrush  dd ?
sscolor dd ?
hHyper dd ?
Oldhwnd dd ?
;
title_error   db  'Something Wrong',0
offset_error  rb  200
lpfmtc	      db  'Address: %0.8X',13,10,13,10,'Registers:',13,10,'eax:',09,'%0.8X',13,10,'ecx:',09,'%0.8X',13,10,\
		  'edx:',09,'%0.8X',13,10,'ebx:',09,'%0.8X',13,10,'esp:',09,'%0.8X',13,10,'ebp:',09,'%0.8X',13,10,'esi:',09,'%0.8X',\
		  13,10,13,10,09,'Coded by Xekep',13,10,09,'ICQ: %u-%u',13,10,09,'PURE 100%% ASM',0

;--- license
lmem dd ?
time_count dd ?
flag_license db 0

struct DLGTEMPLATE
  style 	dd ?
  extendedstule dd ?
  cdit		dw ?
  x		dw ?
  y		dw ?
  cx		dw ?
  cy		dw ?
		dd ?
ends

TEMPLATE_ABOUT DLGTEMPLATE DS_CENTER+WS_POPUP+WS_VISIBLE,WS_EX_TOPMOST+WS_EX_TOOLWINDOW,0,0,0,156,65;207,80

;include 'antidebug.inc'

.code
start:
;        invoke GlobalAlloc,GPTR,50000
;        push eax
;        invokex preg_match,opreg1,gomneco,eax,2,50000,20h    ;'<title>*</title>'
;        .if eax=0
;                invoke MessageBoxA,0,'PARSER ERR!',0,0
;        .else
;                mov eax,[esp]
;                invoke MessageBoxA,0,eax,0,0
;       .endif
;        pop eax
;        invoke GlobalFree,eax
;       .if eax<>0
;                invoke MessageBoxA,0,'GlobalFree ERR!',0,0
;        .endif
;        jmp exit

;opreg1           db      '<div class="content">*</div>*</div>',0

;        gomneco db '7<8<<<9789<div class="content">123<<</div>123</div><<<8908908908908908089089089089<<0890890<div class="content">456</div>456<<</div>890890890890<<80890890890890890890<div class="content">789</div>789</div>789789',0;FILE '1.htm'  ;


;         db 0
	call set_seh
	invoke CreateMutexA,0,0,"fora notify"
	mov [hmutex],eax
	invoke GetLastError
	.if eax=0B7h
		invoke CloseHandle,[hmutex]
		invokex DialogPOPUP,'Возможен запуск только одной копии программы!',1
		invoke Sleep,4000
		jmp exit
	.endif
	invoke LoadLibrary,'riched32.dll'
	invoke LoadLibraryA,'msvcrt.dll'
	invoke GetProcAddress,eax,'time'
	mov [time],eax
	invoke GetCommandLineA
	push eax
	invoke lstrlenA,eax
	pop ecx
	sub eax,9
	add eax,ecx
	invoke lstrcmpA,eax,command
	.if eax=0
		mov [tray_flag],1
		invoke Sleep,5000
	.endif
	invoke GetModuleHandleA,0
	mov [mhandle],eax
	invokex getlicense
	.if eax=0
		invoke DialogBoxParamA,[mhandle],IDD_LICENSE,HWND_DESKTOP,plicense,0
		.if eax=-1
			jmp exit
		.endif
	.endif
	invoke OutputDebugStringA,'%s%s'
	invoke DialogBoxParamA,[mhandle],IDD_MAIN,HWND_DESKTOP,main,0
       exit:
	invoke CloseHandle,[hmutex]
	.if [snow_mutex]<>0
		invoke CloseHandle,[snow_mutex]
	.endif
	invoke ExitProcess,0

   error:
	EXCEPTION_RECORD equ dword [esp+4]
	ExceptionAdress equ 12

	mov edi,EXCEPTION_RECORD
	mov edi,dword [edi+ExceptionAdress]
	invoke wsprintf,offset_error,lpfmtc,edi,eax,ecx,edx,ebx,esp,ebp,esi,667,416
	invoke FindWindowA,0,'FORA Notify 1.0'
	invoke MessageBoxA,eax,offset_error,title_error,MB_ICONWARNING
	jmp exit

proc set_seh
	pop eax
	push error ; назначаем свой обработчик структурных исключений
	push dword [fs:0]      ; сохраняем старый обработчик в цепочке
	mov dword [fs:0],esp   ; регистрируем новый обработчик
	push eax
	ret
endp

proc main hwnd, msg, wparam, lparam
	push ebx edi esi
	cmp [msg],WM_HOTKEY
	je .hotkey
	cmp [msg],WM_TIMER
	je .timer
	cmp [msg],WM_INITDIALOG
	je .wminitdialog
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
	;cmp [msg],WM_CLOSE
	;je  .wmclose
	cmp [msg],WM_SETCURSOR
	je .cursor_over_window
	cmp [msg],WM_CTLCOLORSTATIC
	je .wmctlcolorstatic
	cmp [msg],WM_PAINT
	je .wmpaint
	xor eax,eax
	jmp .finish
    .wmpaint:
	invoke GetDlgItem,[hwnd],IDL_LOGO
	push eax
	invoke BeginPaint,eax,ps
	mov [hdc],eax
	invoke CreateCompatibleDC,[hdc]
	mov [hMemDC],eax
	invoke SelectObject,[hMemDC],[hBitmap]
	invoke GetClientRect,dword [esp+4],rect
	invoke BitBlt,[hdc],0,0,[rect.right],[rect.bottom],[hMemDC],0,0,SRCCOPY
	invoke DeleteDC,[hMemDC]
	pop eax
	invoke EndPaint,eax,ps
	xor eax,eax
	jmp .finish
    .wminitdialog:
	.if [tray_flag]=1
		invoke ShowWindow,[hwnd],SW_HIDE
		invoke PostMessageA,[hwnd],WM_SIZE,SIZE_MINIMIZED,0
	.endif
	invoke GetSystemTime,p
	.if [p.wMonth]=1 | [p.wMonth]=12
		.if [p.wMonth]=12 & [p.wDay]<>31
			push ID_LOGO
		.else
			push ID_LOGO2
			invoke SetTimer,[hwnd],TIMER_SNOW,TIME_SNOW,0
			invoke CreateMutexA,0,0,"fora notify logo snow"
			mov [snow_mutex],eax
			invoke ReleaseMutex,[snow_mutex]
		.endif
	.else
		push ID_LOGO
	.endif
	invoke LoadBitmap,[mhandle]
	mov [hBitmap],eax
	.if [firstrun]>0
		invoke GetDlgItem,[hwnd],IDL_LOGO
		invokex AddTooltip,eax,'О программе...'
	.endif
	invoke CreatePopupMenu
	mov [hMenu],eax
	invoke AppendMenu,[hMenu],MF_STRING,IDM_START,str1
	invoke AppendMenu,[hMenu],MF_STRING,IDM_MAIN,str2
	invoke AppendMenu,[hMenu],MF_STRING,IDM_TARIF,str7
	invoke AppendMenu,[hMenu],MF_STRING,IDM_SITE,str4
	invoke AppendMenu,[hMenu],MF_STRING,IDM_UPDATE,str5
	invoke AppendMenu,[hMenu],MF_STRING,IDM_LICENSE,str8
	invoke AppendMenu,[hMenu],MF_STRING,IDM_ABOUT,str6
	invoke AppendMenu,[hMenu],MF_STRING,IDM_POST,str9
	invoke AppendMenu,[hMenu],MF_SEPARATOR,0,0
	invoke AppendMenu,[hMenu],MF_STRING,IDM_EXIT,str3
	invoke GetSystemMenu,[hwnd],0
	mov [hMenu2],eax
	invoke AppendMenu,[hMenu2],MF_SEPARATOR,0,0
	invoke AppendMenu,[hMenu2],MF_STRING,IDM_SITE,str4
	invoke AppendMenu,[hMenu2],MF_STRING,IDM_LICENSE,str8
	invoke AppendMenu,[hMenu2],MF_STRING,IDM_ABOUT,str6
	invoke AppendMenu,[hMenu2],MF_STRING,IDM_POST,str9
	invoke AppendMenu,[hMenu2],MF_SEPARATOR,0,0
	invoke AppendMenu,[hMenu2],MF_STRING,IDM_EXIT,str3
	invoke GlobalAlloc,GPTR,MAX_PATH+10
	mov [buff_pass],eax
	invoke GetModuleFileNameA,[mhandle],[buff_pass],MAX_PATH+10
	add eax,[buff_pass]
	invoke lstrcpyA,eax,command
	mov [vsize],MAX_PATH
	invoke RegOpenKeyEx,key_root,subkey,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke GlobalAlloc,GPTR,MAX_PATH+10
		mov [buff_login],eax
		invoke RegQueryValueExA,[HKey],key_name,0,lpType,[buff_login],vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			invoke lstrcmpA,[buff_login],[buff_pass]
			.if eax=0
				 invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_CHECKED,0
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
				invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,BST_CHECKED,0
			.endif
		.else
			invoke RegCloseKey,[HKey]
			invoke GetDlgItem,[hwnd],ID_ONOFF
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],ID_APPLY
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],ID_SOUND
			invoke EnableWindow,eax,0
		.endif
	.endif
	mov [vsize],10
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke RegQueryValueExA,[HKey],key_name3,0,lpType,notify_flag2,vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			invoke SetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_CHECKED,0
			.if [notify_flag]=1
				invoke SetTimer,[hwnd],TIMER_CHECK,TIME_CHECK,0
				invoke CreateThread,0,0,check,[hwnd],0,0
			.endif
		.else
			.if [notify_flag]=1
				invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
				invoke EnableWindow,eax,0
				invoke GetDlgItem,[hwnd],ID_APPLY
				invoke EnableWindow,eax,0
				invoke GetDlgItem,[hwnd],ID_SOUND
				invoke EnableWindow,eax,0
			.endif
			invoke RegCloseKey,[HKey]
		.endif
	.endif
	mov [vsize],1
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke RegQueryValueExA,[HKey],key_name4,0,lpType,update_flag,vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			.if [update_flag]=1
				invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_CHECKED,0
				invoke CreateThread,0,0,getversion,[hwnd],0,0
				invoke SetTimer,[hwnd],TIMER_UPDATE,TIME_UPDATE,0
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
				invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_CHECKED,0
				invoke RegisterHotKey,[hwnd],IDK_MONEY,MOD_ALT,4Dh
			.endif
		.else
			invoke RegCloseKey,[HKey]
		.endif
	.endif
	mov [vsize],1
	invoke RegOpenKeyEx,key_root,subkey2,0,KEY_QUERY_VALUE,HKey
	.if eax=0
		invoke RegQueryValueExA,[HKey],key_name6,0,lpType,sound_flag,vsize
		.if eax=0
			invoke RegCloseKey,[HKey]
			.if [sound_flag]=1
				invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_CHECKED,0
			.endif
		.else
			invoke RegCloseKey,[HKey]
		.endif
	.endif
	invoke GlobalFree,[buff_pass]
	invoke GetDlgItem,[hwnd],ID_UPDOWN
	invoke SendMessage,eax,UDM_SETRANGE32,0,15000
	invoke CreateFontW,26,0,0,0,1000,0,0,0,DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
	mov [font4],eax
	invoke GetDlgItem,[hwnd],ID_GET
	invoke SendMessage,eax,WM_SETFONT,[font4],TRUE
	invoke CreateFontW,20,0,0,0,1000,0,0,0,DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
	mov [font5],eax
	invoke GetDlgItem,[hwnd],99
	invoke SendMessage,eax,WM_SETFONT,[font5],TRUE
	invoke CreateFontW,15,0,0,0,400,0,0,0,DEFAULT_CHARSET,0,0,ANTIALIASED_QUALITY,DEFAULT_PITCH or FF_DONTCARE,"Arial"
	mov [font6],eax
	invoke GetDlgItem,[hwnd],IDL_STAT
	invoke SendMessage,eax,WM_SETFONT,[font6],TRUE
	invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,0,0,SWP_NOMOVE+SWP_NOSIZE
	invoke SetWindowLongA,[hwnd],GWL_EXSTYLE,WS_EX_LAYERED,0
	invoke SetLayeredWindowAttributes,[hwnd],0,240,LWA_ALPHA
	invoke LoadIcon,[mhandle],ID_ICON
	invoke SendMessageA,[hwnd],WM_SETICON,ICON_BIG,eax
	invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
	invoke	SetWindowLong,eax,GWL_WNDPROC,WindowProcMoney
	mov [OldWndProc2],eax
	invoke GetDlgItem,[hwnd],IDL_WWW
	mov [hHyper],eax
	invoke	SetWindowLong,eax,GWL_WNDPROC,WindowProc
	invoke GetDlgItem,[hwnd],IDL_LOGO
	invoke	SetWindowLong,eax,GWL_WNDPROC,WindowProc
	mov [OldWndProc],eax
	invoke GetSysColor,COLOR_BTNFACE
	invoke CreateSolidBrush,eax
	mov [hUrlBrush],eax
	mov [sscolor],clrMain
	mov eax,[hwnd]
	mov [Oldhwnd],eax
	invokex setstat,[hwnd],stat_ok
	jmp .processed
    .cursor_over_window:
	cmp [sscolor],clrMain
	je  @f
	mov [sscolor],clrMain
	invoke InvalidateRect,[hHyper],NULL,TRUE
	jmp .processed
      @@:
	xor eax,eax
	jmp .finish
    .wmctlcolorstatic:
	xor eax,eax
	mov ecx,[lparam]
	cmp ecx,[hHyper]
	jne @f
	cmp [sscolor],clrMain
	mov eax,[sscolor]
	invoke SetTextColor,[wparam],eax
       @@:
	invoke SetBkMode,[wparam],TRANSPARENT
	mov eax,[hUrlBrush]
	jmp .finish
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
			invoke ShellExecuteA,0,'Open',site,0,0,SW_SHOW
			jmp .processed
		.elseif ax=IDM_UPDATE
			invoke CreateThread,0,0,getversion,[hwnd],0,0
			jmp .processed
		.elseif ax=IDM_ABOUT
			invoke Shell_NotifyIcon,NIM_DELETE,notes
			invoke DialogBoxIndirectParamA,[mhandle],TEMPLATE_ABOUT,HWND_DESKTOP,aboutproc,0
			invoke Shell_NotifyIcon,NIM_ADD,notes
			jmp .processed
		.elseif ax=IDM_TARIF
			invoke Shell_NotifyIcon,NIM_DELETE,notes
			invoke DialogBoxParamA,[mhandle],IDD_TARIF,HWND_DESKTOP,tarifproc,0
			invoke Shell_NotifyIcon,NIM_ADD,notes
			jmp .processed
		.elseif ax=IDM_LICENSE
			invoke Shell_NotifyIcon,NIM_DELETE,notes
			invoke DialogBoxParamA,[mhandle],IDD_LICENSE,HWND_DESKTOP,plicense,1
			invoke Shell_NotifyIcon,NIM_ADD,notes
			jmp .processed
		.elseif ax=IDM_POST
			invoke Shell_NotifyIcon,NIM_DELETE,notes
			invoke DialogBoxParamA,[mhandle],IDD_POST,HWND_DESKTOP,ppost,0
			invoke Shell_NotifyIcon,NIM_ADD,notes
			jmp .processed
		.else
			invoke DestroyWindow,[hwnd]
		.endif
		invoke Shell_NotifyIcon,NIM_DELETE,notes
	.endif
	.if [wparam]=BN_CLICKED shl 16 + IDCANCEL
		invoke ShowWindow,[hwnd],SW_HIDE
		invoke PostMessageA,[hwnd],WM_SIZE,SIZE_MINIMIZED,0
		jmp .processed
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
	cmp [wparam],BN_CLICKED shl 16 + ID_SOUND
	je .sound
	cmp [wparam],BN_CLICKED shl 16 + ID_TARIF
	je .tarif
	cmp [wparam],BN_CLICKED shl 16 + ID_APPLY
	jne .processed
	invoke IsDlgButtonChecked,[hwnd],ID_ONOFF
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke GetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2,10
		.if eax=0
			invokex DialogPOPUP,stat_err_money,0
			mov word [notify_flag2],30h
			invoke SetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2
		.endif
		invoke RegSetValueEx,[HKey],key_name3,0,REG_SZ,notify_flag2,10
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
		.else
			invokex setstat,[hwnd],stat_ok
		.endif
	.endif
	jmp .processed
    .wmclose:
	.if [hkey_flag]<>0
		invoke UnregisterHotKey,[hwnd],IDK_MONEY
	.endif
	invoke DeleteObject,[hBitmap]
	invoke DeleteObject,[font4]
	invoke DeleteObject,[font5]
	invoke DeleteObject,[font6]
	invoke EndDialog,[hwnd],0
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
		 invoke ShellExecuteA,0,'Open',site,0,0,SW_SHOW
		 jmp .processed
	.elseif [wparam]=IDM_ABOUT
		 invoke ShowWindow,[hwnd],SW_HIDE
		 invoke DialogBoxIndirectParamA,[mhandle],TEMPLATE_ABOUT,HWND_DESKTOP,aboutproc,0
		 invoke ShowWindow,[hwnd],SW_SHOW
		 jmp .processed
	.elseif [wparam]=IDM_LICENSE
		 invoke ShowWindow,[hwnd],SW_HIDE
		 invoke DialogBoxParamA,[mhandle],IDD_LICENSE,HWND_DESKTOP,plicense,1
		 invoke ShowWindow,[hwnd],SW_SHOW
		 jmp .processed
	.elseif [wparam]=IDM_EXIT
		 invoke Shell_NotifyIcon,NIM_DELETE,notes
		 jmp .wmclose
	.elseif [wparam]=IDM_POST
		 invoke ShowWindow,[hwnd],SW_HIDE
		 invoke DialogBoxParamA,[mhandle],IDD_POST,HWND_DESKTOP,ppost,0
		 invoke ShowWindow,[hwnd],SW_SHOW
		 jmp .processed
	.endif
	xor eax,eax
	jmp .finish
.timer:
	.if [wparam]=TIMER_UPDATE
		invokex getwinfullscr
		.if eax=1
			jmp .processed
		.endif
		mov [update_flag],1
		invoke CreateThread,0,0,getversion,[hwnd],0,0
	.elseif [wparam]=TIMER_CHECK
		invoke CreateThread,0,0,check,[hwnd],0,0
	.elseif [wparam]=TIMER_SNOW
		invoke GetDlgItem,[hwnd],IDL_LOGO
		invoke CreateThread,0,0,snow,eax,0,0
	.endif
	jmp .processed
.hotkey:
	mov eax,[wparam]
	.if eax=IDK_MONEY
		mov [flag],1
		jmp .get_info
	.endif
	jmp .processed
.wmsize_tray:
	.if [wparam]=SIZE_MINIMIZED
		mov [tray_flag],1
		mov [notes.cbSize],sizeof._NOTIFYICONDATA
		push [hwnd]
		pop [notes.hWnd]
		mov [notes.uID],IDI_TRAY
		mov [notes.uFlags],NIF_ICON+NIF_MESSAGE+NIF_TIP
		mov [notes.uCallbackMessage],WM_SHELLNOTIFY
		invoke LoadIconA,[mhandle],ID_ICON
		mov [notes.hIcon],eax
		invoke lstrcpy,notes.szTip,trayname
		invoke ShowWindow,[hwnd],SW_HIDE
		invoke Shell_NotifyIcon,NIM_ADD,notes
		.if [firstrun]=2
			invoke RtlZeroMemory,notes2,sizeof._NOTIFYICONDATA
			mov [notes2.cbSize],sizeof._NOTIFYICONDATA
			push [hwnd]
			pop [notes2.hWnd]
			mov [notes2.uID],IDI_TRAY
			invoke lstrcpyA,notes2.szInfo,"Нажатием кнопки 'X' сворачивается в трей главное окно. Двойной клик по иконке в трее восстановит окно программы. Вызов контекстного меню по нажатию правой кнопкой мыши на иконке в трее."
			invoke lstrcpyA,notes2.szInfoTitle,'Подсказка'
			mov [notes2.uTimeout],1000*5
			mov [notes2.dwInfoFlags],NIIF_INFO
			mov [notes2.uFlags],NIF_INFO
			invoke Shell_NotifyIcon,NIM_MODIFY,notes2
			mov [firstrun],1
		.endif
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
			invoke TrackPopupMenu,[hMenu],TPM_RIGHTALIGN,[pt.x],[pt.y],NULL,[hwnd],NULL
			invoke PostMessageA,[hwnd],WM_NULL,0,0
		.endif
	.endif
	jmp .processed
.onoff:
	invoke IsDlgButtonChecked,[hwnd],ID_ONOFF
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke GetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2,10
		.if eax=0
			invokex DialogPOPUP,stat_err_money,0
			mov word [notify_flag2],30h
			invoke SetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2
		.endif
		invoke RegSetValueEx,[HKey],key_name3,0,REG_SZ,notify_flag2,10
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
		.else
			invokex setstat,[hwnd],stat_ok
			invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
			invoke EnableWindow,eax,1
			invoke GetDlgItem,[hwnd],ID_APPLY
			invoke EnableWindow,eax,1
			invoke GetDlgItem,[hwnd],ID_SOUND
			invoke EnableWindow,eax,1
			.if [notify_flag]=1
				invoke SetTimer,[hwnd],TIMER_CHECK,TIME_CHECK,0
			.endif
		.endif
	.else
		invoke KillTimer,[hwnd],TIMER_CHECK
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name3
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_CHECKED,0
		.else
			invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],ID_APPLY
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],ID_SOUND
			invoke EnableWindow,eax,0
			invokex setstat,[hwnd],stat_ok
		.endif
	.endif
	invoke RegCloseKey,[HKey]
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
	mov ecx,[vsize]
	mov eax,[buff_login]
	.if byte [eax]<>0			 ;;;;;;;;;;;;;;;;;;;; ЖО ЖО ЖО
		invoke RegCloseKey,[HKey]
		invokex DialogPOPUP,err_acc,0
		invoke GlobalFree,[buff_login]
		jmp .get_login
	.endif
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
		jmp .get_login
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
	invoke GetDlgItem,[hMenu],IDM_TARIF
	invoke EnableWindow,eax,0
	invoke EnableMenuItem,[hMenu],IDM_START,MF_DISABLED
	invoke EnableMenuItem,[hMenu],IDM_TARIF,MF_DISABLED
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
		invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,321,412,SWP_NOMOVE
	.else
		invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,321,297,SWP_NOMOVE
	.endif
	jmp .processed
  .autorun:
	invoke IsDlgButtonChecked,[hwnd],ID_AUTORUN
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_autorun1,0
			invokex setstat,[hwnd],stat_err_autorun1
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_UNCHECKED,0
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
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_UNCHECKED,0
		.else
			invokex setstat,[hwnd],stat_ok
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_CHECKED,0
			invokex DialogPOPUP,stat_err_autorun2,0
			invokex setstat,[hwnd],stat_err_autorun2
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name
		.if eax<>0
			invokex DialogPOPUP,stat_err_autorun2,0
			invokex setstat,[hwnd],stat_err_autorun2
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_CHECKED,0
		.else
			invokex setstat,[hwnd],stat_ok
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
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name4,0,REG_BINARY,flagg,1
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_UNCHECKED,0
		.else
			invoke SetTimer,[hwnd],TIMER_UPDATE,TIME_UPDATE,0
			invokex setstat,[hwnd],stat_ok
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_CHECKED,0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name4
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_CHECKED,0
		.else
			invoke KillTimer,[hwnd],TIMER_UPDATE
			invokex setstat,[hwnd],stat_ok
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
.sethotkey:
	invoke IsDlgButtonChecked,[hwnd],ID_HOTKEY
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name5,0,REG_BINARY,flagg,1
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_UNCHECKED,0
		.else
			invoke RegisterHotKey,[hwnd],IDK_MONEY,MOD_ALT,4Dh
			invokex setstat,[hwnd],stat_ok
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_CHECKED,0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name5
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_CHECKED,0
		.else
			invoke UnregisterHotKey,[hwnd],IDK_MONEY
			mov [hkey_flag],0
			invokex setstat,[hwnd],stat_ok
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
.notifys:
	invoke IsDlgButtonChecked,[hwnd],ID_NOTIFY
	.if eax=BST_CHECKED
		invoke IsDlgButtonChecked,[hwnd],ID_ONOFF
		.if eax=BST_CHECKED
			invoke SetTimer,[hwnd],TIMER_CHECK,TIME_CHECK,0
		.endif
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name2,0,REG_BINARY,flagg,1
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,BST_UNCHECKED,0
		.else
			mov [notify_flag],1
			invokex setstat,[hwnd],stat_ok
			invoke GetDlgItem,[hwnd],ID_ONOFF
			invoke EnableWindow,eax,1
			invoke IsDlgButtonChecked,[hwnd],ID_ONOFF
			.if eax=BST_CHECKED
				invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
				invoke EnableWindow,eax,1
				invoke GetDlgItem,[hwnd],ID_APPLY
				invoke EnableWindow,eax,1
				invoke GetDlgItem,[hwnd],ID_SOUND
				invoke EnableWindow,eax,1
			.endif
		.endif
	.else
		invoke KillTimer,[hwnd],TIMER_CHECK
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,BST_CHECKED,0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name2
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,BST_CHECKED,0
		.else
			mov [notify_flag],0
			invokex setstat,[hwnd],stat_ok
			invoke GetDlgItem,[hwnd],ID_ONOFF
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],ID_APPLY
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],ID_SOUND
			invoke EnableWindow,eax,0
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
.sound:
	invoke IsDlgButtonChecked,[hwnd],ID_SOUND
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name6,0,REG_BINARY,flagg,1
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_UNCHECKED,0
		.else
			mov [sound_flag],1
			invokex setstat,[hwnd],stat_ok
		.endif
	.else
		mov [sound_flag],0
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_CHECKED,0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name6
		.if eax<>0
			invokex DialogPOPUP,stat_err_reg,0
			invokex setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_CHECKED,0
		.else
			invokex setstat,[hwnd],stat_ok
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed

.tarif:
	invoke ShowWindow,[hwnd],SW_HIDE
	invoke DialogBoxParamA,[mhandle],IDD_TARIF,HWND_DESKTOP,tarifproc,0
	invoke ShowWindow,[hwnd],SW_SHOW
	jmp .processed
endp

convert_char:
	sub al,'0'
	cmp al,10
	jb .done
	add al,'0'
	and al,0x5f
	sub al,'A'-10
	and al,0x0f
	.done:
	ret
atol:
	push esi
	xor eax,eax
	xor ebx,ebx
	cmp byte [esi],'-'
	jnz next
	inc esi
next:
	lodsb
	or al,al
	jz .done
	call convert_char

	imul ebx,ecx
	add ebx,eax
	jmp short next
.done:
	xchg ebx,eax
	pop esi
	cmp byte [esi],'-'
	jz negative
	ret
negative:
	neg eax
	ret

ConcatStrings:
	pop edi
	pop eax
	pop ecx
     concatx:
	pop edx
	push ecx
	.if eax<>0
		invoke lstrcatA,eax,edx
	.endif
	pop ecx
	loop concatx
	mov esi,eax
	invoke lstrlenA,eax
	push edi
	ret


proc WindowProc hwnd, msg, wparam, lparam
	cmp [msg],WM_SETCURSOR	    ; Курсор над ссылкой
	je .cursor_over_hyperlink
	cmp [msg],WM_LBUTTONDOWN    ; Нажатие мышкой на ссылке
	je .open_link
	invoke CallWindowProc,[OldWndProc],[hwnd],[msg],[wparam],[lparam]
	ret
.cursor_over_hyperlink:
	mov eax,[hHyper]
	.if [hwnd]<>eax
		jmp .finish
	.endif
	invoke LoadCursor,0,IDC_HAND
	invoke SetCursor,eax
	mov [sscolor],clrActive
	invoke InvalidateRect,[hHyper],NULL,TRUE
	jmp .finish
.open_link:
	mov eax,[hHyper]
	.if [hwnd]=eax
		invoke ShellExecute,[hwnd],'open',site,0,0,SW_SHOW
	.else
		invoke ShowWindow,[Oldhwnd],SW_HIDE
		invoke DialogBoxIndirectParamA,[mhandle],TEMPLATE_ABOUT,HWND_DESKTOP,aboutproc,0
		invoke ShowWindow,[Oldhwnd],SW_SHOW
	.endif
.finish:
	xor eax,eax
	ret
endp

proc WindowProcMoney hwnd, msg, wparam, lparam
	cmp [msg],WM_CHAR
	je .char
      .end!:
	invoke CallWindowProc,[OldWndProc2],[hwnd],[msg],[wparam],[lparam]
	ret
     .char:
	cmp [wparam],VK_BACK
	jne .end!
     .finish:
	xor eax,eax
	ret
endp

proc SetWindowCenter hwnd
	local dtop:RECT
	lea eax,[dtop]
	invoke SystemParametersInfo,SPI_GETWORKAREA,NULL,eax,FALSE
	mov eax,[dtop.right]
	mov ecx,[dtop.bottom]
	push eax
	push ecx
	lea eax,[dtop]
	invoke GetClientRect,[hwnd],eax
	pop ecx
	pop eax
	sub ecx,[dtop.bottom]
	sub eax,[dtop.right]
	mov [dtop.bottom],ecx
	mov [dtop.right],eax
	xor edx,edx
	mov eax,[dtop.right]
	mov ebx,2
	div ebx
	xor edx,edx
	mov [dtop.left],eax
	mov eax,[dtop.bottom]
	mov ebx,2
	div ebx
	mov [dtop.top],eax
	invoke SetWindowPos,[hwnd],HWND_TOP,[dtop.left],[dtop.top],0,0,SWP_NOSIZE
	ret
endp

proc AddTooltip hwnd, text
	local hinst:DWORD
	local ti:TOOLINFO
	invoke GetWindowLong,[hwnd],GWL_HINSTANCE
	mov [hinst],eax
	invoke CreateWindowExA,WS_EX_TOPMOST,TOOLTIPS_CLASS,0,WS_POPUP+TTS_NOPREFIX+TTS_ALWAYSTIP,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,0,0,eax,0
	mov esi,eax
	mov [ti.uFlags],TTF_SUBCLASS
	mov eax,[hwnd]
	mov [ti.hwnd],eax
	mov eax,[hinst]
	mov [ti.hInst],eax
	mov [ti.uId],0
	mov eax,[text]
	mov [ti.lpszText],eax
	lea eax,[ti.Rect]
	invoke GetClientRect,[hwnd],eax
	lea eax,[ti]
	invoke SendMessage,esi,TTM_ADDTOOL,0,eax
	ret
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

display 'Компилим check.inc',13,10

include 'check.inc'

display 'Компилим sound.inc',13,10

include 'sound.inc'

display 'Компилим about.inc',13,10

include 'about.inc'

display 'Компилим getwinfullscr.inc',13,10

include 'getwinfullscr.inc'

display 'Компилим tariff.inc',13,10

include 'tariff.inc'

display 'Компилим https_request.inc',13,10

include 'https_request.inc'

display 'Компилим license.inc',13,10

include 'license.inc'

include 'snow.inc'

include 'post.inc'

.end start

section '.res' readable data resource from 'res.res'

display 'Компилирование успешно завершено О_о',13,10