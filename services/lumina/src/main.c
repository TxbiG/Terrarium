#include "compositor.h"
#include "input.h"
#include "xdg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char *program) {
    printf("usage: %s [--status|--once|--run|--frames N|--key KEYCODE [up|down]|--configure ID W H [--ack]|--ack ID SERIAL|--request-close ID|--close ID]\n", program);
}

static int run_frames(lumina_compositor_t *compositor, unsigned frames) {
    for (unsigned i = 0; compositor && compositor->running && i < frames; ++i)
        lumina_compositor_run_once(compositor);
    lumina_compositor_print_status(compositor);
    return 0;
}

static unsigned pending_serial_for_view(const lumina_compositor_t *compositor, unsigned view_id) {
    if (!compositor)
        return 0;

    for (size_t i = 0; i < compositor->view_count; ++i) {
        if (compositor->views[i].id == view_id)
            return compositor->views[i].pending_configure_serial;
    }

    return 0;
}

int main(int argc, char **argv) {
    const char *mode = argc > 1 ? argv[1] : "--once";
    lumina_compositor_t *compositor = lumina_compositor_create();

    if (!compositor) {
        fprintf(stderr, "terrarium-lumina: failed to create compositor\n");
        return 1;
    }

    if (strcmp(mode, "--status") == 0 || strcmp(mode, "--once") == 0) {
        run_frames(compositor, 1);
        lumina_compositor_destroy(compositor);
        return 0;
    }

    if (strcmp(mode, "--key") == 0 && argc > 2) {
        unsigned keycode = (unsigned)strtoul(argv[2], 0, 10);
        int pressed = argc <= 3 || strcmp(argv[3], "up") != 0;
        lumina_input_handle_key(compositor, keycode, pressed);
        run_frames(compositor, 1);
        lumina_compositor_destroy(compositor);
        return 0;
    }

    if (strcmp(mode, "--configure") == 0 && argc > 4) {
        unsigned view_id = (unsigned)strtoul(argv[2], 0, 10);
        int width = atoi(argv[3]);
        int height = atoi(argv[4]);
        unsigned serial = lumina_xdg_configure_view(compositor, view_id, width, height);
        if (serial == 0) {
            fprintf(stderr, "terrarium-lumina: view %u not found\n", view_id);
            lumina_compositor_destroy(compositor);
            return 1;
        }
        printf("terrarium-lumina: configured view %u serial=%u\n", view_id, serial);
        if (argc > 5 && strcmp(argv[5], "--ack") == 0 && lumina_xdg_ack_configure(compositor, view_id, serial) != 0) {
            fprintf(stderr, "terrarium-lumina: failed to ack configure serial %u\n", serial);
            lumina_compositor_destroy(compositor);
            return 1;
        }
        run_frames(compositor, 1);
        lumina_compositor_destroy(compositor);
        return 0;
    }

    if (strcmp(mode, "--ack") == 0 && argc > 3) {
        unsigned view_id = (unsigned)strtoul(argv[2], 0, 10);
        unsigned serial = (unsigned)strtoul(argv[3], 0, 10);
        if (lumina_xdg_ack_configure(compositor, view_id, serial) != 0) {
            fprintf(stderr, "terrarium-lumina: failed to ack configure serial %u for view %u\n", serial, view_id);
            lumina_compositor_destroy(compositor);
            return 1;
        }
        run_frames(compositor, 1);
        lumina_compositor_destroy(compositor);
        return 0;
    }

    if (strcmp(mode, "--request-close") == 0 && argc > 2) {
        unsigned view_id = (unsigned)strtoul(argv[2], 0, 10);
        unsigned serial = pending_serial_for_view(compositor, view_id);
        if ((serial != 0 && lumina_xdg_ack_configure(compositor, view_id, serial) != 0) ||
            lumina_xdg_request_close(compositor, view_id) != 0) {
            fprintf(stderr, "terrarium-lumina: view %u not ready for close request\n", view_id);
            lumina_compositor_destroy(compositor);
            return 1;
        }
        run_frames(compositor, 1);
        lumina_compositor_destroy(compositor);
        return 0;
    }

    if (strcmp(mode, "--close") == 0 && argc > 2) {
        unsigned view_id = (unsigned)strtoul(argv[2], 0, 10);
        unsigned serial = pending_serial_for_view(compositor, view_id);
        if (serial != 0)
            lumina_xdg_ack_configure(compositor, view_id, serial);
        if (lumina_xdg_close_view(compositor, view_id) != 0) {
            fprintf(stderr, "terrarium-lumina: view %u not found\n", view_id);
            lumina_compositor_destroy(compositor);
            return 1;
        }
        run_frames(compositor, 1);
        lumina_compositor_destroy(compositor);
        return 0;
    }

    if (strcmp(mode, "--frames") == 0 && argc > 2) {
        unsigned frames = (unsigned)strtoul(argv[2], 0, 10);
        if (frames == 0)
            frames = 1;
        run_frames(compositor, frames);
        lumina_compositor_destroy(compositor);
        return 0;
    }

    if (strcmp(mode, "--run") == 0) {
        printf("terrarium-lumina: compositor service started\n");
        while (compositor->running)
            lumina_compositor_run_once(compositor);
        lumina_compositor_destroy(compositor);
        return 0;
    }

    print_usage(argv[0]);
    lumina_compositor_destroy(compositor);
    return 2;
}


