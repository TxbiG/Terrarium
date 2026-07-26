/*
 * TerrariumOS RISC-V final kernel jump shim.
 * a0 receives the handoff pointer.
 */

.global terra_arch_enter_kernel
terra_arch_enter_kernel:
    mv t0, a0
    mv a0, a1
    jr t0
