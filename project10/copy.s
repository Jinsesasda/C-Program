# -*- mode: text -*-

#
/* Author: Jinseok Back
# Section: 0208
# This is a
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

		jal	jacob		# call jacobsthal(number)

		add	$sp, $sp, 4	# pop argument from stack

		sw	$v0, result  	# result= jacobsthal(number)

		move	$a0, $v0	# printf("%d\n", result)
		li	$v0, 1
		syscall
		li	$v0, 11
		li	$a0, 10
		syscall

		li	$v0, 10		# quit program
		syscall

endif:		move	$v0, $t0	# v0 holds return value ans

			     		# epilogue
		lw	$ra, 24($sp)	# restore $ra
		lw	$fp, 20($sp)	# restore old $fp
		add	$sp, $sp, 24	# reset $sp
		jr	$ra  	  	# return to caller

else:		li	$t0, 1		# ans= 1
		sw	$t0, 16($sp)	# push argument ans

		li	$t3, 0		# prev= 0
		sw	$t3, 12($sp)	# push argument prev

		li	$t2, 2		# i= 2
		sw	$t2, 4($sp)

skip:		sw	$t1, 16($sp)	# ans= n

		j	endif		# quit if statement

					# prologue
jacob:		sub	$sp, $sp, 24	# set new stack pointer
		sw	$ra, 24($sp)	# save return address
		sw	$fp, 20($sp)	# save frame pointer
		add	$fp, $sp, 24	# set new frame pointer

		li	$t0, -1		# ans= -1
		sw	$t0, 16($sp)

		lw	$t1, 4($fp)	# get argument n

		bltz	$t1, endif	# if (n >= 0)

		beq	$t1, 0, skip	# if (n == 0 || n == 1)
		beq	$t1, 1, skip

		j	else

					# prologue
helper:		sub	$sp, $sp, 8	# set new stack pointer
		sw	$ra, 8($sp)
		sw	$fp, 4($sp)
		add	$fp, $sp, 8

		lw	$t4, 8($fp)	# 2 * x + y
		mul	$t4, $t4, 2
		lw	$t5, 4($fp)
		add	$t4, $t4, $t5

		move	$v0, $t3	# holding the return value

					# epilogue
		lw	$ra, 8($fp)	# restore $ra
		lw	$fp, 4($fp)	# restore old $fp
		add	$sp, $sp, 8	# reset $sp
		jr	$ra		# return to caller

loop:		bgt	$t2, $t1, endif

		sub	$sp, $sp, 8

		jal	helper		# call helper()

		add	$sp, $sp, 8	# pop arguments from stack

		sw	$v0, 8($sp)	# temp= helper()

		sw	$t0, 12($sp)	# prev= ans

		lw	$t0, 8($sp)	# ans= temp
		sw	$t0, 16($sp)

		add	$t2, $t2, 1	# i++
		sw	$t2, 4($sp)

		j	loop		# iterate loop
