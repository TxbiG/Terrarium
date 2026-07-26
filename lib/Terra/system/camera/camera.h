#ifndef LIBCAMERA_H
#define LIBCAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Camera pixel formats */
typedef enum {
    CAMERA_FORMAT_UNKNOWN = 0,
    CAMERA_FORMAT_RGB,
    CAMERA_FORMAT_YUV,
    CAMERA_FORMAT_NV12,
    CAMERA_FORMAT_GRAY
} camera_format_t;

/* Camera device info */
typedef struct {
    int id;
    char name[64];
    int max_width;
    int max_height;
    int supports_video;
    int supports_photo;
} camera_info_t;

/* Frame buffer */
typedef struct {
    void *data;
    int width;
    int height;
    camera_format_t format;
    unsigned long timestamp;
} camera_frame_t;

/* Camera API */
int camera_init(void);
int camera_shutdown(void);

/* Device enumeration */
int camera_get_count(void);
int camera_get_info(int camera_id, camera_info_t *info);

/* Open / close camera */
int camera_open(int camera_id);
int camera_close(int camera_id);

/* Capture control */
int camera_start_stream(void);
int camera_stop_stream(void);

/* Capture frame */
int camera_capture_frame(camera_frame_t *frame);

/* Camera controls */
int camera_set_resolution(int width, int height);
int camera_set_framerate(int fps);
int camera_set_exposure(int value);
int camera_set_focus(int value);

#ifdef __cplusplus
}
#endif

#endif /* LIBCAMERA_H */
