format pe gui 4.0
include 'C:\Documents and Settings\Администратор\Мои документы\fasmw16905\INCLUDE\WIN32AX.INC'

macro invokex proc,[arg]
{
reverse pushd <arg>
common call proc
}

IDD_MAIN = 666
IDD_LOGIN = 667
ID_GET = 1
ID_LOGIN = 2
IDL_MONEY = 3
IDL_DATA = 4
IDL_DOGOVOR = 5
IDL_TARIF = 6
ID_AUTORUN = 7
ID_GETMON = 8
IDE_GETMON = 9
ID_UPDOWN = 10
ID_OPTIONS = 11
ID_SAVE = 1
IDE_LOGIN = 2
IDE_PASS = 3

BUFFER_SIZE = 50000
UDM_SETRANGE32 = 046fh

.data
fail_teg db 'alert ("*");',0
fail_mess db 'Error WWW-00013: Неверный пароль',0
fail_mess2 db 'Error WWW-00013: Неверный логин',0
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
buffer rb MAX_PATH
vsize dd 64
mhandle dd ?
buff_pass dd ?
buff_login dd ?
buff_trash dd ?
HKey dd ?
lpType dd ?
.code
start:
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
	cmp [msg],WM_INITDIALOG
	je .wminitdialog
	cmp [msg],WM_COMMAND
	je .wmcommand
	cmp [msg],WM_LBUTTONDOWN
	je .move
	cmp [msg],WM_CLOSE
	je exit
	xor eax,eax
	jmp .finish
    .wminitdialog:
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
	invoke SetWindowPos,[hwnd],HWND_TOPMOST,0,0,0,0,SWP_NOMOVE+SWP_NOSIZE
	invoke SetWindowLongA,[hwnd],GWL_EXSTYLE,WS_EX_LAYERED,0
	invoke SetLayeredWindowAttributes,[hwnd],0,220,LWA_ALPHA
	jmp .processed
    .wmcommand:
	cmp [wparam],BN_CLICKED shl 16 + ID_GET
	je .get_info
	cmp [wparam],BN_CLICKED shl 16 + ID_LOGIN
	je .get_login
	cmp [wparam],BN_CLICKED shl 16 + ID_OPTIONS
	je .options
	cmp [wparam],BN_CLICKED shl 16 + ID_AUTORUN
	je .autorun
	jmp .processed
    .processed:
	mov eax,1
    .finish:
	pop edi esi ebx
	ret
    .move:
	invoke SendMessage,[hwnd],0a1h,2,0
	jmp .processed
   .get_info:
	invoke RegOpenKeyExA,HKEY_CURRENT_USER,subkey2,0,KEY_READ,HKey
	.if eax<>0
		jmp .get_login
	.endif
	invoke GlobalAlloc,GPTR,64
	mov [buff_login],eax
	invoke RegQueryValueExA,[HKey],key1,0,lpType,[buff_login],vsize
	.if eax<>0
		invoke GlobalFree,[buff_login]
		invoke RegCloseKey,[HKey]
		jmp .get_login
	.endif
	invoke GlobalAlloc,GPTR,64
	mov [buff_pass],eax
	invoke RegQueryValueExA,[HKey],key2,0,lpType,[buff_pass],vsize
	.if eax<>0
		invoke GlobalFree,[buff_pass]
		invoke GlobalFree,[buff_login]
		invoke RegCloseKey,[HKey]
		jmp .get_login
	.endif
	invoke RegCloseKey,[HKey]
	invoke GetDlgItem,[hwnd],ID_GET
	invoke EnableWindow,eax,0
	invoke GetDlgItem,[hwnd],ID_LOGIN
	invoke EnableWindow,eax,0
	invoke CreateThread,0,0,get_content,[hwnd],0,0,0
	jmp .processed
   .get_login:
	invoke ShowWindow,[hwnd],SW_HIDE
	invoke DialogBoxParamA,[mhandle],IDD_LOGIN,HWND_DESKTOP,getlogin,0
	invoke ShowWindow,[hwnd],SW_SHOW
	jmp .processed
  .options:
	invoke IsDlgButtonChecked,[hwnd],ID_OPTIONS
	.if eax=BST_CHECKED
		invoke SetWindowPos,[hwnd],-1,0,0,321,368,2
	.else
		invoke SetWindowPos,[hwnd],-1,0,0,321,272,2
	.endif
	jmp .processed
  .autorun:
	invoke IsDlgButtonChecked,[hwnd],ID_AUTORUN
	.if eax=BST_CHECKED
		invoke RegOpenKeyEx,HKEY_CURRENT_USER,subkey,0,KEY_WRITE,HKey
		.if eax<>0
			invoke MessageBoxA,[hwnd],'Ошибка добавления в автозапуск',0,0
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,0,0
			jmp .processed
		.endif
		invoke GetModuleFileNameA,[mhandle],buffer,MAX_PATH
		invoke lstrlenA,buffer
		inc eax
		invoke RegSetValueEx,[HKey],key_name,0,REG_SZ,buffer,eax
		.if eax<>0
			invoke MessageBoxA,[hwnd],'Ошибка добавления в автозапуск',0,0
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,0,0
		.endif
	.else
		invoke RegOpenKeyExA,HKEY_LOCAL_MACHINE,subkey,0,KEY_ALL_ACCESS,HKey
		.if eax<>0
			invoke MessageBoxA,[hwnd],'Ошибка удаления из автозапуска',0,0
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,0,0
			jmp .processed
		.endif
		invoke RegDeleteKeyA,[HKey],'fora'
		.if eax<>0
			invoke MessageBoxA,[hwnd],'Ошибка удаления из автозапуска',0,0
			invoke SendDlgItemMessage,[hwnd],ID_AUTORUN,BM_SETCHECK,0,0
		.endif
	.endif
	invoke RegCloseKey,[HKey]
	jmp .processed
