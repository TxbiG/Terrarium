[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load GDT
    lidt [gdt_descriptor]

    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Far jump to protected mode code
    jmp CODE_SEG:pm_start

[BITS 32]
pm_start:
    ; Set up segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; Enable PAE (Physical Address Extension) in CR4 for long mode
    mov eax, cr4
    or eax, 1 << 5         ; set PAE bit (bit 5)
    mov cr4, eax

    ; Load long mode page tables (identity mapping for now)
    mov eax, pdpt          ; load page directory pointer table address
    mov cr3, eax

    ; Enable long mode by setting LME (Long Mode Enable) bit in EFER MSR
    mov ecx, 0xC0000080    ; MSR EFER
    rdmsr
    or eax, 1 << 8         ; LME bit
    wrmsr

    ; Enable paging and long mode by setting PG and PE bits in CR0
    mov eax, cr0
    or eax, 1 << 31        ; PG bit (paging)
    or eax, 1              ; PE bit (protected mode already set)
    mov cr0, eax

    ; Far jump to 64-bit code segment
    jmp 0x08:long_mode_start

; Dummy page tables for identity mapping (needed for long mode)
align 4096
pdpt:
    dq pd | 0x3            ; PDPT entry with present and RW flags
    dq 0
    dq 0
    dq 0

align 4096
pd:
    dq 0x00000000 | 0x3    ; Page directory entry pointing to page table
    dq 0
    dq 0
    dq 0

align 4096
pt:
    dq 0x00000000 | 0x3    ; Page table entry with present and RW flags
    times 511 dq 0

[BITS 64]
long_mode_start:
    ; Now in 64-bit long mode
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; Your 64-bit code here
    ; e.g. call kernel main in 64-bit mode

.hang:
    hlt
    jmp .hang

; GDT with 64-bit code/data segments
gdt_start:
    dq 0x0000000000000000          ; null descriptor
    dq 0x00A09A000000FFFF          ; 64-bit code segment descriptor
    dq 0x00A092000000FFFF          ; 64-bit data segment descriptor
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

times 510-($-$$) db 0
dw 0xAA55
