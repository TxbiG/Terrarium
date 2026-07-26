cmake_minimum_required(VERSION 3.20)

option(TERRARIUM_BUILD_SAMPLE_APPS "Build bundled TerrariumOS application sources" ON)
option(TERRARIUM_BUILD_BASE_APPS "Build applications selected for the base image" ON)
option(TERRARIUM_BUILD_EXPERIMENTAL_APPS "Build prototype/experimental graphical applications" OFF)

if(NOT TERRARIUM_BUILD_SAMPLE_APPS)
    return()
endif()

add_custom_target(terrarium-apps)

function(add_terrarium_app target source)
    add_executable("${target}" "${source}")
    target_include_directories("${target}" PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../lib
        ${CMAKE_CURRENT_SOURCE_DIR}/../system
    )
    target_link_libraries("${target}" PRIVATE
        terrarium-lib
        terrarium-system
        terrarium-app-runtime
    )
    install(TARGETS "${target}" DESTINATION apps)
    add_dependencies(terrarium-apps "${target}")
endfunction()

if(TERRARIUM_BUILD_BASE_APPS)
    add_terrarium_app(terrarium-calculator Calculator.c)
    add_terrarium_app(terrarium-terminal Terminal.c)
    add_terrarium_app(terrarium-task-manager "Task Manager.c")
    add_terrarium_app(terrarium-file-manager "File Manager.c")
    add_terrarium_app(terrarium-notes Notes.c)
    add_terrarium_app(terrarium-settings Settings/main.c)
    install(FILES base-apps.manifest DESTINATION etc/terrarium/apps)
endif()

if(TERRARIUM_BUILD_EXPERIMENTAL_APPS)
    add_terrarium_app(terrarium-bin Bin.c)
    add_terrarium_app(terrarium-compass Compass.c)
    add_terrarium_app(terrarium-media-player "Media Player.c")
    add_terrarium_app(terrarium-paint Paint.c)
    add_terrarium_app(terrarium-photos Photos.c)
    add_terrarium_app(terrarium-health Health/main.c)
    add_terrarium_app(terrarium-horizon Horizon/main.c)
    add_terrarium_app(terrarium-maps Maps/main.c)
    add_terrarium_app(terrarium-docs "Terrarium Docs/main.c")
    add_terrarium_app(terrarium-mail "Terrarium Mail/main.c")
    add_terrarium_app(terrarium-pdf "Terrarium PDF/main.c")
    add_terrarium_app(terrarium-sheets "Terrarium Sheets/main.c")
    add_terrarium_app(terrarium-slides "Terrarium Slides/main.c")
    add_terrarium_app(terrarium-chess Games/Chess.c)
    add_terrarium_app(terrarium-checkers Games/Checkers.c)
    add_terrarium_app(terrarium-dropper Games/Dropper.c)
    add_terrarium_app(terrarium-getaway Games/Getaway.c)
    add_terrarium_app(terrarium-invaders Games/Invaders.c)
    add_terrarium_app(terrarium-snake Games/Snake.c)
    add_terrarium_app(terrarium-swift-board Tools/SwiftBoard/main.cpp)
    add_terrarium_app(terrarium-swift-clip Tools/SwiftClip/main.c)
    add_terrarium_app(terrarium-swift-record Tools/SwiftRecord/main.cpp)
endif()





