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


typedef uint32_t finger_id_t;

enum Moss_Gesture {
    GESTURE_NONE        = 0<<0,     // No gesture
    GESTURE_TAP         = 1<<0,     // Tap gesture
    GESTURE_DOUBLETAP   = 2<<0,     // Double tap gesture
    GESTURE_HOLD        = 3<<0,     // Hold gesture
    GESTURE_DRAG        = 4<<0,     // Drag gesture
    GESTURE_SWIPE_RIGHT = 5<<0,     // Swipe right gesture
    GESTURE_SWIPE_LEFT  = 6<<0,     // Swipe left gesture
    GESTURE_SWIPE_UP    = 7<<0,     // Swipe up gesture
    GESTURE_SWIPE_DOWN  = 8<<0,     // Swipe down gesture
    GESTURE_PINCH_IN    = 9<<0,     // Pinch in gesture
    GESTURE_PINCH_OUT   = 10<<0     // Pinch out gesture
};

typedef enum {
    INPUT_TOUCH_DOWN,
    INPUT_TOUCH_UP,
    INPUT_TOUCH_MOVE
} touch_type_t;

typedef enum {
    INVALID = -1,
    DIRECT,            /**< touch screen with window-relative coordinates */
    INDIRECT_ABSOLUTE, /**< trackpad with absolute device coordinates */
    INDIRECT_RELATIVE  /**< trackpad with screen cursor-relative coordinates */
} touch_device_type_t;

typedef struct {
    finger_id_t id;  /**< the finger ID */
    float x;  /**< the x-axis location of the touch event, normalized (0...1) */
    float y;  /**< the y-axis location of the touch event, normalized (0...1) */
    float pressure; /**< the quantity of pressure applied, normalized (0...1) */
} finger_t;


#ifdef __cplusplus
}
#endif  // __cplusplus
