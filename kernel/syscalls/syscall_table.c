#include "syscall_table.h"

#include "../security/security.h"
#include "../fs/fs.h"
#include "../time/timekeeper.h"

static terra_syscall_fn_t syscall_table[TERRARIUM_MAX_SYSCALLS];
static terra_syscall_info_t syscall_info_table[TERRARIUM_MAX_SYSCALLS];
static terra_syscall_stats_t syscall_stats_table[TERRARIUM_MAX_SYSCALLS];
static int syscalls_ready;

static long syscall_nop(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return TERRARIUM_SYSCALL_OK;
}

static long syscall_abi_info(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;

    if (arg0) {
        uint32_t *out = (uint32_t *)arg0;
        out[0] = TERRARIUM_SYSCALL_ABI_VERSION;
        out[1] = TERRARIUM_MAX_SYSCALLS;
    }

    return TERRARIUM_SYSCALL_ABI_VERSION;
}

static long syscall_time_ns(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)time_ns();
}

static long syscall_time_ms(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)time_ms();
}

static long syscall_time_resolution(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)time_resolution_ns();
}


static long syscall_fs_open(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)terra_vfs_open((const char *)arg0, (uint32)arg1);
}

static long syscall_fs_close(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)terra_vfs_close((int)arg0);
}

static long syscall_fs_read(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)terra_vfs_read((int)arg0, (void *)arg1, (size_t)arg2);
}

static long syscall_fs_write(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)terra_vfs_write((int)arg0, (const void *)arg1, (size_t)arg2);
}

static long syscall_fs_seek(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)terra_vfs_seek((int)arg0, (uint64)arg1);
}

static long syscall_fs_stat(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return (long)terra_vfs_stat((const char *)arg0, (terra_vfs_stat_t *)arg1);
}

static long syscall_net_notready(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return TERRA_STATUS_NOTREADY;
}

static long syscall_service_notready(
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    (void)arg5;
    return TERRA_STATUS_NOTREADY;
}
static void clear_slot(uint32_t id) {
    syscall_table[id] = 0;
    syscall_info_table[id].id = id;
    syscall_info_table[id].name = 0;
    syscall_info_table[id].category = TERRA_SYSCALL_CATEGORY_CORE;
    syscall_info_table[id].arg_count = 0;
    syscall_info_table[id].flags = 0;
    syscall_stats_table[id].calls = 0;
    syscall_stats_table[id].denied = 0;
    syscall_stats_table[id].missing = 0;
    syscall_stats_table[id].bad_args = 0;
}

void syscalls_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRARIUM_MAX_SYSCALLS; ++i)
        clear_slot(i);

    const terra_syscall_info_t core_calls[] = {
        {TERRA_SYSCALL_NOP, "terra.nop", TERRA_SYSCALL_CATEGORY_CORE, 0, 0},
        {TERRA_SYSCALL_ABI_INFO, "terra.abi_info", TERRA_SYSCALL_CATEGORY_CORE, 1, 0},
        {TERRA_SYSCALL_TIME_NS, "terra.time_ns", TERRA_SYSCALL_CATEGORY_CORE, 0, 0},
        {TERRA_SYSCALL_TIME_MS, "terra.time_ms", TERRA_SYSCALL_CATEGORY_CORE, 0, 0},
        {TERRA_SYSCALL_TIME_RESOLUTION, "terra.time_resolution", TERRA_SYSCALL_CATEGORY_CORE, 0, 0},
        {TERRA_SYSCALL_FS_OPEN, "terra.fs.open", TERRA_SYSCALL_CATEGORY_FS, 2, 0},
        {TERRA_SYSCALL_FS_CLOSE, "terra.fs.close", TERRA_SYSCALL_CATEGORY_FS, 1, 0},
        {TERRA_SYSCALL_FS_READ, "terra.fs.read", TERRA_SYSCALL_CATEGORY_FS, 3, 0},
        {TERRA_SYSCALL_FS_WRITE, "terra.fs.write", TERRA_SYSCALL_CATEGORY_FS, 3, 0},
        {TERRA_SYSCALL_FS_SEEK, "terra.fs.seek", TERRA_SYSCALL_CATEGORY_FS, 2, 0},
        {TERRA_SYSCALL_FS_STAT, "terra.fs.stat", TERRA_SYSCALL_CATEGORY_FS, 2, 0},
        {TERRA_SYSCALL_SERVICE_START, "terra.service.start", TERRA_SYSCALL_CATEGORY_SERVICE, 1, 0},
        {TERRA_SYSCALL_SERVICE_STOP, "terra.service.stop", TERRA_SYSCALL_CATEGORY_SERVICE, 1, 0},
        {TERRA_SYSCALL_SERVICE_STATUS, "terra.service.status", TERRA_SYSCALL_CATEGORY_SERVICE, 2, 0},
        {TERRA_SYSCALL_SERVICE_LIST, "terra.service.list", TERRA_SYSCALL_CATEGORY_SERVICE, 3, 0},
    };

    syscalls_ready = 1;
    syscalls_register_info(&core_calls[0], syscall_nop);
    syscalls_register_info(&core_calls[1], syscall_abi_info);
    syscalls_register_info(&core_calls[2], syscall_time_ns);
    syscalls_register_info(&core_calls[3], syscall_time_ms);
    syscalls_register_info(&core_calls[4], syscall_time_resolution);
    syscalls_register_info(&core_calls[5], syscall_fs_open);
    syscalls_register_info(&core_calls[6], syscall_fs_close);
    syscalls_register_info(&core_calls[7], syscall_fs_read);
    syscalls_register_info(&core_calls[8], syscall_fs_write);
    syscalls_register_info(&core_calls[9], syscall_fs_seek);
    syscalls_register_info(&core_calls[10], syscall_fs_stat);
    syscalls_register_info(&core_calls[11], syscall_service_notready);
    syscalls_register_info(&core_calls[12], syscall_service_notready);
    syscalls_register_info(&core_calls[13], syscall_service_notready);
    syscalls_register_info(&core_calls[14], syscall_service_notready);

    const terra_syscall_info_t net_calls[] = {
        {TERRA_SYSCALL_NET_IF_LIST, "terra.net.if_list", TERRA_SYSCALL_CATEGORY_NETWORK, 2, 0},
        {TERRA_SYSCALL_NET_ROUTE_LIST, "terra.net.route_list", TERRA_SYSCALL_CATEGORY_NETWORK, 2, 0},
        {TERRA_SYSCALL_NET_ROUTE_ADD, "terra.net.route_add", TERRA_SYSCALL_CATEGORY_NETWORK, 1, 0},
        {TERRA_SYSCALL_NET_DNS_GET, "terra.net.dns_get", TERRA_SYSCALL_CATEGORY_NETWORK, 2, 0},
        {TERRA_SYSCALL_NET_DNS_SET, "terra.net.dns_set", TERRA_SYSCALL_CATEGORY_NETWORK, 2, 0},
        {TERRA_SYSCALL_NET_FIREWALL_LIST, "terra.net.firewall_list", TERRA_SYSCALL_CATEGORY_NETWORK, 2, 0},
        {TERRA_SYSCALL_NET_FIREWALL_ADD, "terra.net.firewall_add", TERRA_SYSCALL_CATEGORY_NETWORK, 1, 0},
        {TERRA_SYSCALL_NET_SOCKET_LIST, "terra.net.socket_list", TERRA_SYSCALL_CATEGORY_NETWORK, 2, 0},
        {TERRA_SYSCALL_NET_PACKET_DIAG, "terra.net.packet_diag", TERRA_SYSCALL_CATEGORY_NETWORK, 2, 0},
        {TERRA_SYSCALL_NET_PING, "terra.net.ping", TERRA_SYSCALL_CATEGORY_NETWORK, 2, 0},
    };
    for (uint32_t i = 0; i < sizeof(net_calls) / sizeof(net_calls[0]); ++i)
        syscalls_register_info(&net_calls[i], syscall_net_notready);
}

