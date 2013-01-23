proc setstat hwnd, text
	invoke SetDlgItemTextA,[hwnd],IDL_STAT,[text]
	ret
endp