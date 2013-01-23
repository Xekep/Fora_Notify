macro CRC32 name,start,len
{
if DEBUG=0
	local CrtTable
	CrtTable:
	;virtual ta CrtTable
	db 00h,00h,00h,00h,51h,12h,0C2h,4Ah,4Bh,98h,83h,3Bh,0C3h,53h,2Dh,2Bh,96h,30h,07h,77h,45h,0F4h,77h,7Dh,86h,0A7h,5Ah,56h,50h,68h,0F1h,54h,\
	    19h,0C4h,6Dh,07h,1Ah,8Ah,41h,71h,55h,0D7h,0AEh,2Eh,0AEh,6Bh,31h,39h,0A7h,6Bh,0FDh,65h,4Ch,4Dh,62h,23h,0C7h,96h,41h,4Fh,0F4h,1Ch,71h,2Dh,\
	    32h,88h,0DBh,0Eh,0C7h,22h,0C5h,3Dh,29h,2Eh,77h,1Eh,0A2h,0A9h,0E1h,7Ah,0AAh,0AEh,5Dh,5Dh,14h,0E6h,0B5h,37h,5Ch,0D7h,62h,72h,4Bh,0C2h,0C1h,02h,\
	    0DDh,0A8h,84h,4Ch,8Ch,0BAh,46h,06h,98h,9Ah,0C4h,46h,44h,32h,0CDh,41h,0FBh,0BCh,9Fh,17h,0Eh,36h,0B6h,7Fh,0E8h,39h,0E2h,5Ah,4Dh,0E4h,7Ah,17h,\
	    64h,10h,0B7h,1Dh,0E4h,83h,21h,5Ch,8Eh,45h,8Ah,7Bh,24h,0Eh,0BBh,2Ch,52h,5Ch,0EEh,3Ch,8Fh,0A7h,96h,0Ah,0C9h,0A0h,0F7h,30h,69h,85h,63h,33h,\
	    8Fh,0F4h,6Ah,70h,0EBh,9Fh,46h,44h,28h,0CCh,6Bh,6Fh,0E5h,0F3h,0B2h,02h,31h,5Bh,0FAh,12h,0D9h,0ABh,0DFh,5Fh,96h,84h,83h,05h,0A9h,08h,0F9h,43h,\
	    2Bh,4Ch,0B6h,09h,0D9h,83h,2Eh,63h,18h,75h,8Dh,0Ch,4Ch,0D9h,1Ah,3Dh,03h,0DEh,0B7h,5Bh,73h,7Dh,91h,47h,0E3h,98h,0FAh,63h,15h,0F3h,24h,21h,\
	    0F6h,79h,3Fh,2Fh,0E8h,36h,12h,3Ah,79h,0DEh,0A9h,25h,9Ch,77h,59h,1Eh,0FFh,79h,0F3h,73h,5Fh,24h,74h,35h,9Ah,0C8h,0F5h,2Eh,7Fh,54h,10h,42h,\
	    0C8h,20h,6Eh,3Bh,83h,0B8h,0EDh,00h,4Fh,5Dh,0EFh,5Fh,75h,0F9h,15h,6Ch,79h,10h,58h,35h,0A0h,5Dh,87h,46h,48h,1Ch,76h,59h,0A8h,5Ah,3Bh,61h,\
	    0A4h,0B8h,0DCh,79h,4Dh,3Eh,0C0h,03h,1Eh,4Fh,2Dh,15h,38h,5Bh,36h,4Eh,92h,41h,0EFh,61h,48h,31h,0D7h,45h,0D2h,0Ah,0C7h,66h,69h,49h,0F4h,04h,\
	    7Dh,0D4h,0DAh,1Ah,0BCh,0CDh,51h,75h,9Bh,0F5h,0A7h,4Fh,0D5h,11h,1Ch,59h,0E1h,36h,0DEh,66h,0D4h,6Bh,2Fh,47h,0CAh,0E7h,65h,05h,15h,9Ch,86h,29h,\
	    62h,0B6h,0F4h,25h,2Ah,0FDh,56h,02h,0EFh,5Ah,2Ah,20h,51h,0C1h,0E9h,60h,2Ch,09h,07h,0Bh,64h,8Ch,54h,3Ch,8Bh,4Fh,5Bh,72h,6Bh,09h,16h,4Ah,\
	    56h,98h,6Ch,13h,0B2h,0DBh,0D0h,51h,25h,09h,0CBh,57h,0E3h,0C9h,12h,1Bh,30h,0EAh,1Ah,19h,0C4h,3Fh,15h,31h,98h,0B2h,35h,7Ah,58h,3Fh,0AFh,0Ah,\
	    0BFh,1Eh,70h,69h,77h,0E8h,1Fh,5Ah,5Bh,0E1h,18h,51h,73h,0C3h,0B5h,75h,0CDh,65h,9Bh,54h,95h,2Dh,0D7h,7Bh,2Ah,0E6h,49h,42h,0B4h,0BBh,32h,71h,\
	    0ECh,0F3h,7Eh,5Eh,0F9h,43h,53h,6Ah,0D0h,6Dh,24h,74h,6Ah,54h,24h,08h,0F2h,0BCh,53h,4Bh,0EEh,70h,0FBh,47h,38h,0EFh,0B2h,3Ch,0D5h,7Eh,0BEh,51h,\
	    0BDh,0E1h,0BCh,14h,7Eh,06h,15h,4Dh,0BEh,48h,0E8h,6Ah,0B1h,87h,0A4h,58h,34h,91h,0EBh,5Dh,2Fh,14h,0D7h,07h,3Bh,0A9h,11h,7Dh,11h,80h,94h,41h,\
	    90h,41h,0DCh,76h,14h,52h,31h,45h,06h,71h,0DBh,01h,20h,2Eh,0BBh,42h,2Fh,88h,34h,26h,5Ch,0D2h,32h,52h,95h,0E3h,26h,6Bh,71h,3Ch,79h,08h,\
	    0F2h,20h,0B0h,6Ah,14h,01h,0CDh,3Fh,0C3h,0E7h,0A9h,59h,45h,13h,0Fh,75h,0DFh,57h,48h,31h,9Ch,5Fh,0A8h,22h,0E1h,6Ch,9Ch,5Fh,21h,0E1h,06h,2Fh,\
	    0D1h,0E4h,03h,3Ch,2Dh,0EBh,1Bh,7Ah,9Ah,7Ch,80h,07h,0F3h,0BBh,23h,30h,3Ch,9Eh,5Ah,2Ah,5Fh,7Eh,36h,0Ch,03h,0D3h,21h,1Ch,0A7h,46h,5Ch,79h,\
	    09h,00h,0CCh,5Ch,5Fh,99h,4Eh,04h,83h,80h,2Ah,43h,64h,15h,71h,52h,09h,2Dh,6Dh,40h,07h,51h,0E7h,30h,0D2h,92h,0E8h,09h,99h,0C8h,06h,64h,\
	    0FAh,0A8h,0B5h,35h,0B5h,91h,0E3h,16h,0B1h,30h,36h,0Eh,6Bh,0B6h,38h,79h,1Bh,0A6h,0ACh,10h,2Bh,12h,0DCh,34h,9Fh,30h,0CBh,2Bh,17h,15h,0A3h,70h,\
	    0C5h,0DDh,09h,40h,0A0h,07h,0C5h,7Fh,63h,54h,0E8h,54h,0D3h,23h,08h,02h,94h,0CFh,0CBh,0Ah,52h,0E1h,0D4h,0Dh,2Ah,38h,0Dh,53h,5Ah,9Bh,2Bh,4Fh,\
	    0C4h,6Ch,0E9h,4Bh,6Eh,0E6h,0ABh,49h,54h,0FAh,0ADh,04h,0D3h,86h,41h,55h,0DEh,0B5h,54h,40h,0B7h,7Fh,5Bh,5Ch,05h,03h,0D6h,58h,9Ch,0CFh,0A6h,5Dh,\
	    58h,12h,0Eh,16h,0F1h,15h,07h,35h,0C8h,18h,0A9h,78h,0E6h,0EEh,62h,0Eh,6Dh,38h,1Ch,12h,0C7h,0DCh,7Dh,40h,9Bh,41h,23h,3Dh,0A3h,0FCh,19h,45h,\
	    0ACh,30h,0D9h,26h,7Ah,5Eh,74h,43h,0E7h,0A8h,5Ah,1Dh,0F1h,0F3h,91h,76h,97h,81h,0E7h,7Ch,6Fh,58h,02h,30h,0C6h,93h,25h,36h,0E7h,0Ch,7Eh,7Fh,\
	    60h,0D4h,35h,32h,31h,0C6h,0F7h,78h,88h,7Fh,2Ah,62h,67h,0C3h,96h,01h,21h,0CCh,0A7h,33h,03h,0A9h,54h,7Eh,0B0h,7Eh,5Eh,15h,0CBh,56h,85h,0Eh,\
	    0BDh,7Ch,0B1h,7Eh,0ECh,6Eh,73h,34h,7Fh,0B6h,1Bh,0Dh,6Eh,0CBh,0Ah,55h,77h,06h,0D9h,11h,0ADh,98h,27h,7Eh,8Dh,0Bh,0A6h,0Dh,0F6h,54h,9Eh,33h,\
	    17h,4Fh,0E1h,49h,0A7h,0F6h,0F0h,0Fh,15h,0B0h,2Dh,34h,0D4h,1Ch,0CCh,62h,93h,3Bh,0DCh,7Fh,52h,0BBh,96h,34h,91h,0E8h,0BBh,1Fh,0A0h,0B3h,41h,0Dh,\
	    4Fh,5Ch,01h,14h,0ADh,0C2h,65h,47h,0A5h,09h,3Ch,72h,0F4h,1Bh,0FEh,38h,01h,0B1h,0E0h,0Bh,7Ah,00h,1Eh,7Eh,0D4h,0A8h,48h,10h,2Bh,65h,3Fh,11h,\
	    53h,0EDh,0Eh,37h,0B9h,53h,33h,55h,0B3h,8Dh,48h,52h,0BFh,83h,7Dh,03h,70h,0DEh,65h,79h,03h,1Dh,0D0h,0Ch,97h,0FEh,0B0h,1Bh,0BCh,0BCh,8Eh,56h,\
	    7Ah,0C3h,79h,29h,0F0h,43h,9Fh,36h,33h,39h,3Bh,05h,0FCh,64h,23h,7Fh,0B9h,24h,0D0h,70h,0FCh,8Ah,0E5h,34h,9Fh,0A9h,0EEh,45h,0A1h,0E7h,63h,60h,\
	    4Fh,07h,0ADh,66h,7Ah,2Dh,0BFh,62h,5Eh,28h,0AEh,0Fh,0ADh,76h,0E1h,35h,0ADh,0A2h,37h,2Fh,0FCh,0A7h,44h,28h,4Bh,0Ah,37h,61h,1Fh,6Fh,5Ch,32h
	;end virtual

	crc = 0FFFFFFFFh
	repeat len
	load zbyte byte from start+%-1
	crc2 = (crc and 0FFh) xor zbyte
	load zbyte dword from (CrtTable+crc2*4)
	crc = (crc shr 8) xor zbyte
	end repeat
	name dd crc xor 0FFFFFFFFh
end if
}

