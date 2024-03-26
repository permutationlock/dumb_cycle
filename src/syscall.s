.text
.global syscall0
.type	syscall0,@function
syscall0:
	.cfi_startproc
    movq %rdi, %rax
    syscall
    ret
    .size syscall0, .-syscall0
    .cfi_endproc

.global syscall1
.type	syscall1,@function
syscall1:
	.cfi_startproc
    movq %rdi, %rax
    movq %rsi, %rdi
    syscall
    ret
    .size syscall1, .-syscall1
    .cfi_endproc

.global syscall2
.type	syscall2,@function
syscall2:
	.cfi_startproc
    movq %rdi, %rax
    movq %rsi, %rdi
    movq %rdx, %rsi
    syscall
    ret
    .size syscall2, .-syscall2
    .cfi_endproc

.global syscall3
.type	syscall3,@function
syscall3:
	.cfi_startproc
    movq %rdi, %rax
    movq %rsi, %rdi
    movq %rdx, %rsi
    movq %rcx, %rdx
    syscall
    ret
    .size syscall3, .-syscall3
    .cfi_endproc

.global syscall4
.type	syscall4,@function
syscall4:
	.cfi_startproc
    movq %rdi, %rax
    movq %rsi, %rdi
    movq %rdx, %rsi
    movq %rcx, %rdx
    movq %r8, %r10
    syscall
    ret
    .size syscall4, .-syscall4
    .cfi_endproc

.global syscall5
.type	syscall5,@function
syscall5:
	.cfi_startproc
    movq %rdi, %rax
    movq %rsi, %rdi
    movq %rdx, %rsi
    movq %rcx, %rdx
    movq %r8, %r10
    movq %r9, %r8
    syscall
    ret
    .size syscall5, .-syscall5
    .cfi_endproc

.global syscall6
.type	syscall6,@function
syscall6:
	.cfi_startproc
    movq %r9, %r11
    movq %rdi, %rax
	movq 8(%rsp), %r9
    movq %rsi, %rdi
    movq %rdx, %rsi
    movq %rcx, %rdx
    movq %r8, %r10
    movq %r11, %r8
    syscall
    ret
    .size syscall6, .-syscall6
    .cfi_endproc

.section .note.GNU-stack,"",@progbits
