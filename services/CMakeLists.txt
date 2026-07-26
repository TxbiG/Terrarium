option(TERRARIUM_BUILD_SERVICES "Build TerrariumOS user-space services" ON)
option(TERRARIUM_BUILD_GUI_SERVICES "Build compositor and desktop services" OFF)
option(TERRARIUM_BUILD_NOVA "Build the Nova desktop shell service" ${TERRARIUM_BUILD_GUI_SERVICES})

add_custom_target(terrarium-services)

if(NOT TERRARIUM_BUILD_SERVICES)
    return()
endif()

add_subdirectory(shell)
add_executable(terrarium-init
    init/src/main.c
    init/src/service_manager.c
)

target_include_directories(terrarium-init PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/init/include
)

install(TARGETS terrarium-init
    DESTINATION sbin
)
install(DIRECTORY init/units/
    DESTINATION etc/terrarium/services
    FILES_MATCHING PATTERN "*.service" PATTERN "manifest.txt"
)

add_executable(terrarium-login
    login/src/main.c
    login/src/auth.c
    login/src/session.c
)

target_include_directories(terrarium-login PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/login/include
)

install(TARGETS terrarium-login
    DESTINATION sbin
)
install(FILES login/users.conf
    DESTINATION etc/terrarium/login
)
install(DIRECTORY net/config/
    DESTINATION etc/terrarium/net
    FILES_MATCHING PATTERN "*.cfg"
)

function(add_terrarium_service target source include_dir)
    add_executable("${target}" "${source}")
    target_include_directories("${target}" PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/${include_dir}
    )
    install(TARGETS "${target}" DESTINATION sbin)
endfunction()

add_terrarium_service(terrarium-audiod audio/main.c audio)
add_terrarium_service(terrarium-fsd fs/main.c fs)
add_terrarium_service(terrarium-inputd input/main.c input)
add_terrarium_service(terrarium-launcherd launcher/main.c launcher)
add_terrarium_service(terrarium-logd log/main.c log)
add_terrarium_service(terrarium-netd net/main.c net)
add_terrarium_service(terrarium-dhcpd net/dhcpd/dhcpd.c net/dhcpd)
add_terrarium_service(terrarium-firewalld net/firewall/main.c net/firewall)
add_terrarium_service(terrarium-timed time/main.c time)

add_library(terrarium-service-placeholders INTERFACE)
target_include_directories(terrarium-service-placeholders INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/fs
    ${CMAKE_CURRENT_SOURCE_DIR}/input
    ${CMAKE_CURRENT_SOURCE_DIR}/launcher
    ${CMAKE_CURRENT_SOURCE_DIR}/log
    ${CMAKE_CURRENT_SOURCE_DIR}/net
    ${CMAKE_CURRENT_SOURCE_DIR}/time
)

if(TERRARIUM_BUILD_GUI_SERVICES)
    add_subdirectory(lumina)
endif()

if(TERRARIUM_BUILD_NOVA)
    add_subdirectory(nova)
endif()
add_dependencies(terrarium-services
    terrarium-init
    terrarium-shell
    terrarium-login
    terrarium-audiod
    terrarium-fsd
    terrarium-inputd
    terrarium-launcherd
    terrarium-logd
    terrarium-netd
    terrarium-dhcpd
    terrarium-firewalld
    terrarium-timed
)

if(TARGET terrarium-lumina)
    add_dependencies(terrarium-services terrarium-lumina)
endif()

if(TARGET terrarium-nova)
    add_dependencies(terrarium-services terrarium-nova)
endif()


