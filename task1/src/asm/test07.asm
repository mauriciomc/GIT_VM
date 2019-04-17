# Test store
stack 2
	push sometimes_X
	push 88
	store
	push sometimes_X
	load
	putc
	halt
sometimes_X:
	datum 0xdeadbeef
