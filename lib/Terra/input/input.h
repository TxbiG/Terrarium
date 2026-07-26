//                              MIT License

//                        Copyright (c) 2024 Toby

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <stdint.h>

/*
Keyboard and Refreshable braille display
Pointing devices: Mouse, Trackball, Touchpad, Pointing stick, Light pen
Touchscreen
Joystick, Gamepad, Analog stick
Fingerprint Scanner
*/


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef enum {
    EVENT_TYPE_KEYBOARD,
    EVENT_TYPE_MOUSE,
    EVENT_TYPE_CONTROLLER,
} EventType;

typedef struct {
    int8_t keycode;
    bool pressed;  // true for key down, false for key up
} KeyboardEvent;

typedef struct {
    int8_t dx, dy;
} MouseEvent;
typedef struct {
    int8_t dx, dy;
    uint8_t buttons; // bit 0 = left, 1 = right, 2 = middle
} MouseEvent;


typedef struct {
    int axis_x;
    int axis_y;
    bool button_a;
    bool button_b;
    // etc.
} ControllerEvent;

typedef struct {
    uint64_t timestamp;  // milliseconds
    EventType type;
    union {
        KeyboardEvent keyboard;
        MouseEvent mouse;
        ControllerEvent controller;
    } data;
} Event;

#ifdef __cplusplus
}
#endif  // __cplusplus