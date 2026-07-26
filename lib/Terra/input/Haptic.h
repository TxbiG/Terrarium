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

#include "include/Controler.h"
#include "include/Monitor.h"
#include "include/Mouse.h"
#include "include/Keyboard.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef enum {
    AUTOCENTER,
    CARTESIAN,
    CONSTANT,
    CUSTOM,
    DAMPER,
    FRICTION,
    GAIN,
    INERTIA,
    INFINITY,
    LEFTRIGHT,
    PAUSE,
    POLAR,
    RAMP,
    RESERVED1,
    RESERVED2,
    RESERVED3,
    SAWTOOTHDOWN,
    SAWTOOTHUP,
    SINE,
    SPHERICAL,
    SPRING,
    SQUARE,
    STATUS,
    STEERING_AXIS,
    TRIANGLE
} haptic_feedback_type_t;

typedef enum { HAPTIC_CONSTANT, HAPTIC_PERIODIC, HAPTIC_CONDITION, HAPTIC_RAMP, HAPTIC_LEFTRIGHT, HAPTIC_CUSTOM } haptic_effect_type_t;

/*! @param type The type of encoding. @param dir The encoded direction. */
typedef struct { uint8_t type; int32_t dir[3]; } haptic_direction_t;

/*! @param type Header (HAPTIC_LEFTRIGHT). @param length Duration of the effect in milliseconds. (Replay) @param large_magnitude Control of the large controller motor. (Rumble) @param small_magnitude Control of the small controller motor. (Rumble) */
typedef struct { haptic_effect_type_t type; uint32_t length; uint16_t large_magnitude; uint16_t small_magnitude; } haptic_left_right_t;

/*! @param type e.g. HAPTIC_CUSTOM. (Header) @param direction Direction of the effect. (Header) @param length Duration of the effect. (Replay) @param delay Delay before starting the effect. (Replay) 
    @param button Button that triggers the effect. (Trigger)  @param interval How soon it can be triggered again after button. (Trigger) @param channels Axes to use, minimum of one. (Custom) 
    @param period Sample periods. (Custom) @param samples Amount of samples. (Custom) @param data Should contain channels*samples items. (Custom) 
    @param attack_length Duration of the attack. (Envelope) @param attack_level Level at the start of the attack. (Envelope) @param fade_length Duration of the fade. (Envelope) 
    @param fade_level Level at the end of the fade. (Envelope)*/
typedef struct { haptic_effect_type_t type; haptic_direction_t direction; uint32_t length; uint16_t delay; uint16_t button; 
    uint16_t interval;  uint8_t channels; uint16_t period; uint16_t samples; uint16_t *data;
    uint16_t attack_length; uint16_t attack_level; uint16_t fade_length; uint16_t fade_level;
} haptic_custom_t;

typedef struct {
    /* Header */
    haptic_effect_type_t type;      // HAPTIC_RAMP
    haptic_direction_t direction;  // Direction of the effect.

    /* Replay */
    uint32_t length;          // Duration of the effect.
    uint16_t delay;           // Delay before starting the effect.

    /* Trigger */
    uint16_t button;          //Button that triggers the effect.
    uint16_t interval;        // How soon it can be triggered again after button.

    /* Ramp */
    int16_t start;           // Beginning strength level.
    int16_t end;             // Ending strength level.

    /* Envelope */
    uint16_t attack_length;   // Duration of the attack.
    uint16_t attack_level;    // Level at the start of the attack.
    uint16_t fade_length;     // Duration of the fade.
    uint16_t fade_level;      // Level at the end of the fade.
} haptic_ramp_t;

typedef struct {
    /* Header */
    haptic_effect_type_t type;      // SDL_HAPTIC_CONSTANT */
    haptic_direction_t direction;  // Direction of the effect.

    /* Replay */
    uint32_t length;          // Duration of the effect.
    uint16_t delay;           // Delay before starting the effect.

    /* Trigger */
    uint16_t button;          // Button that triggers the effect.
    uint16_t interval;        // How soon it can be triggered again after button.

    /* Constant */
    int16_t level;           // Strength of the constant effect.

    /* Envelope */
    uint16_t attack_length;   // Duration of the attack.
    uint16_t attack_level;    // Level at the start of the attack.
    uint16_t fade_length;     // Duration of the fade.
    uint16_t fade_level;      // Level at the end of the fade.
} haptic_constant_t;

typedef struct {
    /* Header */
    haptic_effect_type_t type;      // HAPTIC_SPRING, HAPTIC_DAMPER, HAPTIC_INERTIA or HAPTIC_FRICTION
    haptic_direction_t direction;  // Direction of the effect.

    /* Replay */
    uint32_t length;          // Duration of the effect.
    uint16_t delay;           // Delay before starting the effect.

    /* Trigger */
    uint16_t button;          // Button that triggers the effect.
    uint16_t interval;        // How soon it can be triggered again after button.

    /* Condition */
    uint16_t right_sat[3];    // Level when joystick is to the positive side; maxF.
    uint16_t left_sat[3];     // Level when joystick is to the negative side; maxF.
    int16_t right_coeff[3];  // How fast to increase the force towards the positive side.
    int16_t left_coeff[3];   // How fast to increase the force towards the negative side.
    uint16_t deadband[3];     // Size of the dead zone; maxF: whole axis-range when 0-centered.
    int16_t center[3];       // Position of the dead zone.
} haptic_condition_t;

