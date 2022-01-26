LOADW reg (mem)
	Loads the value at the specified memory address into the specified register

BEQZ reg addr
	If the value in the register is 0, jump to the specified instruction; otherwise, do nothing

ADD reg_a reg_b
	reg_a = reg_a + reg_b

SUBI reg x
	reg = reg - x, where x is some constant value

JUMP addr
	Jump to the specified instruction

STOREW reg (mem)
	Store the value in the specified register into the specified memory address

HALT
	Terminate the program