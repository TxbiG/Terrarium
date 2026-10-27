#ifndef TERRARIUM_ARCH_IRQ_SPINLOCK_H
#define TERRARIUM_ARCH_IRQ_SPINLOCK_H

/*
 * Non-sleeping IRQ-safe spinlock for short metadata critical sections.
 *
 * Interrupts are disabled before acquiring the lock, preventing a local
 * interrupt from deadlocking on a lock held by the interrupted context.
 * atomic_flag serializes access between CPUs. The saved interrupt state lives
 * in the lock object while it is held; this is safe because only its owner can
 * access that field until the atomic flag is released.
 *
 * IMPORTANT: this primitive is not a substitute for scheduler-aware locking.
 * The current Terrarium tree has no working user-process scheduler/preemption
 * path. If preemption is introduced, integrate preempt-disable/enable or use
 * the kernel's canonical lock primitive before holding these locks in contexts
 * that may be preempted. Never hold this lock across blocking I/O or callbacks
 * that can sleep.
 */
#include <stdatomic.h>
#include <stdint.h>

typedef uintptr_t terra_irq_flags_t;

typedef struct terra_irq_spinlock {
    atomic_flag flag;
    terra_irq_flags_t owner_saved_flags;
} terra_irq_spinlock_t;

#define TERRA_IRQ_SPINLOCK_INIT { ATOMIC_FLAG_INIT, (terra_irq_flags_t)0 }

static inline terra_irq_flags_t terra_irq_spinlock_irq_save_disable(void)
{
#if defined(__x86_64__)
    uintptr_t flags;
    __asm__ volatile("pushfq; popq %0; cli" : "=r"(flags) : : "memory");
    return flags;
#elif defined(__i386__)
    uintptr_t flags;
    __asm__ volatile("pushfl; popl %0; cli" : "=r"(flags) : : "memory");
    return flags;
#elif defined(__aarch64__)
    uintptr_t flags;
    __asm__ volatile("mrs %0, daif\n\tmsr daifset, #2"
                     : "=r"(flags) : : "memory");
    return flags;
#elif defined(__arm__)
    uintptr_t flags;
    __asm__ volatile("mrs %0, cpsr\n\tcpsid i"
                     : "=r"(flags) : : "memory");
    return flags;
#elif defined(__riscv)
    uintptr_t flags;
    const uintptr_t sie = (uintptr_t)2;
    __asm__ volatile("csrrc %0, sstatus, %1"
                     : "=r"(flags) : "r"(sie) : "memory");
    return flags;
#else
# error "Terrarium IRQ spinlock needs an implementation for this architecture"
#endif
}

static inline void terra_irq_spinlock_irq_restore(terra_irq_flags_t flags)
{
#if defined(__x86_64__) || defined(__i386__)
    if (flags & ((uintptr_t)1u << 9))
        __asm__ volatile("sti" : : : "memory");
    else
        __asm__ volatile("cli" : : : "memory");
#elif defined(__aarch64__)
    __asm__ volatile("msr daif, %0" : : "r"(flags) : "memory");
#elif defined(__arm__)
    __asm__ volatile("msr cpsr_c, %0" : : "r"(flags) : "memory");
#elif defined(__riscv)
    const uintptr_t sie = (uintptr_t)2;
    if (flags & sie)
        __asm__ volatile("csrs sstatus, %0" : : "r"(sie) : "memory");
    else
        __asm__ volatile("csrc sstatus, %0" : : "r"(sie) : "memory");
#else
# error "Terrarium IRQ spinlock needs an implementation for this architecture"
#endif
}

static inline void terra_irq_spin_lock(terra_irq_spinlock_t *lock)
{
    terra_irq_flags_t flags;
    if (!lock)
        return;
    flags = terra_irq_spinlock_irq_save_disable();
    while (atomic_flag_test_and_set_explicit(&lock->flag, memory_order_acquire)) {
#if defined(__x86_64__) || defined(__i386__)
        __asm__ volatile("pause" : : : "memory");
#elif defined(__aarch64__) || defined(__arm__)
        __asm__ volatile("yield" : : : "memory");
#else
        __asm__ volatile("nop" : : : "memory");
#endif
    }
    lock->owner_saved_flags = flags;
}

static inline void terra_irq_spin_unlock(terra_irq_spinlock_t *lock)
{
    terra_irq_flags_t flags;
    if (!lock)
        return;
    flags = lock->owner_saved_flags;
    atomic_flag_clear_explicit(&lock->flag, memory_order_release);
    terra_irq_spinlock_irq_restore(flags);
}

#endif
