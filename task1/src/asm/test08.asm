# Test jump when cond = 1
stack 2
	push print_x
	push 1
	jmp 
	push 89
	putc
	halt
print_x:
	push 88
	putc
	halt
