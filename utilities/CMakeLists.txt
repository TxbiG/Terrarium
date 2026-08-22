cmake_minimum_required(VERSION 3.16)
project(terrarium_utilities C)

set(TERRARIUM_UTILITY_SOURCES
    io/cat.c
    io/clear.c
    io/echo.c
    io/less.c
    io/more.c
    dev/hexdump.c
    dev/nm.c
    dev/objdump.c
    dev/pstree.c
    dev/strace.c
    dev/time.c
    device/dmesg.c
    device/free.c
    device/iostat.c
    device/lsdev.c
    device/lspci.c
    device/lsusb.c
    device/vmstat.c
    kernel/klog.c
    kernel/kpanic.c
    kernel/ksysctl.c
    kernel/ksysinfo.c
    kernel/ktrace.c
    ipc/ipcrm.c
    ipc/ipcs.c
    ipc/ipcstat.c
    ipc/msgsend.c
    ipc/rpcinfo.c
    module/moddeps.c
    module/modinfo.c
    module/modlist.c
    module/modload.c
    module/modprobe.c
    module/modunload.c
    net/curl.c
    net/ifconfig.c
    net/ip.c
    net/netstat.c
    net/ping.c
    net/ss.c
    diagnostics/diagnostics.c
    recovery/recoveryctl.c
    package/updatectl.c
    package/pkg.c
    system/servicectl.c
    system/initctl.c
    device/hotplug.c
    device/drvinfo.c
    device/drvunbind.c
    device/drvbind.c
    memory/slabtop.c
    memory/pmap.c
    memory/swapctl.c
    security/sandboxctl.c
    security/auditctl.c
    permission/caps.c
    net/route.c
    net/ssh.c
    net/wget.c
    permission/chmod.c
    permission/chown.c
    permission/id.c
    permission/login.c
    permission/logout.c
    permission/passwd.c
    permission/su.c
    permission/sudo.c
    permission/whoami.c
    process/kill.c
    process/ps.c
    process/reboot.c
    process/shutdown.c
    process/suspend.c
    process/top.c
    process/uptime.c
    process/wait.c
    shell/alias.c
    shell/env.c
    shell/export.c
    shell/history.c
    shell/set.c
    shell/which.c
    storage/cp.c
    storage/df.c
    storage/du.c
    storage/fsck.c
    storage/ls.c
    storage/mkdir.c
    storage/mount.c
    storage/mv.c
    storage/pwd.c
    storage/rm.c
    storage/rmdir.c
    storage/stat.c
    storage/umount.c
    system/service.c
    system/sysctl.c
    system/terrarium.c
    system/uname.c
    text/cut.c
    text/diff.c
    text/find.c
    text/grep.c
    text/head.c
    text/sort.c
    text/tail.c
    text/uniq.c
    text/wc.c
)

foreach(source IN LISTS TERRARIUM_UTILITY_SOURCES)
    get_filename_component(command_name "${source}" NAME_WE)
    if(command_name STREQUAL "ps tree")
        set(command_name "pstree")
    endif()
    add_executable("${command_name}" "${source}")
    target_include_directories("${command_name}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries("${command_name}" PRIVATE terrarium-system)
    install(TARGETS "${command_name}" DESTINATION bin)
    list(APPEND TERRARIUM_UTILITY_TARGETS "${command_name}")
endforeach()

add_custom_target(terrarium-utilities)
add_dependencies(terrarium-utilities ${TERRARIUM_UTILITY_TARGETS})


