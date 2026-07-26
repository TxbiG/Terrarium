; multiboot_headers.asm
section .multiboot_headers
    align 4

    ; ==== Multiboot1 header ====
    multiboot1_header:
        dd 0x1BADB002                  ; magic number
        dd 0x00010003                  ; flags: bit 0 (mem info), bit 1 (boot device), bit 16 (a.out symbols)
        dd -(0x1BADB002 + 0x00010003) ; checksum

    ; ==== Multiboot2 header ====
    multiboot2_header:
        dd 0xE85250D6                  ; magic number
        dd 24                          ; total header length (example)
        dd 0x00000003                  ; flags: bit 0 (mem info), bit 1 (boot device)
        
        ; Tag 1: Boot command line (optional, null terminated)
        dd 1                           ; tag type = command line
        dd 12                          ; size including header
        db 'root=/dev/sda1', 0         ; command line string + null byte
        align 4

        ; Tag 0: End tag
        dd 0
        dd 8
