#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct
{
    bool disabled, button_pressed, button_hovered;

} RADIOBUTTON;

bool RadioButton();

#ifdef __cplusplus
}
#endif  // __cplusplus