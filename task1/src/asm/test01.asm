# Test minimal functionality needed for other tests: push const, putc and halt
# Note "stack" does not lead to any VM instructions.
stack 1
	push 88
	putc
	halt