endp

proc get_content hwnd
	local hsock:DWORD
	local WSA:DWORD
	local port:DWORD
	local sin:DWORD
	local str:DWORD
	local buf:DWORD
	local qmem:DWORD
	local size:DWORD
	local time:DWORD
	;;;;; sockaddr_in
	sin_family equ 0   ;dw
	sin_port   equ 2   ;dw
	sin_addr   equ 4   ;dd
	sin_zero   equ 8   ;db
	;;;;; WSADATA
	wVersion       equ 0	;dw
	wHighVersion   equ 2	;dw
	szDescription  equ 4	;db
	szSystemStatus equ 261	;db
	iMaxSockets    equ 390	;dw
	iMaxUdpDg      equ 392	;dw
	_padding_      equ 394	;db
	lpVendorInfo   equ 396	;dd
	;;;
	mov [port],7777
	invoke GlobalAlloc,GPTR,sizeof.sockaddr_in
	test eax,eax
	je err_mem
	mov [sin],eax
	invoke GlobalAlloc,GPTR,sizeof.WSADATA
	test eax,eax
	je err_mem
	mov [WSA],eax
	invoke MessageBoxA,0,'1',0,0
	invoke WSAStartup,101h,[WSA]
	invoke MessageBoxA,0,'2',0,0
	invoke socket,AF_INET,SOCK_STREAM,0
	cmp eax,-1
	je err_connect
	mov [hsock],eax
	invoke MessageBoxA,0,'3',0,0
	invoke htons,[port]
	mov esi,[sin]
	mov word [esi+sin_port],ax
	mov word [esi+sin_family],AF_INET
	invoke MessageBoxA,0,'4',0,0
	invoke gethostbyname,host
	test eax,eax
	je err_connect_ip
	invoke MessageBoxA,0,'5',0,0
	mov eax,[eax+12]	 ;h_addr_list список указателей на IP
	mov eax,[eax+8] 	   ;берём первый IP
	mov esi,[sin]
	mov dword [esi+sin_addr],eax
	invoke connect,[hsock],[sin],10h
	test eax,eax
	jne err_connect
	invoke MessageBoxA,0,'6',0,0
	invoke lstrlenA,[buff_login]
	mov [size],eax
	invoke lstrlenA,[buff_pass]
	add [size],eax
	invoke lstrlenA,post_login
	add [size],eax
	invoke lstrlenA,post_pass
	add [size],eax
	invoke lstrlenA,post_
	add [size],eax
	invoke GlobalAlloc,GPTR,512
	mov [qmem],eax
	push eax
	invoke wsprintfA,[qmem],qry,[size]
	invoke lstrlenA,[qmem]
	add eax,[qmem]
	invoke lstrcpyA,eax,[buff_login]
	invoke lstrlenA,[qmem]
	add eax,[qmem]
	invoke lstrcpyA,eax,post_pass
	invoke lstrlenA,[qmem]
	add eax,[qmem]
	invoke lstrcpyA,eax,[buff_pass]
	invoke lstrlenA,[qmem]
	add eax,[qmem]
	invoke lstrcpyA,eax,post_
	invoke lstrlenA,[qmem]
	push eax
	invoke send,[hsock],[qmem],eax,0
	pop ecx
	cmp eax,ecx
	jne err_no_send
	invoke GlobalAlloc,GPTR,BUFFER_SIZE
	test eax,eax
	je err_mem
	mov [buf],eax
	mov edi,[buf]
      sread3: ;цикл чтения сокета
	invoke recv,[hsock],edi,256,0 ;читаем по 256 байт за раз
	cmp eax,-1 ;ошибка чтения?
	je err_no_send ;если да, выведем ошибку

	add edi,eax ;перемещаем указатель на память на количество считанных байт

	mov edx,edi ;определяем, не возникнет ли переполнение нашей памяти на следующем шаге чтения
	sub edx,[buf]
	cmp edx,BUFFER_SIZE-256 ;если возникнет, то выведем ошибку. Если такое часто возникает, то просто нужно увеличить BUFFER_SIZE
	jae err_no_send

	cmp eax,0 ;все ли считано?
	jne sread3 ;если нет, то продолжаем чтение
	invokex preg_match,fail_teg,[buf],fail_mess,0
	.if eax=1
		jmp err_login
	.endif
	invokex preg_match,fail_teg,[buf],fail_mess2,0
	.if eax=1
		jmp err_login
	.endif
	invoke GlobalAlloc,GPTR,64
	test eax,eax
	je err_mem
	mov [str],eax
	invokex preg_match,preg1,[buf],[str],1
	invoke SetDlgItemTextA,[hwnd],IDL_MONEY,[str]
	invokex preg_match,preg2,[buf],[str],1
	invoke SetDlgItemTextA,[hwnd],IDL_DATA,[str]
	invokex preg_match,preg3,[buf],[str],1
	invoke SetDlgItemTextA,[hwnd],IDL_DOGOVOR,[str]
	invokex preg_match,preg4,[buf],[str],1
	invoke SetDlgItemTextA,[hwnd],IDL_TARIF,[str]
	invoke GlobalFree,[str]
  disconnect:
	invoke GlobalFree,[buf]
	invoke GlobalFree,[buff_pass]
	invoke GlobalFree,[buff_login]
	invoke closesocket,[hsock]
	invoke WSACleanup
	invoke GlobalFree,[qmem]
	invoke GlobalFree,[WSA]
	invoke GlobalFree,[sin]
	invoke GetDlgItem,[hwnd],ID_GET
	invoke EnableWindow,eax,1
	invoke GetDlgItem,[hwnd],ID_LOGIN
	invoke EnableWindow,eax,1
	invoke ExitThread,0
	ret

 err_no_send:
	invoke MessageBoxA,[hwnd],'Не могу подключиться',0,MB_ICONINFORMATION+MB_OK
	jmp disconnect
 err_connect_ip:
	invoke MessageBoxA,[hwnd],'Не могу получить IP адрес',0,MB_ICONINFORMATION+MB_OK
	jmp disconnect
 err_connect:
	invoke MessageBoxA,[hwnd],'Ошибка подключения',0,MB_ICONINFORMATION+MB_OK
	jmp disconnect
     err_mem:
	invoke MessageBoxA,[hwnd],'Не могу выделить память',0,MB_ICONINFORMATION+MB_OK
	jmp disconnect
 err_login:
	invoke MessageBoxA,[hwnd],'Необходимо указать рабочий аккаунт',0,MB_ICONINFORMATION+MB_OK
	jmp disconnect
