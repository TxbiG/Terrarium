#include "arch.h"

#if defined(KERNEL_ARCH_X86_64) || defined(KERNEL_ARCH_X86)
void x86_early_init(const terra_boot_info_t *boot_info);
void x86_late_init(const terra_boot_info_t *boot_info);
#elif defined(KERNEL_ARCH_AARCH64) || defined(KERNEL_ARCH_ARM32)
void arm_early_init(const terra_boot_info_t *boot_info);
void arm_late_init(const terra_boot_info_t *boot_info);
#elif defined(KERNEL_ARCH_RISCV64) || defined(KERNEL_ARCH_RISCV32)
void riscv_early_init(const terra_boot_info_t *boot_info);
void riscv_late_init(const terra_boot_info_t *boot_info);
#endif

void arch_early_init(const terra_boot_info_t *boot_info) {
#if defined(KERNEL_ARCH_X86_64) || defined(KERNEL_ARCH_X86)
    x86_early_init(boot_info);
#elif defined(KERNEL_ARCH_AARCH64) || defined(KERNEL_ARCH_ARM32)
    arm_early_init(boot_info);
#elif defined(KERNEL_ARCH_RISCV64) || defined(KERNEL_ARCH_RISCV32)
    riscv_early_init(boot_info);
#else
    (void)boot_info;
#endif
}

void arch_late_init(const terra_boot_info_t *boot_info) {
#if defined(KERNEL_ARCH_X86_64) || defined(KERNEL_ARCH_X86)
    x86_late_init(boot_info);
#elif defined(KERNEL_ARCH_AARCH64) || defined(KERNEL_ARCH_ARM32)
    arm_late_init(boot_info);
#elif defined(KERNEL_ARCH_RISCV64) || defined(KERNEL_ARCH_RISCV32)
    riscv_late_init(boot_info);
#else
    (void)boot_info;
#endif
}

void arch_halt(void) {
#if defined(__x86_64__) || defined(__i386__)
    __asm__ volatile("cli; hlt");
#else
    for (;;)
        ;
#endif
}

void arch_poweroff(void) {
    arch_halt();
}

void arch_reboot(void) {
    arch_halt();
}
