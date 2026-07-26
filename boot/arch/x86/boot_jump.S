/*
 * TerrariumOS x86/x86_64 final kernel jump shims.
 * These symbols are intentionally small ABI bridges used by platform backends.
 */

#if defined(__x86_64__)
.global terra_arch_enter_kernel
terra_arch_enter_kernel:
    /* rdi = entry, rsi = handoff. SysV kernel ABI wants handoff in rdi. */
    mov %rdi, %rax
    mov %rsi, %rdi
    cli
    jmp *%rax
#else
.global terra_arch_enter_kernel
terra_arch_enter_kernel:
    /* cdecl: [esp+4] entry, [esp+8] handoff. Kernel receives handoff on stack. */
    mov 4(%esp), %eax
    mov 8(%esp), %edx
    cli
    push %edx
    jmp *%eax
#endif
