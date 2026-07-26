add_executable(terrarium-shell
    src/main.c
    src/shell.c
    src/exec.c
    src/builtins.c
    src/script.c
)

target_include_directories(terrarium-shell PRIVATE
    include
)

install(TARGETS terrarium-shell
    DESTINATION bin
)
