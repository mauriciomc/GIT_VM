# Test stack pointer value is correct relative to code size.
stack 2
	push_sp
	push 81
	add
	putc
	halt
