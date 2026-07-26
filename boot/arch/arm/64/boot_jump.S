/*
 * TerrariumOS AArch64 final kernel jump shim.
 * x0 receives the handoff pointer.
 */

.global terra_arch_enter_kernel
terra_arch_enter_kernel:
    mov x2, x0
    mov x0, x1
    br x2
