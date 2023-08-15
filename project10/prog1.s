# -*- mode: text -*-

# Author: Jinseok Back


# This is a source file written in assembly language (MIPS). It translates the
# C source file "prog1.c" into an MIPS program. It has labels "main" and
# "endif", and global variables "length", "width", "height", and "ans"; they
# are all initialized to 0 in the data section. The program will first go
# through the main function and it does not call any other functions. It also
# has a local variable "ans" which is initialized to -1 in the beginning of
# the main function. The main function will then scan for length, width, and
# height. If the conditions in the if statement at line 46 are met, it will
# modify the local variable "ans" appropriately. Finally, after it prints the
# result with a newline, it will quit program.
#


	.data	
length:	.word 0
width:	.word 0
height:	.word 0
ans:	.word 0

	.text
main:	lw	$t0, ans	# ans= -1
	sub	$t0, $t0, 1
	sw	$t0, ans

				# scanf("%d %d %d", &length, &width, &height)
	li	$v0, 5		# scans for length
	syscall
	move	$t1, $v0
	sw	$t1, length

	li	$v0, 5		# scans for width
	syscall
	move	$t2, $v0
	sw	$t2, width

	li	$v0, 5		# scans for height
	syscall
	move	$t3, $v0
	sw	$t3, height

				# if (length > 0 && width > 0 && height > 0)
	blez	$t1, endif	# if length <= 0 jump to endif
	blez	$t2, endif	# if width <= 0 jump to endif
	blez	$t3, endif	# if height <= 0 jump to endif

		     		# ans= 2 * (width * length + length * height
				# + width * height)
	mul	$t4, $t2, $t1	# save width * length in $t4
	mul	$t5, $t1, $t3	# save length * height in $t5
	add	$t4, $t4, $t5	# add $t4 and $t5 and store it in $t4
	mul	$t5, $t2, $t3	# save width * height in $t5
	add	$t4, $t4, $t5	# add $t4 and $t5 and store it in $t4
	mul	$t0, $t4, 2	# multiply the result by 2

	sw	$t0, ans

endif:	lw	$t0, ans	# get ans

				# printf("$d\n", ans)
	li	$v0, 1		# prints ans
	move	$a0, $t0
	syscall

	li	$v0, 11		# prints \n
	li	$a0, 10
	syscall

	li	$v0, 10		# quit program
	syscall
