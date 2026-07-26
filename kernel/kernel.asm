BITS 32
section .text
global _start        ; entry point visible to linker
extern kmain         ; declare external C function

_start:
    mov esp, stack_space
    add esp, 4096     ; set up stack pointer

    xor eax, eax      ; legacy/null boot info until the native loader passes one
    push eax
    call kmain       ; call the C kernel function

.hang:
    jmp .hang        ; infinite loop to hang after kmain returns

section .bss
stack_space: resb 4096  ; reserve 4KB stack space
