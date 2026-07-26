add_executable(terrarium-nova
    src/main.c
    src/statusbar.c
    src/network-tray.c
    src/workspaces.c
    src/protocol-client.c
)

target_include_directories(terrarium-nova PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/etc
    ${CMAKE_CURRENT_SOURCE_DIR}/protocols
    ${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(terrarium-nova PRIVATE
    terrarium-ui
    terrarium-lib
)

set_target_properties(terrarium-nova PROPERTIES
    LINKER_LANGUAGE CXX
)

install(TARGETS terrarium-nova DESTINATION sbin)

