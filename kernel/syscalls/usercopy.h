#ifndef TERRARIUM_KERNEL_SYSCALL_USERCOPY_H
#define TERRARIUM_KERNEL_SYSCALL_USERCOPY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Safe boundary for syscall access to caller memory. These functions must be
 * implemented by the VM/process layer using the calling process's address
 * space, page-permission checks, overflow checks and fault-safe copying.
 *
 * The current Terrarium tree does not yet have user address spaces or a real
 * syscall entry, so the weak defaults return TERRA_STATUS_NOSYS without ever
 * dereferencing the supplied address. New syscalls must use these helpers and
 * must never cast an untrusted uintptr_t directly to a kernel pointer.
 */
int terra_copy_from_user(void *kernel_dst, uintptr_t user_src, size_t size);
int terra_copy_to_user(uintptr_t user_dst, const void *kernel_src, size_t size);
int terra_copy_from_user_string(char *kernel_dst, size_t capacity,
                                uintptr_t user_src);

#ifdef __cplusplus
}
#endif
#endif
