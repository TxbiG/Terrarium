#ifndef TERRARIUM_SYSTEM_TEXT_JSON_H
#define TERRARIUM_SYSTEM_TEXT_JSON_H

typedef enum terra_json_type {
    TERRA_JSON_NULL = 0,
    TERRA_JSON_BOOL,
    TERRA_JSON_NUMBER,
    TERRA_JSON_STRING,
    TERRA_JSON_ARRAY,
    TERRA_JSON_OBJECT
} terra_json_type_t;

int terra_json_validate(const char *text);
int terra_json_get_string(const char *text, const char *path, char *out_value, unsigned long out_size);

#endif
