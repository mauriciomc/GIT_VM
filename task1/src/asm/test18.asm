# Test eq (not equal case)
stack 2
	push 42
	push 69
	eq
	push 88
	add
	putc
	halt
