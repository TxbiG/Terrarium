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

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define GAMEPAD_BUTTON_A    0       // Xbox A, Sony X
#define GAMEPAD_BUTTON_B    1       // Xbox B Sony O
#define GAMEPAD_BUTTON_X    2       // Xbox X, Sony *Square*
#define GAMEPAD_BUTTON_Y    3       // Xbox Y, Sony *Triangle*
#define JOY_BUTTON_LB       4
#define JOY_BUTTON_RB       5
#define JOY_BUTTON_BACK     6
#define JOY_BUTTON_START    7
#define JOY_BUTTON_LS       8       // Left Stick Press
#define JOY_BUTTON_RS       9       // Right Stick Press

// Joystick axes
#define GAMEPAD_AXIS_LX     0       // Left Stick X Axis
#define GAMEPAD_AXIS_LY     1       // Left Stick Y Axis
#define GAMEPAD_AXIS_RX     2       // Right Stick X Axis
#define GAMEPAD_AXIS_RY     3       // Right Stick Y Axis
#define GAMEPAD_AXIS_LT     4       // Left Trigger
#define GAMEPAD_AXIS_RT     5       // Right Trigger


#ifdef __cplusplus
}
#endif  // __cplusplus