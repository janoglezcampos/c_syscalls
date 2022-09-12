[BITS 64]

GLOBAL Syscall
EXTERN GetSsn

[SECTION .text]

Syscall:
    call get_info

    mov [rsp - 0x8],  rsi
    mov [rsp - 0x10], rdi

    mov rcx, rdx

    mov r10, r8
    mov rdx, r9
    
    mov  r8,  [rsp + 0x28]
    mov  r9,  [rsp + 0x30]

    sub rcx, 0x4
    jle skip

    lea rsi,  [rsp + 0x38]
    lea rdi,  [rsp + 0x28]

    rep movsq
skip:
    syscall

    mov rsi, [rsp - 0x8]
    mov rdi, [rsp - 0x10]

    ret

get_info:
    push rdx
    mov rdx, 0
    sub rsp, 0x28
    call GetSsn
    add rsp, 0x28

    ;rax holds ssn
    pop rdx
    ret