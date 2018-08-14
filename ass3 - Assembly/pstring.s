	#313467441 Lee Alima

	.section .rodata
.invalid_input_output_format:
    .string "invalid input!\n"

	.text
##############################################################################################################################
	.globl	pstrlen				# global label pstrlen
	.type	pstrlen, @function		# begining of the pstrlen method
						# in this method I save the len in a register and return it.
pstrlen:
	pushq	%rbp				# save rbp to the stack
	movq	%rsp,	%rbp			# save rsp in rbp (start frame)
	movzbl	(%rdi),	%eax			# save the value (the first char) of rax in eax (return value)
	jmp	.endOfMethods			# jump to endOfMethods label and return
##############################################################################################################################
	.globl	replaceChar			# global label replaceChar
	.type	replaceChar, @function		# begining of the replaceChar method
						# In this method I go over the string (in the pstring) and looked
						# for the old char, if I found it I replace it in the new char,
						# and continue to the next char, else, continue to the next char.
replaceChar:
	pushq	%rbp				# push rbp to the stack
	movq	%rsp,	%rbp			# save rsp in rbp (start frame)
	movq	%rdi,	%rax			# save rdi in rax (save the address for the pstring)
	inc	%rax				# increment the address of rax by one to skip the len of the pstring				
	jmp	.checkIfnull			# jump to check if rax value is null (the end of the string)

.checkToSwap:					# the loop body
	cmpb	(%rax),	%sil			# compare the current char to the old char(from the input)
	jnz	.increment			# if they are not equals, then jump to .increment label (move to the next char)
	movb	%dl,	(%rax)			# if they are equals then swap them (save the new char in the current one)

.increment:					# increment the address rax saves
	inc	%rax				# move to the next char in the string

.checkIfnull:					# the condition of the loop
	cmpb	$0,	(%rax)			# compare the current string char to 0 (one byte)
	jnz	.checkToSwap			# if it's not a zero, jump to chckToSwap (do the loop)
	movq	%rdi,	%rax			# else, save rdi (the address of the pstring) in rax(return pointer)
	jmp	.endOfMethods			# jump to endOfMethods label and return
##############################################################################################################################
	.globl	pstrijcpy			# global label pstrijcpy
	.type	pstrijcpy, @function		# begining of the pstrijcpy method
						# in this method I started by checking if the indexing are llegal.
						# if they are, I went over both strings ( as long as they havn't ended)
						# and copy char from source to destination if the char is i<=index<=j
pstrijcpy:
	pushq	%rbp				# push rbp to the stack
	movq	%rsp,	%rbp			# save rsp in rbp (start frame)
	pushq	%r12				# push r12 to the stack (r12 is a callee save register,I have to protecet the prev val)
	movq	%rdi,	%r12			# save first pstring pointer address
						# check if option is valid
.checkIfILessThanZero:				# check if i<0
	cmpb	$0,	%dl
	jnge	.printMessage
.checkIfJLessThanZero:				# check if j<0
	cmpb	$0,	%cl
	jnge	.printMessage
.checkIfIBigger:				# check if i>j
	cmpb	%dl,	%cl
	jnge	.printMessage
.checkIfIBiggerThanFirstLen:			# check if i is bigger than the first Pstring len
	cmpb	%dl,	(%rdi)
	jng	.printMessage	
.checkIfIBiggerThanSecondLen:			# check if i is bigger than the second Pstring len
	cmpb	%dl,	(%rsi)
	jng	.printMessage
.checkIfJBiggerThanFirstLen:			# check if j is bigger than the first Pstring len
	cmpb	%cl,	(%rdi)
	jng	.printMessage
.checkIfJBiggerThanSecondLen:			# check if j is bigger than the second Pstring len
	cmpb	%cl,	(%rsi)
	jng	.printMessage
						# if i and j are ok , than start swapping chars
	inc	%rdi				# increment rdi by one to skip the len of the first pstring
	inc	%rsi				# increment rsi by one to skip the lent of the second pstring
	xorq	%r8,	%r8			# make r8 zero
	jmp	.checkIfnull1			# jmp to checkIfnull1
		
.checkForSwapI:					# check if the current char should be changed
	cmpb	%r8b,	%dl			# start by chrcking if current index is bigger than i
	jnle	.incrementStringAndIndex	# if the current index is less than i, continue and increment string and index

