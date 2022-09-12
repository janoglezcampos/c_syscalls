[BITS 32]

GLOBAL _Syscall
EXTERN _GetSsn

[SECTION .text]

_Syscall:
    call get_info

    mov [esp - 0x04], esi
    mov [esp - 0x08], edi

    mov ecx, [esp + 0x08]

    lea esi, [esp + 0x0C]
    lea edi, [esp + 0x04]

    rep movsd

    mov esi, [esp - 0x04]
    mov edi, [esp - 0x08]

    mov edx, fs:[0xc0]
    test edx, edx
    je native

    call edx
    ret

native:
    call sysenter
    ret

sysenter:
    mov edx,esp
    sysenter

get_info:
    push ebp
    mov ebp, esp
    mov eax, [esp + 0x0C]
    push 0
    push eax
    call _GetSsn
    and eax, 0x0000FFFF
    mov esp, ebp
    pop ebp
    ret