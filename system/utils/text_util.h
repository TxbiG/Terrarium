#ifndef TERRARIUM_SYSTEM_UTILS_TEXT_UTIL_H
#define TERRARIUM_SYSTEM_UTILS_TEXT_UTIL_H

int terra_text_trim(char *text);
int terra_text_split_once(char *text, char delimiter, char **left, char **right);
int terra_text_starts_with(const char *text, const char *prefix);

#endif
