	#313467441 Lee Alima

	.section	.rodata
	.align 8
.printMission1:
	.string	"first pstring length: %d, second pstring length: %d\n"
.printMission2:
	.string	"old char: %c, new char: %c, first string: %s, second string: %s\n"
.int_input_format:
	.string	" %d"
.printMission3And4:
	.string	"length: %d, string: %s\n"
.printError:
	.string	"invalid option!\n"
.char_input_format:
	.string " %c"
jump_table:					# switch case labels
	.quad case_50
	.quad case_51
	.quad case_52
	.quad case_53
	.text 					# begining the code
	.globl	run_func 			# global label of run_func, run function creates the jump table
						# and by the input (option) from the user, jump to the right case
						# label and continue correctly in the program
	.type	run_func, @function 		# the begining of the function
run_func:
	pushq	%rbp				# push rbp to the stack
	movq	%rsp,	%rbp			# save rsp in rbp
	pushq	%r12				# this is a callee register and must be saved
	pushq	%r13				# this is a callee register and must be saved
	pushq	%r14				# this is a callee register and must be saved
	pushq	%r15				# this is a callee register and must be saved
	leaq	-50(%rdi),	%r8		# save (option-50) in r8
	cmpq	$3,	%r8 			# compare r8 to 3
	ja	case_default			# go to case_default label
	jmp	*jump_table(,%r8,8)		# else jump using jump table 
						# I can write into rdi because I ended using it,
						# I only needed it for the jump table.

case_50:					# for case 50, print the strings length
	movq	%rsi,	%r12			# save rsi in a callee register, in case that pstrlen changes it.
	movq	%rdx,	%rdi			# save the rdx ( pointer to the first char of the first pstring that symbolize len)
	call pstrlen				# call pstrlen
	movq	%r12,	%rsi			# save the prev val in rsi
	movsbl	%al,	%ebx			# save result in ebx
	movq	%rsi,	%rdi			# save the rdx ( pointer to the first char of the second pstring that symbolize len)
	call pstrlen				# call pstrlen
	movsbl	%al,	%eax			# save result in eax
	movl	%ebx,	%edx			# save parameters ( make it possible to call printf with the right parameters)
	movl	%eax,	%esi			# save parameters ( make it possible to call printf with the right parameters)
	movl	$.printMission1,	%edi	# save the label in edi
	xorq	%rax,	%rax			# max zero eax
	call printf				# call printf
	jmp	.endCase			# end case

case_51:					# for 51 case, receive 2 chars from the user and call twice
						# to replaceChar on both pstrings.
	movq	%rsi,	%r12			# save the first pstring pointer(address) in a callee save register
	movq	%rdx,	%r13			# save the second pstring pointer(address) in a callee save register
						# scan the first char from the user
	subq	$1,	%rsp			# make place in the stack for the first char	
	movq	$.char_input_format,	%rdi	# save " %c" in rdi
	movq	%rsp,	%rsi			# save rsp in rsi
	movq	$0,	%rax			# make rax zero
	call scanf				# scan a char
						# scan the second char
	subq	$1,	%rsp			# make place in the stack for the second char
	movq	$.char_input_format,	%rdi	# save " %c" in rdi
	movq	%rsp,	%rsi			# save rsp in rsi
	movq	$0,	%rax			# make rax zero
	call scanf				# scan the second char
						# save the chars from the stack into callee save registers
	movzbq	(%rsp),	%rdx			# save the second char in rdx
	addq	$1,	%rsp			# clear one byte from the stack
	movzbq	(%rsp),	%rsi			# save the first char in rsi
	addq	$1,	%rsp			# clear one byte from the stack
						# first call for replaceChar (at the first Pstring)
	movq	%r12,	%rdi			# save the first pstring pointer in rdi
	call replaceChar			# call replaceChar for the first pstring
    	movq 	%rax,	%r12			# save result in r12 (callee save), (pointer for the pstring)
						# second call for replaceChar (at the second Pstring)
	movq	%r13,	%rdi			# save the second pstring pointer in rdi	
	call replaceChar			# call replaceChar for the second pstring
	movq	%rax,	%r13			# save result in r13 (callee save), (pointer for the pstring)
						# print by format
    	movq	$.printMission2,	%rdi	# save printing format
	leaq 	1(%r12),	%rcx		# save the pointer to the first pstring (+1 to reach the string) as the forth argument
	leaq	1(%r13),	%r8		# save the pointer to the second pstring (+1 to reach the string) as the fifth argument
    	xorq 	%rax,	%rax			# make rax 0
    	call printf				# print to the user
	jmp	.endCase			# end case

