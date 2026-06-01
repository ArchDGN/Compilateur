			# This code was produced by the CERI Compiler
	.data
FormatString1:	.string "%llu\n"
	.align 8
	.data
	.align 8
a:	.quad 0
b:	.quad 0
c:	.quad 0
d:	.quad 0
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $0
	pop a
	push $1
	pop b
.L_FOR_COND_1:
	push $5
	pop %rbx
	push b
	pop %rax
	cmpq %rbx, %rax
	jg .L_FOR_END_1
	push a
	push b
	pop %rbx
	pop %rax
	addq	%rbx, %rax	# ADD
	push %rax
	pop a
	push b
	pop %rax
	addq $1, %rax
	push %rax
	pop b
	jmp .L_FOR_COND_1
.L_FOR_END_1:
	push a
	push $15
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	je Vrai2	# If equal
	push $0		# False
	jmp Suite2
Vrai2:	push $0xFFFFFFFFFFFFFFFF		# True
Suite2:
	pop %rax
	cmpq $0, %rax
	je .L_ELSE_3
	push a
	pop %rsi		# Récupère la valeur à afficher depuis la pile
	movq $FormatString1, %rdi	# Prépare le format %llu
	movl $0, %eax		# Dit à printf qu'il n'y a pas de nombres flottants
	push %rbp		# Sauvegarde rbp (protection de la pile)
	call printf@PLT	# Appel à la fonction standard C
	pop %rbp		# Restaure rbp
	jmp .L_ENDIF_3
.L_ELSE_3:
	push $0
	pop c
.L_ENDIF_3:
	push $3
	pop d
.L_WHILE_COND_4:
	push d
	push $0
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai5	# If above
	push $0		# False
	jmp Suite5
Vrai5:	push $0xFFFFFFFFFFFFFFFF		# True
Suite5:
	pop %rax
	cmpq $0, %rax
	je .L_WHILE_END_4
	push d
	push $1
	pop %rbx
	pop %rax
	subq	%rbx, %rax	# SUB
	push %rax
	pop d
	jmp .L_WHILE_COND_4
.L_WHILE_END_4:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