int syscalls_is_ready(void) {
    return syscalls_ready;
}

void syscalls_shutdown(void) {
    for (uint32_t i = 0; i < TERRARIUM_MAX_SYSCALLS; ++i)
        clear_slot(i);

    syscalls_ready = 0;
}

int syscalls_register(uint32_t id, terra_syscall_fn_t handler) {
    terra_syscall_info_t info;
    info.id = id;
    info.name = "terra.unnamed";
    info.category = TERRA_SYSCALL_CATEGORY_CORE;
    info.arg_count = 6;
    info.flags = 0;
    return syscalls_register_info(&info, handler);
}

int syscalls_register_info(const terra_syscall_info_t *info,
                           terra_syscall_fn_t handler) {
    if (!info)
        return TERRARIUM_SYSCALL_EINVAL;

    uint32_t id = info->id;
    if (id >= TERRARIUM_MAX_SYSCALLS || !handler)
        return TERRARIUM_SYSCALL_EINVAL;

    if (syscall_table[id])
        return TERRARIUM_SYSCALL_EBUSY;

    syscall_table[id] = handler;
    syscall_info_table[id] = *info;
    return TERRARIUM_SYSCALL_OK;
}

int syscalls_unregister(uint32_t id) {
    if (id >= TERRARIUM_MAX_SYSCALLS)
        return TERRARIUM_SYSCALL_EINVAL;

    clear_slot(id);
    return TERRARIUM_SYSCALL_OK;
}

const terra_syscall_info_t *syscalls_info(uint32_t id) {
    if (id >= TERRARIUM_MAX_SYSCALLS || !syscall_table[id])
        return 0;

    return &syscall_info_table[id];
}

int syscalls_stats(uint32_t id, terra_syscall_stats_t *out) {
    if (id >= TERRARIUM_MAX_SYSCALLS || !out)
        return TERRARIUM_SYSCALL_EINVAL;

    *out = syscall_stats_table[id];
    return TERRARIUM_SYSCALL_OK;
}

uint32_t syscalls_abi_version(void) {
    return TERRARIUM_SYSCALL_ABI_VERSION;
}

long syscalls_dispatch(
    uint32_t id,
    uintptr_t arg0,
    uintptr_t arg1,
    uintptr_t arg2,
    uintptr_t arg3,
    uintptr_t arg4,
    uintptr_t arg5) {
    if (!syscalls_ready || id >= TERRARIUM_MAX_SYSCALLS)
        return TERRARIUM_SYSCALL_EINVAL;

    syscall_stats_table[id].calls++;

    if (security_check_syscall(id) != TERRA_SECURITY_ALLOW) {
        syscall_stats_table[id].denied++;
        return TERRARIUM_SYSCALL_EACCES;
    }

    terra_syscall_fn_t handler = syscall_table[id];
    if (!handler) {
        syscall_stats_table[id].missing++;
        return TERRARIUM_SYSCALL_ENOSYS;
    }

    return handler(arg0, arg1, arg2, arg3, arg4, arg5);
}


