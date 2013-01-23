format pe gui 4.0 on 'stub'
include '%fasm_inc%\WIN32AX.INC'
include 'fmod\#ufmod.inc'
include 'macro.inc'
entry start

clrMain 	=     0FF0000h	; Обычный цвет ссылки (синий)
clrActive	=     00000FFh	; Цвет активной ссылки (красный)

DEBUG=1

if DEBUG
	display 'debug mode'
else
	display 'release mode'
end if

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
TIMER_CRC32 = 1
TIME_CRC32 = 1000

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
HTCLOSE = 20

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

section '.data1' readable writable executable
crc32res_start:
data resource from 'res.res'
end data
crc32res_end:
CRC32 crc32res,crc32res_start,crc32res_end-crc32res_start

data_import

crc32adebug_start:
include 'antidebug.inc'
crc32adebug_end:

CRC32 crc32adebug,crc32adebug_start,crc32adebug_end-crc32adebug_start

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
crc32strings_start:
main_title db 'FORA Notify '
if DEBUG>0
db 'debug version '
end if
cur_ver db '1.1.1',0
stat_err_autorun1 db 'Ошибка добавления в автозапуск',0
stat_err_autorun2 db 'Ошибка удаления из автозапуска',0
stat_err_reg db 'Ошибка работы с реестром',0
err_acc db 'Введите данные',0
err_connect1 db 'Не могу подключиться',0
err_connect2 db 'Не могу получить IP адрес',0
err_connect3 db 'Ошибка подключения',0
err_mem1 db 'Не могу выделить память',0
err_connect4 db 'Необходимо указать рабочий аккаунт',0
stat_err_money db 'Необходимо ввести число',0
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

