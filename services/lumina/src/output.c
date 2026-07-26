#include "output.h"
#include <stdio.h>
#include <string.h>

int lumina_output_add(lumina_compositor_t *compositor, const char *name, int width, int height) {
    lumina_output_t *output;

    if (!compositor || compositor->output_count >= LUMINA_MAX_OUTPUTS) {
        return -1;
    }

    output = &compositor->outputs[compositor->output_count];
    memset(output, 0, sizeof(*output));
    output->id = (unsigned)compositor->output_count + 1u;
    snprintf(output->name, sizeof(output->name), "%s", name ? name : "display");
    output->rect.width = width;
    output->rect.height = height;
    output->enabled = 1;
    compositor->output_count++;
    lumina_compositor_emit(compositor, TERRA_DISPLAY_EVENT_OUTPUT_ADDED, output->id);
    return 0;
}

void lumina_output_init_defaults(lumina_compositor_t *compositor) {
    if (!compositor || compositor->output_count > 0) {
        return;
    }

    lumina_output_add(compositor, "display-0", 1280, 720);
}