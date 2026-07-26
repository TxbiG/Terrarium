#ifndef TERRARIUM_UI_TEXT_INPUT_H
#define TERRARIUM_UI_TEXT_INPUT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_text_input {
    char *buffer;
    size_t capacity;
    size_t length;
    int editable;
    int focused;
} terra_text_input_t;

void terra_text_input_init(terra_text_input_t *input, char *buffer, size_t capacity);
void terra_text_input_clear(terra_text_input_t *input);
int terra_text_input_append_char(terra_text_input_t *input, char ch);
int terra_text_input_append_text(terra_text_input_t *input, const char *text);
int terra_text_input_backspace(terra_text_input_t *input);
const char *terra_text_input_text(const terra_text_input_t *input);

#ifdef __cplusplus
}
#endif

#endif
