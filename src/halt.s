.text
.global halt
.type	halt,@function
halt:
	.cfi_startproc
    ud2
    ret
    .size halt, .-halt
    .cfi_endproc

.section .note.GNU-stack,"",@progbits
