# Test negative numbers
stack 2
	push negative_one
	load
	push 89
	add
	putc
	halt
negative_one:
	datum 0xffffffff
