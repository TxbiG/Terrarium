add_executable(terrarium-lumina
    src/main.c
    src/server.c
    src/output.c
    src/input.c
    src/surface.c
    src/scene.c
    src/xdg.c
    src/wm.c
    src/layout.c
    src/keybinds.c
    src/decorations.c
)

target_include_directories(terrarium-lumina PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/lib
)

install(TARGETS terrarium-lumina
    DESTINATION sbin
)