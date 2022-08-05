//John Hills
//lab3a

.section .rodata 

argc_error: .asciz "\nIncorrect number of arguments.\n"

operror: .asciz "\nInvalid operator. Please use \"+\", \"-\", \"x\", \"/\", or \"%\"." 

div_zero_error: .asciz "\nIllegal division by zero. Try again.\n"

result: .asciz "\n%d %c %d = %d\n"

addition:

add		s4, s1, s3
mv		a4, s4

j		return

subtraction:

sub		s4, s1, s3
mv		a4, s4

j		return

multiply:

mul		s4, s1, s3
mv		a4, s4

j		return

divide:
//error check for dividing with zero
beq		s3, zero, 2f
div		s4, s1, s3
mv		a4, s4

j		return

modulus:

rem		s4, s1, s3
mv		a4, s4

j		return


.section .text
.global main

//main is the only global label.
main:

	//int main (int argc, char * argv[]) 
	//a0  main (a0      , a1). NOTE: elements of a1 are 8 bytes each b/c they are pointers to strings.
	//I'm going to try and use the "saved registries.

	addi	sp, sp, -8
	sd		ra, 0(sp)
//***************************************************************************************************************************

	//a0 stores the number of command lines arguments, including the name of executable. 
	//DO NOT USE DEREFERENCE OPERATIONS ON a0!!! IT'S A VALUE, NOT ADDRESS [SEG FAULT: lw  s0, 0(a0)] 
	//a1 stores a char * array of subsequent command line arguments deliminted by the space character into each index.
	
	li		s11, 4				//required # of command line arguments
	mv		s0, a0				//moves a0 (argc) into s0

	bne		s11, s0, 1f			//ERROR: was getting a "relocation truncated to fit" error. FIX: changed label from "1" to "1f". Again, no idea why this fixed it.
	j		no_argc_error

1:
	la		a0, argc_error
	call	printf@plt	

	j		error_return
no_argc_error:

	//do not need to load the value from "0(a1)". It's just a string containing the executable part of command line arguments. 
	ld		s1, 8(a1)	//loads "left" string operand in s1
	
	ld		s2, 16(a1)	//loads "op" string in s2
	//CRUCIAL STEP. Before adding this "1b" line, s2 (op) had extra data included which made it impossible to convert properly to ascii. Once this was applied, I got proper numbers.
	lb		s2, 0(s2)	//something I saw sean do. It would seem that perhaps I'm getting too much data for the op to be recognized by ascii
	ld		s3, 24(a1)	//loads "right" string operand in s3

	//Convert strings to ints
	mv		a0, s1
	call	atoi@plt
	mv		s1, a0
	
	mv		a0, s3
	call	atoi@plt
	mv		s3, a0

	//assigning ascii values for each op
	li		s5, 43		// "+"
	li		s6, 45		// "-"
	li		s7, 88		// "X"
	li		s8, 120		// "x"
	li		s9, 47		// "/"
	li		s10, 37		// "%"

	//preparing arguments for final print
	mv		a1, s1		// "left" %d
	mv		a2, s2		// "op" %c
	mv		a3, s3		// "right" %d
	
	beq		s2, s5, addition
	beq		s2, s6, subtraction
	beq		s2, s7, multiply
	beq		s2, s8, multiply
	beq		s2, s10, modulus
	beq		s2, s9, divide

	//Error check for invalid operators
	la		a0, operror
	call	printf@plt
	j		error_return
	
2:

	la		a0, div_zero_error
	call	printf@plt
	j		error_return
//***************************************************************************************************************************
	
return:
	
	la		a0, result
	call	printf@plt

	li		a0, 0
	ld		ra, 0(sp)
	addi	sp, sp, 8

	ret

error_return:

	li		a0, -1
	ld		ra, 0(sp)
	addi	sp, sp, 8

	ret