CrtTable:; rb 1024
db 00h,00h,00h,00h,51h,12h,0C2h,4Ah,4Bh,98h,83h,3Bh,0C3h,53h,2Dh,2Bh,96h,30h,07h,77h,45h,0F4h,77h,7Dh,86h,0A7h,5Ah,56h,50h,68h,0F1h,54h
db 19h,0C4h,6Dh,07h,1Ah,8Ah,41h,71h,55h,0D7h,0AEh,2Eh,0AEh,6Bh,31h,39h,0A7h,6Bh,0FDh,65h,4Ch,4Dh,62h,23h,0C7h,96h,41h,4Fh,0F4h,1Ch,71h,2Dh
db 32h,88h,0DBh,0Eh,0C7h,22h,0C5h,3Dh,29h,2Eh,77h,1Eh,0A2h,0A9h,0E1h,7Ah,0AAh,0AEh,5Dh,5Dh,14h,0E6h,0B5h,37h,5Ch,0D7h,62h,72h,4Bh,0C2h,0C1h,02h
db 0DDh,0A8h,84h,4Ch,8Ch,0BAh,46h,06h,98h,9Ah,0C4h,46h,44h,32h,0CDh,41h,0FBh,0BCh,9Fh,17h,0Eh,36h,0B6h,7Fh,0E8h,39h,0E2h,5Ah,4Dh,0E4h,7Ah,17h
db 64h,10h,0B7h,1Dh,0E4h,83h,21h,5Ch,8Eh,45h,8Ah,7Bh,24h,0Eh,0BBh,2Ch,52h,5Ch,0EEh,3Ch,8Fh,0A7h,96h,0Ah,0C9h,0A0h,0F7h,30h,69h,85h,63h,33h
db 8Fh,0F4h,6Ah,70h,0EBh,9Fh,46h,44h,28h,0CCh,6Bh,6Fh,0E5h,0F3h,0B2h,02h,31h,5Bh,0FAh,12h,0D9h,0ABh,0DFh,5Fh,96h,84h,83h,05h,0A9h,08h,0F9h,43h
db 2Bh,4Ch,0B6h,09h,0D9h,83h,2Eh,63h,18h,75h,8Dh,0Ch,4Ch,0D9h,1Ah,3Dh,03h,0DEh,0B7h,5Bh,73h,7Dh,91h,47h,0E3h,98h,0FAh,63h,15h,0F3h,24h,21h
db 0F6h,79h,3Fh,2Fh,0E8h,36h,12h,3Ah,79h,0DEh,0A9h,25h,9Ch,77h,59h,1Eh,0FFh,79h,0F3h,73h,5Fh,24h,74h,35h,9Ah,0C8h,0F5h,2Eh,7Fh,54h,10h,42h
db 0C8h,20h,6Eh,3Bh,83h,0B8h,0EDh,00h,4Fh,5Dh,0EFh,5Fh,75h,0F9h,15h,6Ch,79h,10h,58h,35h,0A0h,5Dh,87h,46h,48h,1Ch,76h,59h,0A8h,5Ah,3Bh,61h
db 0A4h,0B8h,0DCh,79h,4Dh,3Eh,0C0h,03h,1Eh,4Fh,2Dh,15h,38h,5Bh,36h,4Eh,92h,41h,0EFh,61h,48h,31h,0D7h,45h,0D2h,0Ah,0C7h,66h,69h,49h,0F4h,04h
db 7Dh,0D4h,0DAh,1Ah,0BCh,0CDh,51h,75h,9Bh,0F5h,0A7h,4Fh,0D5h,11h,1Ch,59h,0E1h,36h,0DEh,66h,0D4h,6Bh,2Fh,47h,0CAh,0E7h,65h,05h,15h,9Ch,86h,29h
db 62h,0B6h,0F4h,25h,2Ah,0FDh,56h,02h,0EFh,5Ah,2Ah,20h,51h,0C1h,0E9h,60h,2Ch,09h,07h,0Bh,64h,8Ch,54h,3Ch,8Bh,4Fh,5Bh,72h,6Bh,09h,16h,4Ah
db 56h,98h,6Ch,13h,0B2h,0DBh,0D0h,51h,25h,09h,0CBh,57h,0E3h,0C9h,12h,1Bh,30h,0EAh,1Ah,19h,0C4h,3Fh,15h,31h,98h,0B2h,35h,7Ah,58h,3Fh,0AFh,0Ah
db 0BFh,1Eh,70h,69h,77h,0E8h,1Fh,5Ah,5Bh,0E1h,18h,51h,73h,0C3h,0B5h,75h,0CDh,65h,9Bh,54h,95h,2Dh,0D7h,7Bh,2Ah,0E6h,49h,42h,0B4h,0BBh,32h,71h
db 0ECh,0F3h,7Eh,5Eh,0F9h,43h,53h,6Ah,0D0h,6Dh,24h,74h,6Ah,54h,24h,08h,0F2h,0BCh,53h,4Bh,0EEh,70h,0FBh,47h,38h,0EFh,0B2h,3Ch,0D5h,7Eh,0BEh,51h
db 0BDh,0E1h,0BCh,14h,7Eh,06h,15h,4Dh,0BEh,48h,0E8h,6Ah,0B1h,87h,0A4h,58h,34h,91h,0EBh,5Dh,2Fh,14h,0D7h,07h,3Bh,0A9h,11h,7Dh,11h,80h,94h,41h
db 90h,41h,0DCh,76h,14h,52h,31h,45h,06h,71h,0DBh,01h,20h,2Eh,0BBh,42h,2Fh,88h,34h,26h,5Ch,0D2h,32h,52h,95h,0E3h,26h,6Bh,71h,3Ch,79h,08h
db 0F2h,20h,0B0h,6Ah,14h,01h,0CDh,3Fh,0C3h,0E7h,0A9h,59h,45h,13h,0Fh,75h,0DFh,57h,48h,31h,9Ch,5Fh,0A8h,22h,0E1h,6Ch,9Ch,5Fh,21h,0E1h,06h,2Fh
db 0D1h,0E4h,03h,3Ch,2Dh,0EBh,1Bh,7Ah,9Ah,7Ch,80h,07h,0F3h,0BBh,23h,30h,3Ch,9Eh,5Ah,2Ah,5Fh,7Eh,36h,0Ch,03h,0D3h,21h,1Ch,0A7h,46h,5Ch,79h
db 09h,00h,0CCh,5Ch,5Fh,99h,4Eh,04h,83h,80h,2Ah,43h,64h,15h,71h,52h,09h,2Dh,6Dh,40h,07h,51h,0E7h,30h,0D2h,92h,0E8h,09h,99h,0C8h,06h,64h
db 0FAh,0A8h,0B5h,35h,0B5h,91h,0E3h,16h,0B1h,30h,36h,0Eh,6Bh,0B6h,38h,79h,1Bh,0A6h,0ACh,10h,2Bh,12h,0DCh,34h,9Fh,30h,0CBh,2Bh,17h,15h,0A3h,70h
db 0C5h,0DDh,09h,40h,0A0h,07h,0C5h,7Fh,63h,54h,0E8h,54h,0D3h,23h,08h,02h,94h,0CFh,0CBh,0Ah,52h,0E1h,0D4h,0Dh,2Ah,38h,0Dh,53h,5Ah,9Bh,2Bh,4Fh
db 0C4h,6Ch,0E9h,4Bh,6Eh,0E6h,0ABh,49h,54h,0FAh,0ADh,04h,0D3h,86h,41h,55h,0DEh,0B5h,54h,40h,0B7h,7Fh,5Bh,5Ch,05h,03h,0D6h,58h,9Ch,0CFh,0A6h,5Dh
db 58h,12h,0Eh,16h,0F1h,15h,07h,35h,0C8h,18h,0A9h,78h,0E6h,0EEh,62h,0Eh,6Dh,38h,1Ch,12h,0C7h,0DCh,7Dh,40h,9Bh,41h,23h,3Dh,0A3h,0FCh,19h,45h
db 0ACh,30h,0D9h,26h,7Ah,5Eh,74h,43h,0E7h,0A8h,5Ah,1Dh,0F1h,0F3h,91h,76h,97h,81h,0E7h,7Ch,6Fh,58h,02h,30h,0C6h,93h,25h,36h,0E7h,0Ch,7Eh,7Fh
db 60h,0D4h,35h,32h,31h,0C6h,0F7h,78h,88h,7Fh,2Ah,62h,67h,0C3h,96h,01h,21h,0CCh,0A7h,33h,03h,0A9h,54h,7Eh,0B0h,7Eh,5Eh,15h,0CBh,56h,85h,0Eh
db 0BDh,7Ch,0B1h,7Eh,0ECh,6Eh,73h,34h,7Fh,0B6h,1Bh,0Dh,6Eh,0CBh,0Ah,55h,77h,06h,0D9h,11h,0ADh,98h,27h,7Eh,8Dh,0Bh,0A6h,0Dh,0F6h,54h,9Eh,33h
db 17h,4Fh,0E1h,49h,0A7h,0F6h,0F0h,0Fh,15h,0B0h,2Dh,34h,0D4h,1Ch,0CCh,62h,93h,3Bh,0DCh,7Fh,52h,0BBh,96h,34h,91h,0E8h,0BBh,1Fh,0A0h,0B3h,41h,0Dh
db 4Fh,5Ch,01h,14h,0ADh,0C2h,65h,47h,0A5h,09h,3Ch,72h,0F4h,1Bh,0FEh,38h,01h,0B1h,0E0h,0Bh,7Ah,00h,1Eh,7Eh,0D4h,0A8h,48h,10h,2Bh,65h,3Fh,11h
db 53h,0EDh,0Eh,37h,0B9h,53h,33h,55h,0B3h,8Dh,48h,52h,0BFh,83h,7Dh,03h,70h,0DEh,65h,79h,03h,1Dh,0D0h,0Ch,97h,0FEh,0B0h,1Bh,0BCh,0BCh,8Eh,56h
db 7Ah,0C3h,79h,29h,0F0h,43h,9Fh,36h,33h,39h,3Bh,05h,0FCh,64h,23h,7Fh,0B9h,24h,0D0h,70h,0FCh,8Ah,0E5h,34h,9Fh,0A9h,0EEh,45h,0A1h,0E7h,63h,60h
db 4Fh,07h,0ADh,66h,7Ah,2Dh,0BFh,62h,5Eh,28h,0AEh,0Fh,0ADh,76h,0E1h,35h,0ADh,0A2h,37h,2Fh,0FCh,0A7h,44h,28h,4Bh,0Ah,37h,61h,1Fh,6Fh,5Ch,32h

