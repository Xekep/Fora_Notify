data	    9
	    dd a ; StartAddressOfRawData
	    dd a ; EndAddressOfRawData
	    dd a ; AddressOfIndex
	    dd c ; AddressOfCallBacks
a	    dd 0 ; Size of Zero Fill
b	    dd ? ; Characteristics
c	    dd callback ; Array Of Callbacks
d	    dd 0
	    dd 0	; NULL - end of Array Of Callbacks
end data

TH32CS_SNAPPROCESS = 2
struct ProcessEntry32
    dwSize		dd ?
    cntUsage		dd ?
    th32ProcessID	dd ?
    th32DefaultHeapID	dd ?
    th32ModuleID	dd ?
    cntThreads		dd ?
    th32ParentProcessID dd ?
    pcPriClassBase	dd ?
    dwFlags		dd ?
    szExeFile		rb MAX_PATH
ends

proc	callback handle,reason,reserved
	local .hMutex:DWORD
	local .s13:STARTUPINFO
	local .s14:PROCESS_INFORMATION

	cmp [reason],DLL_PROCESS_ATTACH
	jnz .end!
	invokex GetModuleHandleA,0
	mov ecx,eax
	add ecx,[ecx+3Ch]
	mov ecx,[ecx+28h]
	add eax,ecx
	cmp eax,start
	jne .end!
	xor ebx,ebx
	if DEBUG=0
		fake_ff
		invokex CreateMutexA,ebx,ebx,trayname
		mov [.hMutex],eax
		invokex GetLastError
		.if eax<>0b7h
			lea eax,[.s13]
			invokex GetStartupInfoA,eax
			invokex GetCommandLineA
			lea edi,[.s13]
			lea esi,[.s14]
			invokex CreateProcessA,ebx,eax,ebx,ebx,ebx,ebx,ebx,ebx,edi,esi
			invokex WaitForSingleObject,[.s14.hProcess],-1
			invokex CloseHandle,[.hMutex]
			invokex ExitProcess,0
		.endif
		; Убийство родительского процесса
		invokex CreateToolhelp32Snapshot,TH32CS_SNAPPROCESS,ebx
		cmp eax,-1
		je @f
		mov edi,eax
		invokex GetCurrentProcessId
		stdcall .kill,edi,eax
		@@:
		; Убийство мьютекса
		invokex CloseHandle,[.hMutex]
		mov eax,[fs:30h] ;указатель на PEB
		cmp byte [eax+2], 0 ;проверка флага BeingDebugged
		jne .end!

		invokex GetModuleHandleA,'NTDLL.DLL'
		invokex GetProcAddress,eax,'NtQueryInformationProcess'
		lea ecx,[.s14]
		stdcall eax,-1,7,ecx,4,0
		cmp dword [.s14],0
		jne .end!
	end if
	mov [d],.callback2
.end!:
	ret
endp

proc .callback2 handle,reason,reserved
	local lpflOldProtec:DWORD

	cmp [reason],DLL_PROCESS_ATTACH
	jne @f
	mov [d],0
	invokex GetModuleHandleA,0
	mov [mhandle],eax
	invokex LoadLibraryA,'riched32.dll'
	invokex LoadLibraryA,'msvcrt.dll'
	invokex GetProcAddress,eax,time
	mov [time],eax
	invokex LoadLibraryA,'MSIMG32.dll'
	invokex GetProcAddress,eax,TransparentBlt
	mov dword [TransparentBlt],eax
	if DEBUG=0
		stdcall .xor,crc32strings_start,crc32strings_end-crc32strings_start
		lea eax,[lpflOldProtec]
		invokex VirtualProtect,start,crc32code_end-start+4,PAGE_EXECUTE_READWRITE,eax
		stdcall .xor,start,crc32code_end-start
		lea eax,[lpflOldProtec]
		invokex VirtualProtect,start,crc32code_end-start+4,[lpflOldProtec],eax
		invokex SetTimer,0,TIMER_CRC32,TIME_CRC32,scrc32
	end if
	; генерация таблицы
	;CRC32TableGen CrtTable
	@@:
	ret
endp

proc .xor data,iter
	mov ecx,[iter]
	mov eax,[data]
	@@:
	xor dword [eax+ecx-1],0123FFFFFh
	loop @b
	ret
endp

proc .kill hSnapshot,pid
	local pentry:ProcessEntry32
	local result dd 0

	lea eax,[pentry]
	invokex RtlZeroMemory,eax,sizeof.ProcessEntry32
	mov [pentry.dwSize],sizeof.ProcessEntry32
	lea eax,[pentry]
	invokex Process32First,[hSnapshot],eax
	jmp .next
	@@:
	lea eax,[pentry]
	invokex Process32Next,[hSnapshot],eax
      .next:
	mov ecx,[pid]
	.if [pentry.th32ProcessID]=ecx & eax<>0
		invokex OpenProcess,PROCESS_TERMINATE,0,[pentry.th32ParentProcessID]
		test eax,eax
		je .exit
		invokex TerminateProcess,eax,0
		mov [result],1
		jmp @b
	.elseif eax=1
		; Убийство отладчиков
		lea eax,[pentry.szExeFile]
		stdcall .TerminateDBGProcess,eax,[pentry.th32ProcessID]
		jmp @b
	.endif
      .exit:
	invokex CloseHandle,[hSnapshot]
	mov eax,[result]
	ret
endp

proc .TerminateDBGProcess pname,pid
	pushad
	mov ecx,20
	mov esi,.parray
	@@:
	push ecx
	push esi
	invokex lstrcmpiA,dword [esi],[pname]
	.if eax=0
		invokex OpenProcess,PROCESS_TERMINATE,0,[pid]
		invokex TerminateProcess,eax,0
		add esp,8
		popad
		xor eax,eax
		inc eax
		ret
	.endif
	pop esi
	pop ecx
	add esi,4
	loop @b
	popad
	xor eax,eax
	ret
	.parray dd .olly,.hassistant,.cengine,.artmoney,\
		   .winhex,.hiew,.hdasm,.w32dasm,.ida,.ida64,.petools,\
		   .lordpe,.peid,.rdgpd,.die,.imprec,.windbg,.imdbg,\
		   .chimprec64,.chimprec,0

	.olly db 'OLLYDBG.EXE',0
	.hassistant db 'HEXASSISTANT.EXE',0
	.cengine db 'CHEAT ENGINE.EXE',0
	.artmoney db 'ARTMONEY.EXE',0
	.winhex db 'WINHEX.EXE',0
	.hiew db 'HIEW.EXE',0
	.hdasm db 'HDASM.EXE',0
	.w32dasm db 'W32DASM.EXE',0
	.ida db 'IDAG.EXE',0
	.ida64 db 'IDAG64.EXE',0
	.petools db 'PETOOLS.EXE',0
	.lordpe db 'LORDPE.EXE',0
	.peid db 'PEID.EXE',0
	.rdgpd db 'RDG PACKER DETECTOR.EXE',0
	.die db 'DIE.EXE',0
	.imprec db 'IMPORTREC.EXE',0
	.windbg db 'WINDBG.EXE',0
	.imdbg db 'IMMUNITYDEBUGGER.EXE',0
	.chimprec64 db 'CHimpREC 64.exe',0
	.chimprec db 'CHimpREC.exe',0
endp