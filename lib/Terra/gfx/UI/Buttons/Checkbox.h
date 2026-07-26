#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct
{
    bool disabled, button_pressed;
} CHECKBOX;

bool Checkbox();

#ifdef __cplusplus
}
#endif  // __cplusplus