crc32strings_end:
flagg db 1
buffer rb MAX_PATH+10
vsize dd ?
mhandle dd ?
buff_pass dd ?
buff_login dd ?
buff_trash dd ?
HKey dd ?
lpType dd ?
notify_flag db 0
notify_flag2 rb 10
update_flag db 0
tray_flag db 0
firstrun db 0
hkey_flag db 0
gcontent_mutex dd ?
sound_flag db 0
pOldProc2 dd ?
hMBHook dd ?
sec dd ?
flag_sec db 0
music_flag dd 0
UPD_flag db 0
snow_mutex dd ?
TaskbarCreated dd ?
TransparentBlt db 'TransparentBlt',0

;--- TARIF
tprogress dd ?
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
;--- Отзывы
post_time dd 0
post_count dd 0
post_button rb 5
time dd 'time',0

tOldWndProc dd ?
thUrlBrush  dd ?
tsscolor dd ?
thHyper dd ?
tOldhwnd dd ?

;--- ABOUT
text db 'FORA Notify 1.1.1 by Xekep'
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
		  'edx:',09,'%0.8X',13,10,'ebx:',09,'%0.8X',13,10,'edi:',09,'%0.8X',13,10,'esp:',09,'%0.8X',13,10,'ebp:',09,'%0.8X',13,10,'esi:',09,'%0.8X',\
		  13,10,13,10,09,'Coded by Xekep',13,10,09,'ICQ: %u-%u',13,10,09,'PURE 100%% ASM',0

