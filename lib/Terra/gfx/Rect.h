#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct Recti {
    int x, y, w, h;
} RECTI;

typedef struct {
    float x, y, w, h;
} RECT;

static inline RECTI Recti(int x, int y, int width, int height) {
    RECTI rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    return rect;
}

static inline RECT Rect(float x, float y, float width, float height) {
    RECT rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    return rect;
}

#ifdef __cplusplus
}
#endif  // __cplusplus
