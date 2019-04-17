# Check IP is as expected. Note when pushed IP points to the next instruction.
stack 3
	push_ip
	push_ip
	push 85
	add
	add
	putc
	halt