case_52:					# for case 52, receive 2 numbers from the user and call
						# pstrijcpy once, for the first pstring
	movq	%rsi,	%r12			# save the first pstring pointer(address) in a callee save register
	movq	%rdx,	%r13			# save the second pstring pointer(address) in a callee save register
						# scan the first int from the user
	subq	$4,	%rsp			# make place in the stack for the first int	
	movq	$.int_input_format,	%rdi	# save " %d" in rdi
	movq	%rsp,	%rsi			# save rsp in rsi
	movq	$0,	%rax			# make rax zero
	call scanf				# scan a numbe
						# scan the second number from the user
	subq	$4,	%rsp			# make place in the stack for the second int	
	movq	$.int_input_format,	%rdi	# save " %d" in rdi
	movq	%rsp,	%rsi			# save rsp in rsi
	movq	$0,	%rax			# make rax zero
	call scanf				# scan a number
						# save the numbers and clear the stack
	movzbq	(%rsp), %rcx			# save one byte (the first  number as char) in cl(rcx)
	addq	$4,	%rsp			# clear 4 bytes from the stack
	movzbq	(%rsp),	%rdx			# save one bytr (the second number as char) in dl(rdx)
	addq	$4,	%rsp			# clear 4 bytes from the stack
						# call pstrijcpy and print result
	movq	%r12,	%rdi			# save the first pstring pointer in rdi (destination)
	movq	%r13,	%rsi			# save the second pstring pointer in rsi (source)
	call pstrijcpy				# call the method
	movq	%rax,	%r14			# save the return pstring pointer in r14 (callee save register)
	inc	%r14				# increment the address by one to skip the len
	movq	%r12,	%rdi			# save the first pstring pointer in rdi
	call pstrlen				# call pstrlen
	movq	$.printMission3And4,	%rdi	# save the print format in rdi
	movsbl	%al,	%esi			# save result in esi(rsi)
	movq	%r14,	%rdx			# save returned pstring in rdx
	xorq	%rax,	%rax			# make rax zero
	call printf				# call printf
						# print the second string and it's len
	movq	%r13,	%rdi			# save the second pstring pointer in rdi
	call pstrlen				# call pstrlen
	movq	$.printMission3And4,	%rdi	# save the printing format
	movsbl	%al,	%esi			# save result in esi (rsi)
	inc	%r13				# increment the address by one to skip len
	movq	%r13,	%rdx			# save the string of the second pstring in rdx
	xorq	%rax,	%rax			# make rax zero
	call printf				# call printf
	jmp	.endCase			# end case
		
case_53:					# for case 53, call swapCase and pstrlen twice on
						# both pstrings
	movq	%rsi,	%r12			# save the first pstring pointer(address) in a callee save register
	movq	%rdx,	%r13			# save the second pstring pointer(address) in a callee save register
						# call the method on the first pstring
	movq	%rsi,	%rdi			# save the first pstring in rdi	
	call swapCase				# call swapCase
	movq	%rax,	%r14			# save the return value in r14 (callee save register)		
	inc	%r14				# skip the first char (skip the len)
	movq	%rsi,	%rdi			# save the first pstring in rdi
	call pstrlen				# call pstrlen
	movq	$.printMission3And4,	%rdi	# save the print format in rdi
	movsbl	%al,	%esi			# save result in esi(rsi)
	movq	%r14,	%rdx			# save returned pstring in rdx
	xorq	%rax,	%rax			# make rax zero
	call printf				# call printf
						# call the method on the second pstring
	movq	%r13,	%rdi			# save the second pstring in rdi
	call swapCase				# call swapCase
	movq	%rax,	%r14			# save the return pstring in r14
	inc	%r14				# skip the first char (skip the len)
	movq	%r13,	%rdi			# save the second pstring in rdi
	call pstrlen				# call pstrlen
	movq	$.printMission3And4,	%rdi	# save the print format in rdi
	movsbl	%al,	%esi			# save result in esi(rsi)
	movq	%r14,	%rdx			# save returned pstring in rdx
	xorq	%rax,	%rax			# make rax zero
	call printf				# call printf
	jmp	.endCase
		
case_default:					# default case, for invalid option.
						# I printed an error message and move to endCase
	movq	$.printError,	%rdi		# save the error message in rdi
	xorq	%rax,	%rax			# max rax zero
	call printf				# call printf


.endCase:					# the end of every case
	popq	%r15				# return to the register value before run_func method
	popq	%r14				# return to the register value before run_func method
	popq	%r13				# return to the register value before run_func method
	popq	%r12				# return to the register value before run_func method
	movq	%rbp,	%rsp			# return to the old fp
	popq	%rbp				# pop rbp from the stack
	xorq	%rax,	%rax			# max zero rax
	ret					# return to main				# return to main
