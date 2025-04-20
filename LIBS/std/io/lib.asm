section .text

global print
global exit

print:
    mov rcx, rax  
.next_char:
    cmp byte [rcx], 0
    je .found_end
    inc rcx
    jmp .next_char
.found_end:
    sub rcx, rax
    mov rdx, rcx     
    mov rsi, rax     
    mov rax, 1  
    mov rdi, 1
    syscall
    ret

exit:
    mov rdi, rax
    mov rax, 60
    syscall