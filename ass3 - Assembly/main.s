	#313467441 Lee Alima

	.section	.rodata
.symbolInt:				# create a label for scanning an integer
	.string	"%d"
.symbolString:				# create a label for scaning a string
	.string	"%s"
	.text
	.globl	main			# global label main
	.type	main, @function		# begining of the main method
main:					# this is the main method, in the main I received 2 integers and 2
					# strings from the user and saved in the stack the information
					# as was requested in the assignment. with the right pointers
					# to the specific locations in the stack I called run_func
	pushq	%rbp			# save rbp address to the stack
	movq	%rsp,	%rbp		# save the begining address of the frame in rbp
					# scan the first Pstring data, start by receiving an integer
					# save it to the stack and than to a register and clearing the
					# stack again. than I made more place in the stack for the string and the len(char)
	sub	$4,	%rsp		# make place on the stack (for int)
	movq	%rsp,	%rsi		# save rsp in rsi
	movq	$.symbolInt,	%rdi	# save the label of "%d" in rdi
	xorq	%rax,	%rax		# make rax value to 0
	call scanf			# call scanf method and scan input
	movzbq	(%rsp),	%r10		# save the len (4 bytes) value in r10
	add	$4,	%rsp		# clear the number from the stack
	subq	$1,	%rsp		# make place on the stack for '/0'
	movb	$0,	(%rsp)		# write '/0' on the stack (the end of the string)
	subq	%r10,	%rsp		# make place in the stack for the string (by the len saved in r10)
	subq	$1,	%rsp		# make place in the stack for len (char)
	movb	%r10b,	(%rsp)		# save in rsp the value of r10 (convert it to char, using byte)
	movq	%rsp,	%rsi		# save the current stack pointer address in rsi
	inc	%rsi			# increment rsi by 1 - to scan input at the right place (after the first char - len)
	movq	$.symbolString,	%rdi	# save the label of "%s" in rdi
	xorq	%rax,	%rax		# make rax value to 0
	pushq	%r10			# pushing r10 (he first input from the user - len as integer)
	call scanf			# call scanf
	popq	%r10			# pop %r10 from the stack
	movq	%r10,	%r13		# save %r10(len in integer) in a calle save register r13
	leaq	(%rsp),	%r15		# save the current address that symbolizes the pointer for the pstring in a callee save
					# scan the second Pstring data ,do the exactly the same thing as was done before.
	sub	$4,	%rsp		# make place on the stack ( for int )
	movq	%rsp,	%rsi		# save rsp in rsi
	movq	$.symbolInt,	%rdi	# save the label of "%d" in rdi
	xorq	%rax,	%rax		# make rax value to 0
	call scanf			# call scanf method and scan input
	movzbq	(%rsp),	%r10		# save the len ( 4 bytes ) value in r10
	add	$4,	%rsp		# clear the number from the stack
	subq	$1,	%rsp		# make place on the stack for len (char)
	movb	$0,	(%rsp)		# write '/0' on the stack
	subq	%r10,	%rsp		# make place in the stack for the string (by the len saved in r10)
	subq	$1,	%rsp		# make place in the stack for len (char)
	movb	%r10b,	(%rsp)		# save in rsp the valuer of r10 (convert it to char, using byte)
	movq	%rsp,	%rsi		# save rsp in rsi
	inc	%rsi			# increment rsi in 1 - to scan input at the right place ( after len )
	movq	$.symbolString,	%rdi	# save the label of "%s" in rdi
	xorq	%rax,	%rax		# make rax value to 0
	pushq	%r10			# pushing r10 (he first input from the user - len as integer)
	call scanf			# call scanf
	popq	%r10			# pop %r10 from the stack
	movq	%r10,	%r14		# save %r10 (len in integer) in a calle save register r14
	leaq	(%rsp),	%r12		# save the current address that symbolizes the pointer for the pstring in a callee save
					# scan the option number
	sub	 $4,	%rsp		# make place on the stack (for int), now for the case option
	movq	%rsp,	%rsi		# save rsp in rsi
	movq	$.symbolInt,	%rdi	# save the label of "%d" in rdi
	xorq	%rax,	%rax		# make rax value to 0
	call	scanf			# call scanf to scan the input
	movzbq	(%rsp),	%rax		# save the number in rax
	add	$4,	%rsp		# clear the number from the stack
					# send run_func the right arguments
	movq	%rax, %rdi		# save the option number as the first parameter to the function
	movq	%r15, %rsi		# save the address of the first Pstring as the second parameter
	movq	%r12, %rdx		# save the address of the second Pstring as the third parameter
	call run_func			# call the funcion
	addq	$4,	%rsp		# clear 4 bytes from the stack for the 2 '\0' and the lens
	addq	%r13,	%rsp		# clear the first string from the stack
	addq	%r14,	%rsp		# clear the second string from the stack
	movq	%rbp,	%rsp		# return to the old fp
	popq	%rbp			# pop rbp from the stack
	xorq	%rax,	%rax		# make rax value to 0
	ret				# end main
