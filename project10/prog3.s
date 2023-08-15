# -*- mode: text -*-

#
# Author: Jinseok Back

# Section: 0208
# This is a source file written in assembly language (MIPS). It translates the
# C source file "prog3.c" into an MIPS program. It has two functions which are
# "main" and "jacobsthal". It has two global variables "number" and "result"
# which are all initialized to 0 in the data section. This program will start
# with the main function that scans for a number and calls jacobsthal to print
# the result integer. The jacobsthal uses its function parameter n to compute
# and generate a Jacobsthal number. During the process, it will recursively
# call itself until it finds Jacobsthal n-1 and Jacobsthal n-2. Jacobsthal n-2
# is saved in the local variable "temp1" and Jacobsthal n-1 is saved in
# another local variable "temp2". The result value, a Jacobsthal number, will
# be the addition of the two numbers from temp1 and temp2. After the recursive
# call has ended, it will return the result "ans" to its caller "main". The
# "main" function will then print the result integer and then terminate the
# program.
#

		.data
number:		.word 0
result:		.word 0

		.text
main:		li	$sp, 0x7ffffffc	# set up a stack pointer

		li	$v0, 5		# scanf("%d", &number)
		syscall

		sw	$v0, ($sp)	# push argument number
		sub	$sp, $sp, 4

		jal	jacobsthal	# call jacobsthal(number)

		add	$sp, $sp, 4	# pop argument

		sw	$v0, result	# result = jacobsthal(number)

			     		# printf("%d\n", result)
		move	$a0, $v0	# prints d
		li	$v0, 1
		syscall

		li	$v0, 11
		li	$a0, 10		# prints \n
		syscall

		li	$v0, 10		# quit program
		syscall

					# prologue
jacobsthal:	sub	$sp, $sp, 20	# set new $sp
		sw	$ra, 20($sp)	# save $ra
		sw	$fp, 16($sp)	# save $fp
		add	$fp, $sp, 20	# set new $fp

		li	$t0, -1	  	# ans= -1
		sw	$t0, 12($sp)

		lw	$t1, 4($fp)	# get argument n

		bltz	$t1, endif	# if (n >= 0)

		beq	$t1, 0, then	# if (n == 0 || n == 1)
		beq	$t1, 1, then

else:		sub	$t1, $t1, 2	# push argument n - 2
		sw	$t1, ($sp)
		sub	$sp, $sp, 4

		jal	jacobsthal	# call jacobsthal(n - 2)

		add	$sp, $sp, 4	# pop argument n
		add	$t1, $t1, 2	# restore n
		sw	$t1, ($sp)

		move	$t2, $v0	# temp1= 2*jacbosthal(n-2)
		mul	$t2, $t2, 2
		sw	$t2, 8($sp)

		sub	$t1, $t1, 1	# push argument n - 1
		sw	$t1, ($sp)
		sub	$sp, $sp, 4

		jal	jacobsthal	# call jaconsthal(n - 1)

		add	$sp, $sp, 4	# pop argument n
		add	$t1, $t1, 1	# restore n
		sw	$t1, ($sp)

		move	$t3, $v0	# temp2= jacobsthal(n-1)
		sw	$t3, 4($sp)

		lw	$t2, 8($sp)	# get temp 1
		lw	$t3, 4($sp)	# get temp 2
		add	$t4, $t2, $t3	# ans= temp1 + temp2
		sw	$t4, 12($sp)	# save ans

		j	endif		# quit if statement

then:		lw	$t1, 4($fp)	# ans= n
		sw	$t1, 12($sp)

endif:		lw	$t0, 12($sp)	# get return value
		move	$v0, $t0

			     		# epilogue
		lw	$ra, 20($sp)	# restore $ra
		lw	$fp, 16($sp)	# restore $fp
		add	$sp, $sp, 20	# reset $sp
		jr	$ra  	  	# return to caller
