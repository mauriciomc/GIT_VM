# Test load
# Note that "data_ptr:" is just a label and does not lead directly to any output in the .bin file.
stack 1
	push data_ptr
	load
	putc
	halt
data_ptr:
	datum 0x00000058
# Note: 0x58 = 88 (decimal)
