# exit.s
    .section    __TEXT,__text
    .globl  _main
    .equ maxCount, 0x114514
_main:
    movq    $0, %rax
    retq