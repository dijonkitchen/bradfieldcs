section .text
    global sum_to_n

sum_to_n: ; inclusive
    mov rax, 0 ; sum
    mov rbx, 0 ; count
    jmp .compare

.loop:
    add rax, rbx
    add rbx, 1
    jmp .compare

.compare:
    cmp rbx, rdi
    jle .loop
	ret

