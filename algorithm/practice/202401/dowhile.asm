a:
        .string "BasIC"
b:
        .string "MinIX"
main:
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-4], 0
.L3:
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        movzx   eax, BYTE PTR a[rax]
        and     eax, -33
        mov     edx, eax
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        mov     BYTE PTR a[rax], dl
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        movzx   eax, BYTE PTR b[rax]
        or      eax, 32
        mov     edx, eax
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        mov     BYTE PTR b[rax], dl
        add     DWORD PTR [rbp-4], 1
        cmp     DWORD PTR [rbp-4], 4
        setle   al
        test    al, al
        je      .L2
        jmp     .L3
.L2:
        mov     eax, 0
        pop     rbp
        ret