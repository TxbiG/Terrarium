#ifndef TERRARIUM_SYSTEM_TEXT_REGEX_H
#define TERRARIUM_SYSTEM_TEXT_REGEX_H

typedef struct terra_regex {
    void *opaque;
} terra_regex_t;

int terra_regex_compile(const char *pattern, terra_regex_t *out_regex);
int terra_regex_match(const terra_regex_t *regex, const char *text);
void terra_regex_free(terra_regex_t *regex);

#endif
