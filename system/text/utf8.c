#include "utf8.h"

static int utf8_expected_length(unsigned char first) {
    if (first < 0x80)
        return 1;
    if ((first & 0xE0) == 0xC0)
        return 2;
    if ((first & 0xF0) == 0xE0)
        return 3;
    if ((first & 0xF8) == 0xF0)
        return 4;
    return 0;
}

int terra_utf8_decode_one(const char **text, uint32_t *out_codepoint) {
    if (!text || !*text || !out_codepoint)
        return 0;

    const unsigned char *s = (const unsigned char *)*text;
    int len = utf8_expected_length(s[0]);
    if (len == 0)
        return 0;

    uint32_t cp = 0;
    if (len == 1) {
        cp = s[0];
    } else {
        uint32_t min_cp = len == 2 ? 0x80u : (len == 3 ? 0x800u : 0x10000u);
        cp = s[0] & (uint32_t)(0x7Fu >> len);
        for (int i = 1; i < len; ++i) {
            if ((s[i] & 0xC0) != 0x80)
                return 0;
            cp = (cp << 6) | (uint32_t)(s[i] & 0x3F);
        }
        if (cp < min_cp || cp > 0x10FFFFu || (cp >= 0xD800u && cp <= 0xDFFFu))
            return 0;
    }

    *out_codepoint = cp;
    *text += len;
    return len;
}

int terra_utf8_validate(const char *text) {
    if (!text)
        return 0;

    while (*text) {
        uint32_t cp;
        if (terra_utf8_decode_one(&text, &cp) == 0)
            return 0;
    }

    return 1;
}

unsigned long terra_utf8_length(const char *text) {
    if (!text)
        return 0;

    unsigned long count = 0;
    while (*text) {
        uint32_t cp;
        if (terra_utf8_decode_one(&text, &cp) == 0)
            return 0;
        ++count;
    }

    return count;
}
