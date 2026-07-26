#!/bin/bash
# Used to give visual display using EGL (Built off similarly to Waylands architecture)

gcc -o flora_compositor src/compositor.c src/surfaces.c src/protocol.c src/buffer.c -Iinclude
gcc -o flora_client src/client_main.c src/client_lib.c src/protocol.c src/buffer.c -Iinclude