;--- license
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

CRC32 crc32str,crc32strings_start,crc32strings_end-crc32strings_start
XOR crc32strings_start,crc32strings_end-crc32strings_start

section '.data0' code readable executable
start:
	call set_seh
	invoke CreateMutexA,0,0,key_name
	mov [hmutex],eax
	invoke GetLastError
	.if eax=0B7h
		invoke CloseHandle,[hmutex]
		invoke DialogBoxParamA,[mhandle],IDD_POPUP,HWND_DESKTOP,POPUP,'Возможен запуск только одной копии программы!'
		jmp exit
	.endif
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
	stdcall getlicense
	.if eax=0
		invoke DialogBoxParamA,[mhandle],IDD_LICENSE,HWND_DESKTOP,plicense,0
		.if eax=-1
			jmp exit
		.endif
	.endif
	if DEBUG=0
		invoke OutputDebugStringA,'%s%s'
	end if
	invoke DialogBoxParamA,[mhandle],IDD_MAIN,HWND_DESKTOP,main,0
       exit:
	invoke CloseHandle,[hmutex]
	invoke ExitProcess,0

   error:
	EXCEPTION_RECORD equ dword [esp+4]
	ExceptionAdress equ 12


	mov dword [offset_error],edi
	mov dword [offset_error+4],esp

	mov edi,EXCEPTION_RECORD
	mov edi,dword [edi+ExceptionAdress]

	cinvoke wsprintfA,offset_error,lpfmtc,edi,eax,ecx,edx,ebx,dword [offset_error],dword [offset_error+4],ebp,esi,667,416
	invoke FindWindowA,0,main_title
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
	je  .wmmax
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
	.if [msg]=WM_NCRBUTTONUP & [wparam]=HTCLOSE
		jmp .wmclose
	.elseif [msg]=WM_NCLBUTTONUP & [wparam]=HTCLOSE
		jmp .wmsize_tray
	.endif
	mov eax,[TaskbarCreated]
	.if [msg]=eax & [tray_flag]=1
		jmp .wmsize_tray2
	.endif
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
	stdcall dword [TransparentBlt],[hdc],0,0,[rect.right],[rect.bottom],[hMemDC],0,0,[rect.right],[rect.bottom],0FFFFFFh
    ;    invoke CreateCompatibleDC,[hdc]
    ;    mov [hMaskDC],eax
    ;    invoke CreateBitmap,[rect.right],[rect.bottom],1,1,0
    ;    mov [hMask],eax
    ;    invoke SelectObject,[hMaskDC],[hMask]
    ;    invoke SetBkColor,[hMemDC],0FFFFFFh
    ;    invoke BitBlt,[hMaskDC],0,0,[rect.right],[rect.bottom],[hMemDC],0,0,SRCCOPY
    ;    invoke BitBlt,[hdc],0,0,[rect.right],[rect.bottom],[hMaskDC],0,0,SRCERASE
    ;    invoke BitBlt,[hdc],0,0,[rect.right],[rect.bottom],[hMemDC],0,0,SRCINVERT
    ;    invoke DeleteObject,[hMask]
    ;    invoke DeleteDC,[hMaskDC]
	invoke DeleteDC,[hMemDC]
	mov eax,[esp]
	invoke ReleaseDC,eax,[hdc]
	pop eax
	invoke EndPaint,eax,ps
	invoke ValidateRect,[hwnd],rect
	xor eax,eax
	jmp .finish
    .wminitdialog:
	invoke SetWindowTextA,[hwnd],main_title
	.if [tray_flag]=1
		invoke ShowWindow,[hwnd],SW_HIDE
		invoke PostMessageA,[hwnd],WM_SIZE,SIZE_MINIMIZED,0
	.endif
	invoke RegisterWindowMessage,"TaskbarCreated"
	mov [TaskbarCreated],eax
	if DEBUG=0
		call scrc32
	end if
	invoke GetSystemTime,p
	.if [p.wMonth]=1 | [p.wMonth]=12
		.if [p.wMonth]=12 & [p.wDay]<25
			push ID_LOGO
		.else
			push ID_LOGO2
			invoke CreateMutexA,0,0,0
			mov [snow_mutex],eax
			invoke ReleaseMutex,[snow_mutex]
			invoke SetTimer,[hwnd],TIMER_SNOW,TIME_SNOW,0
		.endif
	.else
		push ID_LOGO
	.endif
	invoke LoadBitmap,[mhandle]
	mov [hBitmap],eax
	invoke GetDlgItem,[hwnd],IDL_LOGO
	invoke GetClientRect,eax,rect
	.if [firstrun]>0
		invoke GetDlgItem,[hwnd],IDL_LOGO
		stdcall AddTooltip,eax,'О программе...'
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
	invoke AppendMenu,[hMenu2],MF_STRING,IDM_UPDATE,str5
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
	xor eax,eax
	invoke CreateMutex,eax,eax,eax
	mov [gcontent_mutex],eax
	stdcall setstat,[hwnd],stat_ok
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
			invoke CreateThread,0,0,check_money,[hwnd],0,0
			jmp .processed
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
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke GetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2,10
		.if eax=0
			stdcall DialogPOPUP,stat_err_money,0
			mov word [notify_flag2],30h
			invoke SetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2
		.endif
		invoke RegSetValueEx,[HKey],key_name3,0,REG_SZ,notify_flag2,10
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
		.else
			stdcall setstat,[hwnd],stat_ok
		.endif
	.endif
	jmp .processed
    .wmclose:
	.if [snow_mutex]
		invoke CloseHandle,[snow_mutex]
	.endif
	.if [gcontent_mutex]
		invoke CloseHandle,[gcontent_mutex]
	.endif
	.if [hkey_flag]
		invoke UnregisterHotKey,[hwnd],IDK_MONEY
	.endif
	invoke DeleteObject,[hUrlBrush]
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
	invoke SendMessage,[hwnd],WM_NCLBUTTONDOWN,2,0
	jmp .processed
