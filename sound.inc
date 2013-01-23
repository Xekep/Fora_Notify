proc sound param
	.if [param]=1 &  [music_flag]=0
		invokex getwinfullscr
		.if eax=1
			ret
		.endif
		call uFMOD_init
		push XM_MEMORY
		push xm_length
		push xm
		call uFMOD_PlaySong
		invoke Sleep,2880
		push 0
		push 0
		push 0
		call uFMOD_PlaySong
	.elseif [param]=2
		mov [music_flag],1
		call uFMOD_init
		push XM_MEMORY
		push xm2_length
		push xm2
		call uFMOD_PlaySong
		ret
	.elseif [param]=3
		mov [music_flag],0
		push 0
		push 0
		push 0
		call uFMOD_PlaySong
		ret
	.endif
	invoke ExitThread,0

	xm file 'fmod\1.xm'
	xm_length = $ - xm
	xm2 file 'fmod\music.xm'
	xm2_length = $ - xm2
endp