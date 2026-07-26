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

#define KEY_0 0
#define KEY_1 2
#define KEY_2 3
#define KEY_3 4
#define KEY_4 5
#define KEY_5 6
#define KEY_6 7
#define KEY_7 8
#define KEY_8 9
#define KEY_9 10
#define KEY_A 11
#define KEY_B 12
#define KEY_C 13
#define KEY_D 14
#define KEY_E 15
#define KEY_F 16
#define KEY_G 17
#define KEY_H 18
#define KEY_I 19
#define KEY_J 20
#define KEY_K 21
#define KEY_L 22
#define KEY_M 23
#define KEY_N 24
#define KEY_O 25
#define KEY_P 26
#define KEY_Q 27
#define KEY_R 28
#define KEY_S 29
#define KEY_T 30
#define KEY_U 31
#define KEY_V 32
#define KEY_W 33
#define KEY_X 34
#define KEY_Y 35
#define KEY_Z 36

#define KEY_APOSTROPHE      37
#define KEY_BACKSLASH       38
#define KEY_COMMA           39
#define KEY_EQUAL           40
#define KEY_GRAVE_ACCENT    41
#define KEY_LEFT_BRACKET    42
#define KEY_MINUS           43
#define KEY_PERIOD          44
#define KEY_RIGHT_BRACKET   45
#define KEY_SEMICOLON       46
#define KEY_SLASH           47
#define KEY_WORLD_2         48

#define KEY_BACKSPACE       49
#define KEY_DELETE          50
#define KEY_END             51
#define KEY_ENTER           52
#define KEY_ESCAPE          53
#define KEY_HOME            54
#define KEY_INSERT          55
#define KEY_MENU            56
#define KEY_PAGE_DOWN       57
#define KEY_PAGE_UP         58
#define KEY_PAUSE           59
#define KEY_SPACE           60
#define KEY_TAB             61
#define KEY_CAPS_LOCK       62
#define KEY_NUM_LOCK        63
#define KEY_SCROLL_LOCK     64

#define KEY_F1  65
#define KEY_F2  66
#define KEY_F3  67
#define KEY_F4  68
#define KEY_F5  69
#define KEY_F6  70
#define KEY_F7  71
#define KEY_F8  72
#define KEY_F9  73
#define KEY_F10 74
#define KEY_F11 75
#define KEY_F12 76
#define KEY_F13 77
#define KEY_F14 78
#define KEY_F15 79
#define KEY_F16 80
#define KEY_F17 81
#define KEY_F18 82
#define KEY_F19 83
#define KEY_F20 84
#define KEY_F21 85
#define KEY_F22 86
#define KEY_F23 87
#define KEY_F24 88

#define KEY_LEFT_ALT        89
#define KEY_LEFT_CONTROL    90
#define KEY_LEFT_SHIFT      91
#define KEY_LEFT_SUPER      92

#define KEY_PRINT_SCREEN    93
#define KEY_RIGHT_ALT       94
#define KEY_RIGHT_CONTROL   95
#define KEY_RIGHT_SHIFT     96
#define KEY_RIGHT_SUPER     97

#define KEY_DOWN    98
#define KEY_LEFT    99
#define KEY_RIGHT   100
#define KEY_UP      101

#define KEY_KP_0        102
#define KEY_KP_1        103
#define KEY_KP_2        104
#define KEY_KP_3        105
#define KEY_KP_4        106
#define KEY_KP_5        107
#define KEY_KP_6        108
#define KEY_KP_7        109
#define KEY_KP_8        110
#define KEY_KP_9        111
#define KEY_KP_ADD      112
#define KEY_KP_DECIMAL  113
#define KEY_KP_DIVIDE   114
#define KEY_KP_ENTER    115
#define KEY_KP_EQUAL    116
#define KEY_KP_MULTIPLY 117
#define KEY_KP_SUBTRACT 118



bool action_pressed();
bool action_released();

bool action_just_pressed();
bool action_just_released();


#ifdef __cplusplus
}
#endif  // __cplusplus