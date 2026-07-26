/*
 * TerrariumOS BIOS real-mode interrupt bindings.
 *
 * This file is intentionally gated behind TERRA_BOOT_ENABLE_BIOS_REALMODE_BINDINGS.
 * BIOS interrupts must execute from a 16-bit real-mode thunk. The symbols below
 * are the firmware-side ABI targets used by boot/platform/x86_bios_realmode.c.
 *
 * The current boot tree does not yet provide the full protected/long-mode to
 * real-mode trampoline, so this assembly is kept out of default host builds.
 */

#if defined(TERRA_BOOT_ENABLE_BIOS_REALMODE_BINDINGS)

.code16
.section .text

.global terra_bios_rm_int13_read
terra_bios_rm_int13_read:
    /*
     * Input:
     *   stack/c ABI passes a pointer to terra_bios_int13_packet_t.
     * Firmware contract:
     *   caller/thunk must place the packet below 1 MiB and set DL to the boot
     *   drive before entering this routine.
     */
    pushw %bp
    movw %sp, %bp
    pushw %si
    movw 4(%bp), %si
    movb $0x42, %ah
    int $0x13
    jc 1f
    xorl %eax, %eax
    jmp 2f
1:
    movl $-1, %eax
2:
    popw %si
    popw %bp
    ret

.global terra_bios_rm_e820_collect
terra_bios_rm_e820_collect:
    /*
     * Input:
     *   entries, capacity, out_count.
     * Uses INT 15h E820. The thunk must make ES:DI addressable for entries.
     */
    pushw %bp
    movw %sp, %bp
    pushw %di
    pushw %ebx
    xorl %ebx, %ebx
    xorl %edx, %edx
    movl $0x534D4150, %edx
    xorl %ecx, %ecx
    movw 8(%bp), %cx
    xorl %eax, %eax
    movw %ax, 12(%bp)
3:
    cmpw $0, %cx
    je 5f
    movl $0xE820, %eax
    movl $24, %ecx
    int $0x15
    jc 4f
    cmpl $0x534D4150, %eax
    jne 4f
    addw $24, %di
    incl 12(%bp)
    testl %ebx, %ebx
    jz 5f
    jmp 3b
4:
    movl $-1, %eax
    jmp 6f
5:
    xorl %eax, %eax
6:
    popw %ebx
    popw %di
    popw %bp
    ret

.global terra_bios_rm_vbe_current_mode
terra_bios_rm_vbe_current_mode:
    /*
     * Placeholder for INT 10h VBE mode-info thunk. A later boot-sector thunk
     * should copy the VBE mode info block into terra_bios_vbe_mode_info_t.
     */
    movl $-1, %eax
    ret

.global terra_bios_rm_read_key
terra_bios_rm_read_key:
    movb $0x01, %ah
    int $0x16
    jz 7f
    xorb %ah, %ah
    int $0x16
    xorl %eax, %eax
    ret
7:
    movl $-1, %eax
    ret

.global terra_bios_rm_find_rsdp
terra_bios_rm_find_rsdp:
    /*
     * RSDP scanning lives better in C once EBDA/high BIOS memory are mapped.
     * Return 0 until the thunk exports the EBDA pointer safely.
     */
    xorl %eax, %eax
    xorl %edx, %edx
    ret

#endif
