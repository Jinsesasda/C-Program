# -*- mode: text -*-

#
# Jinseok Back
# Section: 0208
# This is a source file written in assembly language (MIPS). It translates the
# C source file "prog2.c" into an MIPS program. It has three functions which
# are "main", "jacobsthal", and "helper". It has two global variables "number"
# and "result" which are all initialized to 0 in the date section. This program
# will first go through the main function and it will call another function 
# "jacobsthal". The "jacobsthal" function is supposed to use the function
# parameter n to compute and generate a Jacobsthal number. During the process
# it will call its helper function "helper" by passing two arguments x and y,
# which are prev and ans, repectively. prev is required to save n-2 in the
# Jacobsthal sequence. The result will then be returned to its caller, main().
# Finally, after printing the result in the main function, it will terminate
# the program.
#

		.data
number:		.word 0
result:		.word 0

		.text
main:		li	$sp, 0x7ffffffc	# set up stack pointer

		li	$v0, 5		# scanf("%d", &number)
		syscall

		sw	$v0, ($sp)	# push argument number
		sub	$sp, $sp, 4

		jal	jacobsthal	# call jacobsthal(number)

		add	$sp, $sp, 4	# pop argument from stack

		sw	$v0, result  	# result= jacobsthal(number)

			     		# printf("%d\n", result)
		move	$a0, $v0	# prints result
		li	$v0, 1
		syscall

		li	$v0, 11		# prints \n
		li	$a0, 10
		syscall

		li	$v0, 10		# quit program
		syscall

					# prologue
jacobsthal:	sub	$sp, $sp, 24	# set new stack pointer
		sw	$ra, 24($sp)	# save return address
		sw	$fp, 20($sp)	# save frame pointer
		add	$fp, $sp, 24	# set new frame pointer

		li	$t0, -1		# ans= -1
		sw	$t0, 16($sp)

		lw	$t1, 4($fp)	# get argument n

		bltz	$t1, endif	# if (n >= 0)

		beq	$t1, 0, then	# if (n == 0 || n == 1)
		beq	$t1, 1, then	# jump to then

else:		li	$t0, 1		# ans= 1
		sw	$t0, 16($sp)	# push argument ans

		li	$t3, 0		# prev= 0
		sw	$t3, 12($sp)	# push argument prev

		li	$t2, 2		# i= 2
		sw	$t2, 4($sp)

		j	loop		# jump to the for-loop

then:		sw	$t1, 16($sp)	# ans= n

		j	endif

loop:		lw	$t2, 4($sp)	# get i
		bgt	$t2, $t1, endif	# if (i <= n)

		lw	$t3, 12($sp)	# get prev
		lw	$t0, 16($sp)	# get ans

		sw	$t3, ($sp)	# push argument prev
		sub	$sp, $sp, 4

		sw	$t0, ($sp)	# push argument ans
		sub	$sp, $sp, 4

		jal	helper		# call helper(prev, ans)

		add	$sp, $sp, 8	# pop arguments from stack

		sw	$v0, 8($sp)	# temp= helper(prev, ans)

		sw	$t0, 12($sp)	# prev= ans

		sw	$v0, 16($sp)	# ans= temp

		add	$t2, $t2, 1	# i++
		sw	$t2, 4($sp)

		j	loop		# iterate loop

endif:		lw	$t0, 16($sp)
		move	$v0, $t0	# v0 holds return value ans

			     		# epilogue
		lw	$ra, 24($sp)	# restore $ra
		lw	$fp, 20($sp)	# restore old $fp
		add	$sp, $sp, 24	# reset $sp
		jr	$ra  	  	# return to caller

					# prologue
helper:		sub	$sp, $sp, 8	# set new stack pointer
		sw	$ra, 8($sp)	# save return address
		sw	$fp, 4($sp)	# save frame pointer
		add	$fp, $sp, 8	# set new frame pointer

			     	  	# 2 * x + y
		lw	$t4, 8($fp)	# get x
		mul	$t4, $t4, 2	# compute 2 * x
		lw	$t5, 4($fp)	# get y
		add	$t4, $t4, $t5	# add y to the result

		move	$v0, $t4	# holding the return value

					# epilogue
		lw	$ra, 8($sp)	# restore $ra
		lw	$fp, 4($sp)	# restore old $fp
		add	$sp, $sp, 8	# reset $sp
		jr	$ra		# return to caller
