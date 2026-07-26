#include "TextInput.h"
#include <string.h>

void terra_text_input_init(terra_text_input_t *input, char *buffer, size_t capacity) {
    if (!input) {
        return;
    }

    input->buffer = buffer;
    input->capacity = capacity;
    input->length = 0;
    input->editable = 1;
    input->focused = 0;

    if (buffer && capacity > 0) {
        buffer[0] = '\0';
    }
}

void terra_text_input_clear(terra_text_input_t *input) {
    if (!input || !input->buffer || input->capacity == 0) {
        return;
    }

    input->length = 0;
    input->buffer[0] = '\0';
}

int terra_text_input_append_char(terra_text_input_t *input, char ch) {
    if (!input || !input->buffer || !input->editable || input->capacity == 0) {
        return -1;
    }

    if (input->length + 1 >= input->capacity) {
        return -1;
    }

    input->buffer[input->length++] = ch;
    input->buffer[input->length] = '\0';
    return 0;
}

int terra_text_input_append_text(terra_text_input_t *input, const char *text) {
    if (!text) {
        return 0;
    }

    while (*text) {
        if (terra_text_input_append_char(input, *text++) != 0) {
            return -1;
        }
    }

    return 0;
}

int terra_text_input_backspace(terra_text_input_t *input) {
    if (!input || !input->buffer || !input->editable || input->length == 0) {
        return -1;
    }

    input->buffer[--input->length] = '\0';
    return 0;
}

const char *terra_text_input_text(const terra_text_input_t *input) {
    if (!input || !input->buffer) {
        return "";
    }

    return input->buffer;
}
