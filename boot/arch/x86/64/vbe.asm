[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; Set VESA mode 0x118 = 1024x768x32bpp (linear framebuffer)
    mov ax, 0x4F02                                     ; Set VBE Mode
    mov bx, 0x118 | 0x4000 ; set LFB mode (bit 14)
    int 0x10

    ; Check for success
    cmp ax, 0x004F
    jne .fail

    ; Get mode info for framebuffer address (optional)
    ; Set up segment:offset where buffer is located, usually 0xE0000000 or similar

    ; Here you would call your multiboot1/2 loader
    jmp load_kernel

.fail:
    ; fallback to text mode or halt
    hlt
    jmp $

load_kernel:
    ; Jump to protected mode loader or multiboot-compatible stub
    ; e.g., from multiboot1.asm or multiboot2.asm
    ; You’ll enter protected mode here and jump to kernel
