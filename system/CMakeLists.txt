add_library(terrarium-system STATIC
    runtime/syscall.c
    runtime/init.c
    runtime/process.c
    runtime/service.c
    hardware/hwinfo.c
    net/ip.c
    net/net.c
    net/route.c
    net/dns.c
    net/firewall.c
    net/ping.c
    shell/shell.c
    shell/history.c
    shell/job_control.c
    fs/vfs.c
    fs/mount.c
    fs/block.c
    text/utf8.c
    dev/backtrace.c
    dev/symbols.c
    dev/debug.c
    recovery/recovery.c
    log/crash.c
    auth/caps.c
    auth/secrets.c
    auth/sandbox.c
    auth/audit.c
    package/signature.c
    package/update.c
    package/install.c
    package/repo.c
    package/package.c
    package/store.c
    utils/fs_util.c
    utils/disk_util.c
)

target_include_directories(terrarium-system PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(terrarium-system PUBLIC
    terrarium-lib
)