endp

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
	invoke GlobalAlloc,GPTR,eax
	mov [buff_login],eax
	invoke GetDlgItemTextA,[hwnd],IDE_LOGIN,eax
	invoke GetDlgItem,[hwnd],IDE_PASS
	invoke GetWindowTextLengthA,eax
	.if eax=0
		invoke MessageBoxA,[hwnd],'Введите пароль',0,MB_ICONINFORMATION+MB_OK
		invoke GlobalFree,[buff_login]
		jmp .processed
	.endif
	invoke GlobalAlloc,GPTR,eax
	mov [buff_pass],eax
	invoke GetDlgItemTextA,[hwnd],IDE_PASS,eax
	invoke RegOpenKeyEx,HKEY_CURRENT_USER,subkey2,0,KEY_WRITE,HKey
	.if eax<>0
		invoke RegCreateKeyA,HKEY_CURRENT_USER,subkey2,HKey
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
	invoke GlobalFree,[buff_pass]
	invoke GlobalFree,[buff_login]
     .exit:
	invoke RegCloseKey,[HKey]
	invoke EndDialog,[hwnd],0,0
	ret
endp

proc preg_match mat, str, str2, bool
	local mem:DWORD
	local start1:DWORD
	.if [bool]=0
		invoke lstrlenA,[str]
		invoke GlobalAlloc,GPTR,eax
	.else
		mov eax,[str2]
	.endif
	mov [mem],eax
	mov ecx,[mem]
	mov edx,[mat]
	mov ebx,[str]
	mov al, byte [edx]
     pmmetka1:
	.if byte [ebx]=al & byte [edx]<>0 & al<>'*'
		inc edx
		inc ebx
		mov al, byte [edx]
		jmp pmmetka1
	.elseif byte [edx]<>0
		.if al='*'
			mov [start1],1
			inc edx
			mov al, byte [edx]
			jmp pmmetka1
		.elseif [start1]=1
		push eax
		mov al,byte [ebx]
		mov byte [ecx],al
		pop eax
		inc ecx
		inc ebx
		jmp pmmetka1
		.elseif byte [ebx]<>0
		inc ebx
		jmp pmmetka1
		.endif
	.endif
	.if [bool]=0
		invoke lstrcmpA,[mem],[str2]
		.if eax=-1
			mov eax,0
		.else
			mov eax,1
		.endif
		push eax
		invoke GlobalFree,[mem]
		pop eax
	.endif
	ret
endp

.end start
section '.res' readable data resource from 'res.res'