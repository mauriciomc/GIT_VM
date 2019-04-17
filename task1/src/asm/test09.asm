# Test jump when cond = 0
stack 2
	push print_x
	push 0
	jmp 
	push 88
	putc
	halt
print_x:
	push 89
	putc
	halt
