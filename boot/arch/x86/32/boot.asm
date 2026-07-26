[BITS 16]
[ORG 0x7C00]           ; boot sector load address

start:
    cli                 ; disable interrupts
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00      ; stack pointer

    ; Load GDT (Global Descriptor Table) for protected mode
    lidt [gdt_descriptor]

    ; Enable protected mode (set PE bit in CR0)
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to flush prefetch and switch to 32-bit code segment
    jmp CODE_SEG:init_pm

[BITS 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000     ; set up stack

    ; Now in protected mode - you can continue with your 32-bit code here
    ; For example, call your C main or print something

hang:
    hlt
    jmp hang

; GDT definition
gdt_start:
    dq 0x0000000000000000      ; null descriptor
    dq 0x00CF9A000000FFFF      ; code segment descriptor
    dq 0x00CF92000000FFFF      ; data segment descriptor
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

times 510-($-$$) db 0
dw 0xAA55                  ; boot sector signature