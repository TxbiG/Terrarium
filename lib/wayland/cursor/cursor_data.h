#ifndef FLORA_CURSOR_H
#define FLORA_CURSOR_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>


struct wl_cursor_theme;
struct wl_buffer {
	void (*destroy)(void *data);		// Requests
    void (*release)(void *data);		// Events
};
struct wl_shm;


/** A still image part of a cursor
 *
 * Use `wl_cursor_image_get_buffer()` to get the corresponding `struct
 * wl_buffer` to attach to your `struct wl_surface`. */
struct wl_cursor_image {
	uint32_t width; /** Actual width */
	uint32_t height; /** Actual height */
	uint32_t hotspot_x; /** Hot spot x (must be inside image) */
	uint32_t hotspot_y; /** Hot spot y (must be inside image) */
	uint32_t delay;  /** Animation delay to next frame (ms) */
    uint32_t *pixels; /* pointer to pixels */
};

/** A cursor, as returned by `wl_cursor_theme_get_cursor()` */
struct wl_cursor {
	unsigned int image_count;    /** How many images there are in this cursor’s animation */
	struct wl_cursor_image** images; /** The array of still images composing this animation */
	char* name; /** The name of this cursor */
};

struct wl_cursor_theme * wl_cursor_theme_load(const char *name, int size, struct wl_shm *shm);

void wl_cursor_theme_destroy(struct wl_cursor_theme *theme);

struct wl_cursor* wl_cursor_theme_get_cursor(struct wl_cursor_theme *theme, const char *name);

struct wl_buffer * wl_cursor_image_get_buffer(struct wl_cursor_image *image);

int wl_cursor_frame(struct wl_cursor *cursor, uint32_t time);

int wl_cursor_frame_and_duration(struct wl_cursor *cursor, uint32_t time, uint32_t *duration);

#ifdef  __cplusplus
}
#endif

#endif // FLORA_CURSOR_H