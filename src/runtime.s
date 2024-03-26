.text
.extern _cstart
.global _start
_start:
	xor %rbp,%rbp
	mov %rsp,%rdi
	call _cstart

.section .note.GNU-stack,"",@progbits
