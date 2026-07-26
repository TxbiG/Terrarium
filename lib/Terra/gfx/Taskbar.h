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

#define TASKBAR_POSITION_BOTTOM 0
#define TASKBAR_POSITION_TOP    (1 << 0)
#define TASKBAR_POSITION_LEFT   (2 << 0)
#define TASKBAR_POSITION_RIGHT  (3 << 0)

#define CONTAINER_FLOAT_LEFT    0
#define CONTAINER_FLOAT_CENTER  (1 << 0)
#define CONTAINER_FLOAT_RIGHT;  (2 << 0)

typedef struct { int x,y,w,h; } Rect;
typedef struct { int r,g,b,a; } Colour;


typedef struct {
    short float_type;
    // Contents

    int margin;
    int padding;

    Rect rect;
    Colour colour;
} Container;


typedef struct {
    short position_type;

    int margin;
    int padding;

    Rect rect;
    Colour colour;

    Container m_containers[];
} Taskbar;

// Initialize a Taskbar
void init_taskbar(Taskbar* taskbar, short position_type, int margin, int padding, Rect rect, Colour colour, Container* containers);

void update_taskbar(Taskbar* taskbar, short position_type, int margin, int padding, Rect rect, Colour colour, Container* containers);
void edit_taskbar(Taskbar* taskbar, short position_type, int margin, int padding, Rect rect, Colour colour, Container* containers);

// Free resources for a Taskbar (if dynamically allocated containers are used)
void terminate_taskbar(Taskbar* taskbar);