macro data_import
{
data import
     library kernel32,'KERNEL32.DLL',\
	     user32,'USER32.DLL',\
	     gdi32,'GDI32.DLL',\
	     advapi32,'ADVAPI32.DLL',\
	     comctl32,'COMCTL32.DLL',\
	     comdlg32,'COMDLG32.DLL',\
	     shell32,'SHELL32.DLL',\
	     wsock32,'WSOCK32.DLL',\
	     wininet,'WININET.DLL'

     import_kernel32
     import_user32
     import_gdi32
     import_advapi32
     import_comctl32
     import_comdlg32
     import_shell32
     import_wsock32
     import_wininet

     all_api
end data
}

macro CRC32TableGen data
{
	local .InitTableLoop,.entryLoop,.no_topbit,.entrygoon

	xor ebx,ebx
	mov edx,data
  .InitTableLoop:
	xor eax,eax
	mov al,bl
	xor ecx,ecx
  .entryLoop:
	test eax,1
	jz .no_topbit
	shr eax,1
	xor eax,0EDB88320h
	jmp .entryLoop
  .no_topbit:
	shr eax,1
  .entrygoon:
	inc ecx
	test ecx,8
	jz .entryLoop
	mov dword [ebx*4+edx],eax
	inc ebx
	test ebx,256
	jz .InitTableLoop
}

macro fake_ff
{
if DEBUG=0
	push .real
	clc
	jc .fake
	retn
	.fake:
	db 0FFh
	.real:
end if
}

macro invokex proc,[arg]
{
reverse pushd <arg>
xor edx,edx
dec edx
imul ecx,edx,-proc
common call dword [ecx]
}