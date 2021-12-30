	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 11, 3
	.section	__TEXT,__literal4,4byte_literals
	.p2align	2                               ## -- Begin function main
LCPI0_0:
	.long	0x4e6e6b28                      ## float 1.0E+9
LCPI0_1:
	.long	0x49742400                      ## float 1.0E+6
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movl	$0, -4(%rbp)
	callq	_clock
	movq	%rax, -16(%rbp)
	movl	$0, -20(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$1000000000, -20(%rbp)          ## imm = 0x3B9ACA00
	jge	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	jmp	LBB0_3
LBB0_3:                                 ##   in Loop: Header=BB0_1 Depth=1
	movl	-20(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -20(%rbp)
	jmp	LBB0_1
LBB0_4:
	movss	LCPI0_0(%rip), %xmm0            ## xmm0 = mem[0],zero,zero,zero
	movss	%xmm0, -56(%rbp)                ## 4-byte Spill
	movss	LCPI0_1(%rip), %xmm0            ## xmm0 = mem[0],zero,zero,zero
	movss	%xmm0, -52(%rbp)                ## 4-byte Spill
	callq	_clock
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	-16(%rbp), %rcx
	subq	%rcx, %rax
	movq	%rax, %rdx
	shrq	%rdx
	movl	%eax, %ecx
	andl	$1, %ecx
                                        ## kill: def $rcx killed $ecx
	orq	%rdx, %rcx
	cvtsi2ss	%rcx, %xmm0
	addss	%xmm0, %xmm0
	cvtsi2ss	%rax, %xmm1
	movss	%xmm1, -48(%rbp)                ## 4-byte Spill
	testq	%rax, %rax
	movss	%xmm0, -44(%rbp)                ## 4-byte Spill
	js	LBB0_6
## %bb.5:
	movss	-48(%rbp), %xmm0                ## 4-byte Reload
                                        ## xmm0 = mem[0],zero,zero,zero
	movss	%xmm0, -44(%rbp)                ## 4-byte Spill
LBB0_6:
	movss	-56(%rbp), %xmm1                ## 4-byte Reload
                                        ## xmm1 = mem[0],zero,zero,zero
	movss	-52(%rbp), %xmm2                ## 4-byte Reload
                                        ## xmm2 = mem[0],zero,zero,zero
	movss	-44(%rbp), %xmm0                ## 4-byte Reload
                                        ## xmm0 = mem[0],zero,zero,zero
	divss	%xmm2, %xmm0
	movss	%xmm0, -36(%rbp)
	movaps	%xmm1, %xmm0
	divss	-36(%rbp), %xmm0
	movss	%xmm0, -40(%rbp)
	movss	-40(%rbp), %xmm0                ## xmm0 = mem[0],zero,zero,zero
	divss	%xmm1, %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	L_.str(%rip), %rdi
	movb	$1, %al
	callq	_printf
	xorl	%eax, %eax
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Clock speed approximately %.3f GHz\n"

.subsections_via_symbols