.syscommand:
	.if [wparam]=IDM_SITE
		 invoke ShellExecuteA,0,'Open',site,0,0,SW_SHOW
		 jmp .processed
	.elseif [wparam]=IDM_UPDATE
		 invoke CreateThread,0,0,getversion,[hwnd],0,0
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
		stdcall getwinfullscr
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
	.if [wparam]=IDK_MONEY
		invoke CreateThread,0,0,check_money,[hwnd],0,0
	.endif
	jmp .processed
.wmsize_tray:
	.if [wparam]=SIZE_MINIMIZED
 .wmsize_tray2:
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
		invoke Shell_NotifyIconA,NIM_ADD,notes
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
			invoke Shell_NotifyIconA,NIM_MODIFY,notes2
			mov [firstrun],1
		.endif
	.endif
	jmp .processed
.wmmax:
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
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke GetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2,10
		.if eax=0
			stdcall DialogPOPUP,stat_err_money,0
			mov word [notify_flag2],30h
			invoke SetDlgItemTextA,[hwnd],IDE_NOTIFYMONEY,notify_flag2
		.endif
		invoke RegSetValueEx,[HKey],key_name3,0,REG_SZ,notify_flag2,10
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_UNCHECKED,0
		.else
			stdcall setstat,[hwnd],stat_ok
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
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name3
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_ONOFF,BM_SETCHECK,BST_CHECKED,0
		.else
			invoke GetDlgItem,[hwnd],IDE_NOTIFYMONEY
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],ID_APPLY
			invoke EnableWindow,eax,0
			invoke GetDlgItem,[hwnd],ID_SOUND
			invoke EnableWindow,eax,0
			stdcall setstat,[hwnd],stat_ok
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
   .get_info:
	invoke CreateThread,0,0,@f,[hwnd],0,0
	invoke CloseHandle,eax
	jmp .processed
	@@:
	mov eax,dword [esp+4]
	local hwnd:DWORD
	mov [hwnd],eax
	invoke WaitForSingleObject,[gcontent_mutex],0
	.if eax=WAIT_TIMEOUT
		jmp @f
	.endif
	stdcall get_content,[hwnd],0
	.if eax=1
		invoke SendDlgItemMessageA,[hwnd],ID_LOGIN,BM_CLICK,0,0
	.endif
	invoke ReleaseMutex,[gcontent_mutex]
	@@:
	invoke ExitThread,0
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
			stdcall DialogPOPUP,stat_err_autorun1,0
			stdcall setstat,[hwnd],stat_err_autorun1
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke GetModuleFileNameA,[mhandle],buffer,MAX_PATH+10
		add eax,buffer
		invoke lstrcpyA,eax,command
		invoke lstrlenA,buffer
		invoke RegSetValueEx,[HKey],key_name,0,REG_SZ,buffer,eax
		.if eax<>0
			stdcall DialogPOPUP,stat_err_autorun1,0
			stdcall setstat,[hwnd],stat_err_autorun1
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_UNCHECKED,0
		.else
			stdcall setstat,[hwnd],stat_ok
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_CHECKED,0
			stdcall DialogPOPUP,stat_err_autorun2,0
			stdcall setstat,[hwnd],stat_err_autorun2
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name
		.if eax<>0
			stdcall DialogPOPUP,stat_err_autorun2,0
			stdcall setstat,[hwnd],stat_err_autorun2
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,BST_CHECKED,0
		.else
			stdcall setstat,[hwnd],stat_ok
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
.update1:
	invoke IsDlgButtonChecked,[hwnd],ID_UPDATE
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name4,0,REG_BINARY,flagg,1
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_UNCHECKED,0
		.else
			invoke SetTimer,[hwnd],TIMER_UPDATE,TIME_UPDATE,0
			stdcall setstat,[hwnd],stat_ok
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_CHECKED,0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name4
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_UPDATE,BM_SETCHECK,BST_CHECKED,0
		.else
			invoke KillTimer,[hwnd],TIMER_UPDATE
			stdcall setstat,[hwnd],stat_ok
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
.sethotkey:
	invoke IsDlgButtonChecked,[hwnd],ID_HOTKEY
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name5,0,REG_BINARY,flagg,1
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_UNCHECKED,0
		.else
			invoke RegisterHotKey,[hwnd],IDK_MONEY,MOD_ALT,4Dh
			stdcall setstat,[hwnd],stat_ok
		.endif
	.else
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_CHECKED,0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name5
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_HOTKEY,BM_SETCHECK,BST_CHECKED,0
		.else
			invoke UnregisterHotKey,[hwnd],IDK_MONEY
			mov [hkey_flag],0
			stdcall setstat,[hwnd],stat_ok
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
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name2,0,REG_BINARY,flagg,1
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,BST_UNCHECKED,0
		.else
			mov [notify_flag],1
			stdcall setstat,[hwnd],stat_ok
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
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name2
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_NOTIFY,BM_SETCHECK,BST_CHECKED,0
		.else
			mov [notify_flag],0
			stdcall setstat,[hwnd],stat_ok
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
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_UNCHECKED,0
			jmp .processed
		.endif
		invoke RegSetValueEx,[HKey],key_name6,0,REG_BINARY,flagg,1
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_UNCHECKED,0
		.else
			mov [sound_flag],1
			stdcall setstat,[hwnd],stat_ok
		.endif
	.else
		mov [sound_flag],0
		invoke RegOpenKeyExA,key_root,subkey2,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_CHECKED,0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			jmp .processed
		.endif
		invoke RegDeleteValueA,[HKey],key_name6
		.if eax<>0
			stdcall DialogPOPUP,stat_err_reg,0
			stdcall setstat,[hwnd],stat_err_reg
			invoke SendDlgItemMessage,[hwnd],ID_SOUND,BM_SETCHECK,BST_CHECKED,0
		.else
			stdcall setstat,[hwnd],stat_ok
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

