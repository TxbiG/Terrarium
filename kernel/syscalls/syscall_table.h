#ifndef TERRARIUM_SYSCALL_TABLE_H
#define TERRARIUM_SYSCALL_TABLE_H

#include <bootloader.h>
#include <status.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRARIUM_MAX_SYSCALLS 1024u
#define TERRARIUM_SYSCALL_ABI_VERSION 1u
#define TERRARIUM_SYSCALL_OK TERRA_STATUS_OK
#define TERRARIUM_SYSCALL_ENOSYS TERRA_STATUS_NOSYS
#define TERRARIUM_SYSCALL_EINVAL TERRA_STATUS_INVAL
#define TERRARIUM_SYSCALL_EACCES TERRA_STATUS_ACCES
#define TERRARIUM_SYSCALL_EBUSY TERRA_STATUS_BUSY

typedef enum terra_syscall_id {
    TERRA_SYSCALL_NOP = 0,
    TERRA_SYSCALL_ABI_INFO = 1,
    TERRA_SYSCALL_TIME_NS = 2,
    TERRA_SYSCALL_TIME_MS = 3,
    TERRA_SYSCALL_TIME_RESOLUTION = 4,

    TERRA_SYSCALL_FS_BASE = 100,
    TERRA_SYSCALL_FS_OPEN = 100,
    TERRA_SYSCALL_FS_CLOSE = 101,
    TERRA_SYSCALL_FS_READ = 102,
    TERRA_SYSCALL_FS_WRITE = 103,
    TERRA_SYSCALL_FS_SEEK = 104,
    TERRA_SYSCALL_FS_STAT = 105,

    TERRA_SYSCALL_PROC_BASE = 200,
    TERRA_SYSCALL_MEM_BASE = 300,
    TERRA_SYSCALL_IPC_BASE = 400,
    TERRA_SYSCALL_NET_BASE = 500,
    TERRA_SYSCALL_NET_IF_LIST = 500,
    TERRA_SYSCALL_NET_ROUTE_LIST = 501,
    TERRA_SYSCALL_NET_ROUTE_ADD = 502,
    TERRA_SYSCALL_NET_DNS_GET = 503,
    TERRA_SYSCALL_NET_DNS_SET = 504,
    TERRA_SYSCALL_NET_FIREWALL_LIST = 505,
    TERRA_SYSCALL_NET_FIREWALL_ADD = 506,
    TERRA_SYSCALL_NET_SOCKET_LIST = 507,
    TERRA_SYSCALL_NET_PACKET_DIAG = 508,
    TERRA_SYSCALL_NET_PING = 509,
    TERRA_SYSCALL_SECURITY_BASE = 600,

    TERRA_SYSCALL_HARDWARE_BASE = 700,
    TERRA_SYSCALL_HWINFO = 700,

    TERRA_SYSCALL_SERVICE_BASE = 800,
    TERRA_SYSCALL_SERVICE_START = 800,
    TERRA_SYSCALL_SERVICE_STOP = 801,
    TERRA_SYSCALL_SERVICE_STATUS = 802,
    TERRA_SYSCALL_SERVICE_LIST = 803
} terra_syscall_id_t;

typedef enum terra_syscall_category {
    TERRA_SYSCALL_CATEGORY_CORE = 0,
    TERRA_SYSCALL_CATEGORY_FS,
    TERRA_SYSCALL_CATEGORY_PROCESS,
    TERRA_SYSCALL_CATEGORY_MEMORY,
    TERRA_SYSCALL_CATEGORY_IPC,
    TERRA_SYSCALL_CATEGORY_NETWORK,
    TERRA_SYSCALL_CATEGORY_SECURITY,
    TERRA_SYSCALL_CATEGORY_DRIVER,
    TERRA_SYSCALL_CATEGORY_HARDWARE,
    TERRA_SYSCALL_CATEGORY_SERVICE
} terra_syscall_category_t;

typedef struct terra_syscall_info {
    uint32_t id;
    const char *name;
    terra_syscall_category_t category;
    uint8_t arg_count;
    uint32_t flags;
} terra_syscall_info_t;

typedef struct terra_syscall_stats {
    uint64_t calls;
    uint64_t denied;
    uint64_t missing;
    uint64_t bad_args;
} terra_syscall_stats_t;

typedef long (*terra_syscall_fn_t)(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5);

void syscalls_init(const terra_boot_info_t *boot_info);
int syscalls_is_ready(void);
void syscalls_shutdown(void);
int syscalls_register(uint32_t id, terra_syscall_fn_t handler);
int syscalls_register_info(const terra_syscall_info_t *info,
                           terra_syscall_fn_t handler);
int syscalls_unregister(uint32_t id);
const terra_syscall_info_t *syscalls_info(uint32_t id);
int syscalls_stats(uint32_t id, terra_syscall_stats_t *out);
uint32_t syscalls_abi_version(void);
long syscalls_dispatch(
    uint32_t id,
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5);

#ifdef __cplusplus
}
#endif

#endif
