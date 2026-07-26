#ifndef MONITOR_H
#define MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

// Resolution and Layout
// Brightness and COlor
// Multiple Displays
// Monitor Config Functions
    // Color Temp
    // Contrast
    // DisplayAreaPos
    // SetMonitorDisplaySize

/* Monitor / display info */
typedef struct {
    int id;
    char name[64];
    int width;
    int height;
    int refresh_rate;
    int color_depth;
    int is_primary;
    int redBits; 
    int greenBits;
    int blueBits;
    float xscale, yscale;   // Content scale of the monitor
    int xpos, ypos;         // Virtual position of the monitor
    float gamma;            // Monitor gamma
} monitor_handle_t;

struct Moss_VideoMode { int width, height, redBits, greenBits, blueBits, refreshRate; };

typedef struct {
    uint8* size;
    uint8* red;
    uint8* green;
    uint8* blue;
} gamma_ramp_t;

/* Enumeration */
int screen_get_count(void);
int screen_get_info(int index, screen_info_t *info);

/* Resolution / mode control */
int screen_set_resolution(int screen_id, int width, int height, int refresh_rate);
int screen_get_current_mode(int screen_id, int *width, int *height, int *refresh_rate);

/* Framebuffer capture */
int screen_capture_frame(int screen_id, void **buffer, int *width, int *height, int *stride);

gamma_ramp_t* get_monitor_gamma_ramp();
void set_monitor_gamma_ramp(display, int monitor_id, gamma_ramp_t);


//
int GetMonitors();

void GetWindowFrameSize();
void GetMonitorPhysicalSize();
void GetMonitorContentScale();
void GetMonitorPos();

VideoMode* GetVideoModes();


#ifdef __cplusplus
}
#endif

#endif /* MONITOR_H */
