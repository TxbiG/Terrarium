#ifndef TERRARIUM_SYSTEM_TEXT_UTF8_H
#define TERRARIUM_SYSTEM_TEXT_UTF8_H

#include <stdint.h>

int terra_utf8_validate(const char *text);
unsigned long terra_utf8_length(const char *text);
int terra_utf8_decode_one(const char **text, uint32_t *out_codepoint);

#endif