.checkForSwapJ:					# keep on checking if the current char should be changed
	cmpb	%r8b,	%cl			# check if the current index is less than j
	jnl	.swap				# if the current char is less than j (and already bigger than i) call swap
	jmp	.incrementStringAndIndex	# else, continue and increment string and index

.swap:						# change the current char in the destination string to the current char in th source
	xorq	%r9,	%r9			# make r9 zero
	movb	(%rsi),	%r9b			# save the current char from the source pstring
	movb	%r9b,	(%rdi)			# swap the char in the destinaion pstring

.incrementStringAndIndex:			# update the information for the next loop
	inc	%r8				# add 1 to the index
	inc	%rdi				# move to the next char in the first pstring (destination)
	inc	%rsi				# move to the nextchar in the second pstring (source)

.checkIfnull1:					# check if the first string has ended
	cmpb	$0,	(%rdi)			# compare between the current char in the first string to 0
	jz	.end				# if it's 0 than jump to end, else, check the second string current char

.checkIfnull2:					# check if the second string has ended		
	cmpb	$0,	(%rdi)			# compare between the current char in the second string to 0
	jnz	.checkForSwapI			# move on in the loop
	jmp	.end				# else, end loop

.printMessage:					# print an error message to the user	
	movq	$.invalid_input_output_format,	%rdi
	xorq	%rax,	%rax			# max zero eax
	call printf				# call printf

.end:						# the end of the method
	movq	%r12,	%rax			# save r12 in rax (return value)
	popq	%r12				# pop r12 from the stack (return to the value befor this method calling)
	jmp	.endOfMethods			# jump to endOfMethods label and return
##############################################################################################################################
	.globl	swapCase			# global label swapCase
	.type	swapCase, @function		# begining of the swapCase method
						# in this method I looked for letters ( by comparing the char value
						# to the possible values of a letter in ascii), if it's a letter I
						# changed it by adding\subbing 32 from the value and continue to next
						# char.
swapCase:
	pushq	%rbp				# push rbp to the stack
	movq	%rsp,	%rbp			# save rsp in rbp (start frame)
	movq	%rdi,	%rax			# save rdi in rax (save the address for the pstring)
	inc	%rax				# increment the address of rax by one to skip the len of the pstring				
	jmp	.checkIfEnd			# jump to check if rax value is null (the end of the string)

.checkToSwapChar:				# check if the current char is a letter
	cmpb	$90,	(%rax)			# compare the current char value to 90
	jnle	.checkForSmallLetter		# if it's bigger than 90 than jump to checkForSmallLetter
	cmpb	$65,	(%rax)			# compare the char value to 65
	jnl	.convertToSmall			# if it's bigger than 65 (and already smaller than 90) it's s big letter
						# and there is a need to change it
	jmp	.moveToTheNextLetter		# if it's not a big letter than move to the next char
	
.checkForSmallLetter:				# check if the current char is a "small" letter
	cmpb	$122,	(%rax)			# compare the char value to 122
	jnle	.moveToTheNextLetter		# if it's bigger than 122, than it's not a small letter
	cmpb	$97,	(%rax)			# compare the char value to 97
	jnl	.convertToBig			# if it's bigger than 97 (and already smaller than 122) it's s small letter
						# and there is a need to change it
	jmp	.moveToTheNextLetter		# current char value is between 91 to 96
	
.convertToBig:					# convert "Small" letter into big letter
	sub	$32,	(%rax)			# sub 32 from the value of the char (computed in ascii)
	jmp	.moveToTheNextLetter		# move to the next char

.convertToSmall:				# convert "Big" letter into small letter
	add	$32,	(%rax)			# add 32 to the value of the char (computed in ascii)

.moveToTheNextLetter:				# move to the next char in the string
	inc	%rax				# add 1 to the address of rax

.checkIfEnd:					# the condition of the loop , as long as the string hasn't ended
	cmpb	$0,	(%rax)			# compare the current string char to 0 (check if null)
	jnz	.checkToSwapChar		# if it's no a zero, jump to chckToSwap (do the loop)
	movq	%rdi,	%rax			# else, save rdi (the address of the pstring) in rax(return pointer)
	
##############################################################################################################################
.endOfMethods:		
	movq	%rbp,	%rsp			# return to the old fp
	popq	%rbp				# pop rbp from the stack
	ret					# end method
