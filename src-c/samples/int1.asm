# use with GNU as
.section	__TEXT,__text
	.intel_syntax
	.globl	_main
_main:
	.cfi_startproc
	mov	eax, 1
	ret
	.cfi_endproc