proc _InterlockedIncrement mem,size
	; 0 - byte
	; 1 - word
	; 2 - dword

	mov ecx,[mem]
	xor eax,eax
	inc eax
	.if [size]=0
		lock xadd byte [ecx],al
	.elseif [size]=1
		lock xadd word [ecx],ax
	.else
		lock xadd dword [ecx],eax
	.endif
	ret
endp

proc _InterlockedDecrement mem,size
	; 0 - byte
	; 1 - word
	; 2 - dword

	mov ecx,[mem]
	xor eax,eax
	dec eax
	.if [size]=0
		lock xadd byte [ecx],al
	.elseif [size]=1
		lock xadd word [ecx],ax
	.else
		lock xadd dword [ecx],eax
	.endif
	ret
endp

include 'getversion.inc'

include 'getcontent.inc'

include 'getlogin.inc'

include 'parser.inc'

include 'stat.inc'

include 'popup.inc'

include 'check.inc'

include 'sound.inc'

include 'about.inc'

include 'getwinfullscr.inc'

include 'tariff.inc'

include 'https_request.inc'

include 'license.inc'

include 'snow.inc'

include 'post.inc'

include 'crc32.inc'

include 'tea.asm'

crc32code_end:
CRC32 crc32code,start,crc32code_end-start
XOR start,crc32code_end-start