#ifndef TERRARIUM_GFX_COLOR_H
#define TERRARIUM_GFX_COLOR_H

#include "gfx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef hsv_t HSV;
typedef color_t COLOR;

static inline COLOR Color(float r, float g, float b, float a) {
    COLOR color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

static inline HSV hsv(float h, float s, float v) {
    HSV value;
    value.h = h;
    value.s = s;
    value.v = v;
    return value;
}

#ifdef __cplusplus
}
#endif

#endif
