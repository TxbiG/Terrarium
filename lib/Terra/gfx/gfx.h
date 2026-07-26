#ifndef TERRARIUM_GFX_H
#define TERRARIUM_GFX_H


typedef struct {
    int x, y, width, height;
} recti_t;

typedef struct {
    float x, y, width, height;
} rect_t;

typedef struct {
    float h;
    float s;
    float v;
} hsv_t;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} color_t;

typedef struct {
  
} texture_t;

typedef struct {
  
} surface_t;

#endif // TERRARIUM_GFX_H
