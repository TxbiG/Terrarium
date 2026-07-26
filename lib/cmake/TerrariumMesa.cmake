# Mesa/OpenGL/Vulkan import boundary for TerrariumOS.
#
# TerrariumOS does not build Mesa from the vendored source snapshot yet. This
# file imports a Mesa install prefix that was cross-built for the Terrarium
# userspace ABI and exposes stable CMake targets to apps/services.

if(NOT TERRARIUM_ENABLE_MESA)
    return()
endif()

if(NOT IS_ABSOLUTE "${TERRARIUM_MESA_ROOT}")
    message(FATAL_ERROR
        "TERRARIUM_ENABLE_MESA requires TERRARIUM_MESA_ROOT to be an absolute "
        "path to a Mesa install prefix/sysroot.")
endif()

if(NOT EXISTS "${TERRARIUM_MESA_ROOT}")
    message(FATAL_ERROR
        "TERRARIUM_MESA_ROOT does not exist: ${TERRARIUM_MESA_ROOT}")
endif()

set(_TERRARIUM_MESA_INCLUDE_DIRS
    "${TERRARIUM_MESA_ROOT}/include"
    "${TERRARIUM_MESA_ROOT}/usr/include"
)

set(_TERRARIUM_MESA_LIBRARY_DIRS
    "${TERRARIUM_MESA_ROOT}/lib"
    "${TERRARIUM_MESA_ROOT}/lib64"
    "${TERRARIUM_MESA_ROOT}/usr/lib"
    "${TERRARIUM_MESA_ROOT}/usr/lib64"
)

find_path(TERRARIUM_MESA_EGL_INCLUDE_DIR
    NAMES EGL/egl.h
    PATHS ${_TERRARIUM_MESA_INCLUDE_DIRS}
    NO_DEFAULT_PATH
)

if(NOT TERRARIUM_MESA_EGL_INCLUDE_DIR)
    message(FATAL_ERROR
        "Could not find EGL/egl.h under TERRARIUM_MESA_ROOT: ${TERRARIUM_MESA_ROOT}")
endif()

function(_terrarium_import_shared_library target library_names)
    find_library(${target}_LIBRARY
        NAMES ${library_names}
        PATHS ${_TERRARIUM_MESA_LIBRARY_DIRS}
        NO_DEFAULT_PATH
    )

    if(NOT ${target}_LIBRARY)
        message(FATAL_ERROR
            "Could not find ${library_names} under TERRARIUM_MESA_ROOT: "
            "${TERRARIUM_MESA_ROOT}")
    endif()

    add_library(${target} UNKNOWN IMPORTED GLOBAL)
    set_target_properties(${target} PROPERTIES
        IMPORTED_LOCATION "${${target}_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${TERRARIUM_MESA_EGL_INCLUDE_DIR}"
    )
endfunction()

add_library(terrarium-mesa INTERFACE)
add_library(Terrarium::Mesa ALIAS terrarium-mesa)

if(TERRARIUM_MESA_OPENGL)
    _terrarium_import_shared_library(terrarium-mesa-egl "EGL;libEGL")
    add_library(Terrarium::EGL ALIAS terrarium-mesa-egl)

    _terrarium_import_shared_library(terrarium-mesa-opengl "GL;OpenGL;libGL;libOpenGL")
    add_library(Terrarium::OpenGL ALIAS terrarium-mesa-opengl)

    _terrarium_import_shared_library(terrarium-mesa-glesv2 "GLESv2;libGLESv2")
    add_library(Terrarium::GLESv2 ALIAS terrarium-mesa-glesv2)

    target_link_libraries(terrarium-mesa INTERFACE
        Terrarium::EGL
        Terrarium::OpenGL
        Terrarium::GLESv2
    )
endif()

if(TERRARIUM_MESA_VULKAN)
    find_path(TERRARIUM_MESA_VULKAN_INCLUDE_DIR
        NAMES vulkan/vulkan.h
        PATHS ${_TERRARIUM_MESA_INCLUDE_DIRS}
        NO_DEFAULT_PATH
    )

    if(NOT TERRARIUM_MESA_VULKAN_INCLUDE_DIR)
        message(FATAL_ERROR
            "Could not find vulkan/vulkan.h under TERRARIUM_MESA_ROOT: "
            "${TERRARIUM_MESA_ROOT}")
    endif()

    _terrarium_import_shared_library(terrarium-mesa-vulkan "vulkan;Vulkan;libvulkan")
    set_target_properties(terrarium-mesa-vulkan PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${TERRARIUM_MESA_VULKAN_INCLUDE_DIR}"
    )
    add_library(Terrarium::Vulkan ALIAS terrarium-mesa-vulkan)
    target_link_libraries(terrarium-mesa INTERFACE Terrarium::Vulkan)
endif()

foreach(_terrarium_mesa_runtime_dir IN LISTS _TERRARIUM_MESA_LIBRARY_DIRS)
    if(EXISTS "${_terrarium_mesa_runtime_dir}")
        install(DIRECTORY "${_terrarium_mesa_runtime_dir}/"
            DESTINATION lib
            FILES_MATCHING
                PATTERN "*.so"
                PATTERN "*.so.*"
                PATTERN "*.dll"
                PATTERN "*.dylib"
        )
    endif()
endforeach()

foreach(_terrarium_mesa_vulkan_dir
        "${TERRARIUM_MESA_ROOT}/share/vulkan"
        "${TERRARIUM_MESA_ROOT}/usr/share/vulkan")
    if(EXISTS "${_terrarium_mesa_vulkan_dir}")
        install(DIRECTORY "${_terrarium_mesa_vulkan_dir}/"
            DESTINATION share/vulkan
        )
    endif()
endforeach()

unset(_TERRARIUM_MESA_INCLUDE_DIRS)
unset(_TERRARIUM_MESA_LIBRARY_DIRS)