# TerrariumOS library surface.
#
# Most folders under lib are imported API/header trees or early userspace
# library work. Keep them as interface targets until each library has the
# platform, allocator, syscall, and window-system glue it needs to compile.

add_library(terrarium-lib INTERFACE)
target_include_directories(terrarium-lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Terra
    ${CMAKE_CURRENT_SOURCE_DIR}/Formats
)

add_library(terrarium-formats INTERFACE)
target_include_directories(terrarium-formats INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Formats
)

add_library(terrarium-stb INTERFACE)
target_include_directories(terrarium-stb INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb
)

add_library(terrarium-ui STATIC
    Terra/gfx/UI/Desktop.cpp
    Terra/gfx/UI/Taskbar.cpp
    Terra/gfx/UI/Widget/Widget.cpp
    Terra/gfx/UI/Window/Window.cpp
    Terra/gfx/UI/InputFields/TextInput.cpp
)
target_include_directories(terrarium-ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Terra
)


add_library(terrarium-window-client STATIC
    Terra/window/window_client.c
)
add_library(terrarium-app-runtime STATIC
    Terra/app/app.c
)
target_include_directories(terrarium-window-client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Terra
)
target_include_directories(terrarium-app-runtime PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Terra
)
target_link_libraries(terrarium-app-runtime PUBLIC terrarium-window-client)
add_library(terrarium-wayland-lite STATIC
    wayland/connection.c
    wayland/src/wayland-server_protocol.c
)
target_include_directories(terrarium-wayland-lite PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/wayland
    ${CMAKE_CURRENT_SOURCE_DIR}/wayland/src
)


option(TERRARIUM_BUILD_WAYLAND_WINDOW "Build the experimental Wayland-backed Terra window API" OFF)

if(TERRARIUM_BUILD_WAYLAND_WINDOW)
    add_library(terrarium-wayland-window STATIC
        Terra/window/Display/Window/Window.c
        Terra/window/Display/Window/xdg-shell-client-protocol.c
        wayland/src/wayland-client.c
        wayland/src/wayland-client-protocol.c
        wayland/src/wayland-util.c
    )
    target_include_directories(terrarium-wayland-window PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Terra
        ${CMAKE_CURRENT_SOURCE_DIR}/Terra/window/Display/Window
        ${CMAKE_CURRENT_SOURCE_DIR}/wayland/src
    )
endif()
add_library(terrarium-egl-headers INTERFACE)
target_include_directories(terrarium-egl-headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/egl/include
)

add_library(terrarium-vulkan-headers INTERFACE)
target_include_directories(terrarium-vulkan-headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/vulkan
)

if(TERRARIUM_ENABLE_MESA)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/TerrariumMesa.cmake)
endif()

add_library(terrarium-wayland-headers INTERFACE)
target_include_directories(terrarium-wayland-headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/wayland
    ${CMAKE_CURRENT_SOURCE_DIR}/wayland/src
)

add_library(terrarium-wlroots-headers INTERFACE)
target_include_directories(terrarium-wlroots-headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/wlroots/include
)

target_link_libraries(terrarium-lib INTERFACE
    terrarium-ui
    terrarium-window-client
    terrarium-wayland-lite
    terrarium-formats
    terrarium-stb
    terrarium-egl-headers
    terrarium-vulkan-headers
    terrarium-wayland-headers
    terrarium-wlroots-headers
)

if(TERRARIUM_ENABLE_MESA)
    target_link_libraries(terrarium-lib INTERFACE Terrarium::Mesa)
endif()

# Compatibility target for existing CMake references. This is intentionally an
# interface target until a real libc source tree exists under lib/src.
add_library(libc-user INTERFACE)
target_link_libraries(libc-user INTERFACE terrarium-lib)