typedef struct {
    /* Header */
    haptic_effect_type_t type;      // HAPTIC_SINE, HAPTIC_SQUARE HAPTIC_TRIANGLE, HAPTIC_SAWTOOTHUP or HAPTIC_SAWTOOTHDOWN
    haptic_direction_t direction;  // Direction of the effect.

    /* Replay */
    uint32_t length;      // Duration of the effect.
    uint16_t delay;       // Delay before starting the effect.

    /* Trigger */
    uint16_t button;      // Button that triggers the effect.
    uint16_t interval;    // How soon it can be triggered again after button.

    /* Periodic */
    uint16_t period;      // Period of the wave.
    int16_t magnitude;   // Peak value; if negative, equivalent to 180 degrees extra phase shift.
    int16_t offset;      // Mean value of the wave.
    uint16_t phase;       // Positive phase shift given by hundredth of a degree.

    /* Envelope */
    uint16_t attack_length;   // Duration of the attack.
    uint16_t attack_level;    // Level at the start of the attack.
    uint16_t fade_length; // Duration of the fade.
    uint16_t fade_level;  // Level at the end of the fade.
} haptic_periodic_t;

typedef union {
    /* Common for all force feedback effects */
    uint16_t type;                        // Effect type.
    haptic_constant_t constant;       // Constant effect.
    haptic_periodic_t periodic;       // Periodic effect.
    haptic_condition_t condition;     // Condition effect.
    haptic_ramp_t ramp;               // Ramp effect.
    haptic_left_right_t leftright;     // Left/Right effect.
    haptic_custom_t custom;           // Custom effect.
} haptic_effect_t;


typedef struct {
    // Replay - All effects have this
    uint32_t duration;        // Duration of effect (ms).
    uint16_t delay;           // Delay before starting effect.

    // Trigger - All effects have this
    uint16_t button;          // Button that triggers effect.
    uint16_t interval;        // How soon before effect can be triggered again.

    // Envelope - All effects except condition effects have this
    uint16_t attack_length;   // Duration of the attack (ms).
    uint16_t attack_level;    // Level at the start of the attack.
    uint16_t fade_length;     // Duration of the fade out (ms).
    uint16_t fade_level;      // Level at the end of the fade.
} haptic_t;

typedef uint32_t haptic_effectID_t;
typedef uint32_t hapticID_t;

/*            Haptic Feedback          */
/*! @brief X. @param X X.*/
haptic_t* OpenHaptic(hapticID_t id);
/*! @brief X. @param X X.*/
void CloseHaptic(haptic_t* haptic);
/*! @brief X. @param X X.*/
hapticID_t CreateHapticEffect(haptic_t* haptic);
/*! @brief X. @param X X.*/
void DestroyHapticEffect(haptic_t* haptic);
/*! @brief X. @param X X.*/
bool GetHapticEffectStatus(haptic_t* haptic);
/*! @brief X. @param X X.*/
uint32_t GetHapticFeatures(haptic_t* haptic);
/*! @brief X. @param X X.*/
haptic_t* GetHapticFromID(haptic_t* haptic);
/*! @brief X. @param X X.*/
hapticID_t* GetHapticID(haptic_t* haptic);
/*! @brief X. @param X X.*/
const char* GetHapticName(haptic_t* haptic);
/*! @brief X. @param X X.*/
const char* GetHapticNameForID(haptic_t* haptic);
/*! @brief X. @param X X.*/
hapticID_t* GetHaptics(haptic_t* haptic);
/*! @brief X. @param X X.*/
int GetMaxHapticEffects(haptic_t* haptic);
/*! @brief X. @param X X.*/
int GetMaxHapticEffectsPlaying(haptic_t* haptic);
/*! @brief X. @param X X.*/
MOSS_API int GetNumHapticAxes(haptic_t* haptic);
/*! @brief X. @param X X.*/
MOSS_API bool HapticEffectSupported(haptic_t* haptic);
/*! @brief X. @param X X.*/
bool HapticRumbleSupported(haptic_t* haptic);
/*! @brief X. @param X X.*/
bool InitHapticRumble(haptic_t* joystick);
/*! @brief X. @param X X.*/
bool IsJoystickHaptic(Moss_Joystick);
/*! @brief X. @param X X.*/
bool IsMouseHaptic(void);
/*! @brief X. @param X X.*/
haptic_t* OpenHapticFromJoystick(Moss_Joystick* joystick);
/*! @brief X. @param X X.*/
haptic_t* OpenHapticFromMouse(void);
/*! @brief X. @param X X.*/
bool PauseHaptic(haptic_t* haptic);
/*! @brief X. @param X X.*/
bool PlayHapticRumble(haptic_t* haptic, float strength, uint32 length);
/*! @brief X. @param X X.*/
bool ResumeHaptic(haptic_t* haptic);
/*! @brief X. @param X X.*/
bool RunHapticEffect(haptic_t* haptic, uint32 iterations);
/*! @brief X. @param X X.*/
bool SetHapticAutocenter(haptic_t* haptic, int center);
/*! @brief X. @param X X.*/
bool SetHapticGain(haptic_t* haptic, int gain);
/*! @brief X. @param X X.*/
bool StopHapticEffect(haptic_t* haptic, haptic_effectID_t effect);
/*! @brief X. @param X X.*/
bool StopHapticEffects(haptic_t* haptic);
/*! @brief X. @param X X.*/
bool StopHapticRumble(haptic_t* haptic);
/*! @brief X. @param X X.*/
bool UpdateHapticEffect(haptic_t* haptic, haptic_effectID_t effect, const haptic_effect_t* data);

#ifdef __cplusplus
}
#endif  // __cplusplus
