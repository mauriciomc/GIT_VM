# Test eq (equal case)
stack 2
	push 42
	push 42
	eq
	push 87
	add
	putc
	halt
