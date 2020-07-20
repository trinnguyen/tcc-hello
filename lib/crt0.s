.text
.intel_syntax noprefix
.globl _start

_start:
	# call main
	mov rdi, 0
	call main
	mov r12, rax

	# exit
	mov rax, 60
	mov rdi, r12
	syscall
