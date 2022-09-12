[BITS 32]

GLOBAL _Syscall
EXTERN _GetSsn

[SECTION .text]

_Syscall:
    mov ecx, [esp + 0x08]
    not ecx
    add ecx, 1
    lea edx, [esp + ecx * 4]

    mov ecx, [esp]
    mov [edx], ecx

    mov [edx - 0x04], esi
    mov [edx - 0x08], edi

    mov eax, [esp + 0x04] ;hash to eax
    mov ecx, [esp + 0x08] ;n_args

    lea esi, [esp + 0x0C]
    lea edi, [edx + 0x04]

    rep movsd

    mov esi, [edx - 0x04]
    mov edi, [edx - 0x08]
    
    mov esp, edx

    call get_info ;hash comes in eax

    mov edx, fs:[0xC0]
    test edx, edx
    je native

    mov edx, fs:[0xC0]
    jmp ecx

native:
    mov edx, ecx
    sub edx, 0x05
    push edx
    mov edx, esp
    jmp ecx
    ret

get_info:
    push ebp
    mov ebp, esp

    push edx
    lea edx, [esp - 0x04]
    sub esp, 0x04

    push edx
    push eax
    call _GetSsn
    add esp, 0x08

    and eax, 0x0000FFFF
    pop ecx
    pop edx

    mov esp, ebp
    pop ebp
    ret