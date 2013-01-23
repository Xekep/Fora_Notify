proc scrc32 hwnd,msg,timer_id,time
	; Проверка контрольной суммы текст строк
	fake_ff
	mov eax,-1
	mov esi,crc32strings_start
	mov ecx,crc32strings_end-crc32strings_start
	call .crc32
	cmp [crc32str],eax
	jne .err1
	; Проверка контрольной суммы ресурсов
	mov eax,-1
	mov esi,crc32res_start
	mov ecx,crc32res_end-crc32res_start
	call .crc32
	cmp [crc32res],eax
	jne .err1
	; Проверка контрольной суммы кода
	mov eax,-1
	mov esi,start
	mov ecx,crc32code_end-start
	call .crc32
	cmp [crc32code],eax
	jne .err1
	; Проверка контрольной суммы TLS CallBack
	mov eax,-1
	mov esi,crc32adebug_start
	mov ecx,crc32adebug_end-crc32adebug_start
	call .crc32
	cmp [crc32adebug],eax
	jne .err1
	fake_ff
	mov ecx,(.funcs_end-.funcs)/4
	mov ebx,.funcs
      .loop_:
	mov eax,[ebx]
	neg eax
	mov eax,[eax]
	cmp byte [eax],0CCh
	je .err1
	cmp byte [eax],0CDh
	je .err1
	add ebx,4
	loop .loop_
	ret
	.funcs dd -InternetOpenA,-InternetReadFile,-InternetCloseHandle,-InternetConnectA,-HttpSendRequestA,\
		  -HttpOpenRequestA,-InternetSetOptionA,-InternetQueryOptionA,-send,-recv,-WSAStartup,-WSACleanup,\
		  -closesocket,-connect,-htons,-gethostbyname,-socket,-ShellExecuteA,-Shell_NotifyIconA,-RegCloseKey,\
		  -RegOpenKeyExA,-RegCreateKeyA,-RegDeleteValueA,-RegQueryValueExA,-RegSetValueExA,-CreateProcessA,\
		  -CreateThread,-CreateToolhelp32Snapshot,-ExitProcess,-ExitThread,-GetCommandLineA,-GetCurrentProcessId,\
		  -GetCurrentThreadId,-GetLastError,-GetModuleFileNameA,-GetModuleHandleA,-GetProcAddress,-GetStartupInfoA,\
		  -GetSystemTime,-GetVolumeInformationA,-GetWindowsDirectoryA,-GlobalAlloc,-GlobalFree,-LoadLibraryA,-MulDiv,\
		  -MultiByteToWideChar,-OpenProcess,-OutputDebugStringA,-Process32First,-Process32Next,-ReleaseMutex,-RtlZeroMemory,\
		  -Sleep,-TerminateProcess,-VirtualProtect,-WaitForSingleObject,-WideCharToMultiByte,-lstrcatA,-lstrcmpA,-lstrcmpiA,\
		  -lstrcpyA,-lstrlenA,-MessageBoxTimeoutA,-AppendMenuA,-BeginPaint,-CallNextHookEx,-CallWindowProcA,-CreatePopupMenu,\
		  -CreateWindowExA,-DestroyWindow,-DialogBoxIndirectParamA,-DialogBoxParamA,-DrawTextA,-EnableMenuItem,-EnableWindow,\
		  -EndDialog,-EndPaint,-FindWindowA,-GetClientRect,-GetCursorPos,-GetDC,-GetDlgCtrlID,-GetDlgItem,-GetDlgItemTextA,\
		  -GetForegroundWindow,-GetSysColor,-GetSystemMenu,-GetSystemMetrics,-GetWindowLongA,-GetWindowPlacement,\
		  -GetWindowRect,-GetWindowTextLengthA,-InvalidateRect,-IsDlgButtonChecked,-KillTimer,-LoadBitmapA,-LoadCursorA,\
		  -LoadIconA,-MessageBoxA,-MoveWindow,-PostMessageA,-PostQuitMessage,-RegisterHotKey,-RegisterWindowMessageA,\
		  -ReleaseDC,-SendDlgItemMessageA,-SendMessageA,-SetActiveWindow,-SetCursor,-SetDlgItemTextA,-SetForegroundWindow,\
		  -SetLayeredWindowAttributes,-SetTimer,-SetWindowLongA,-SetWindowPos,-SetWindowTextA,-SetWindowsHookExA,-ShowWindow,\
		  -SystemParametersInfoA,-TrackPopupMenu,-UnhookWindowsHookEx,-UnregisterHotKey,-UpdateWindow,-ValidateRect,-wsprintfA,\
		  -BitBlt,-CreateCompatibleBitmap,-CreateCompatibleDC,-CreateFontA,-CreateFontW,-CreateSolidBrush,-DeleteDC,-DeleteObject,\
		  -GetDeviceCaps,-GetTextMetricsA,-SelectObject,-SetBkColor,-SetBkMode,-SetPixelV,-SetTextColor
	.funcs_end dd 0
	.err1:
	invokex KillTimer,[hwnd],[timer_id]
	xor eax,eax
	push eax
	push eax
	push 2000
	push eax
	push eax
	push eax
	push fail_X
	push eax
	mov eax,-PostQuitMessage
	neg eax
	push dword [eax]
	mov eax,-MessageBoxTimeoutA
	neg eax
	push dword [eax]
	push main-1
	retn
  .crc32:
     .ComputeLoop:
	xor ebx,ebx
	xor al,[esi]
	mov bl,al
	shr eax,8
	xor eax,dword [4*ebx+CrtTable]
	inc esi
	loop .ComputeLoop
	xor eax,0FFFFFFFFh
	retn
endp