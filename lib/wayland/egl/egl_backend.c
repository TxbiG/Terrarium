#include <stddef.h> /* offsetof */
#include <stdio.h>  /* printf */

#include "egl_backend.h"

struct egl_window_v0 {
    struct surface *surface;

    int width;
    int height;
    int dx;
    int dy;

    int attached_width;
    int attached_height;
};

/* From: ca3ed3e024 - Ander Conselvan de Oliveira : egl/wayland: Don't invalidate drawable on swap buffers */
struct egl_window_v1 {
    struct surface *surface;

    int width;
    int height;
    int dx;
    int dy;

    int attached_width;
    int attached_height;

    void *private;
    void (*resize_callback)(struct egl_window *, void *);
};

/* From: 690ead4a13 - Stencel, Joanna : egl/wayland-egl: Fix for segfault in dri2_wl_destroy_surface. */
#define WL_EGL_WINDOW_VERSION_v2 2
struct egl_window_v2 {
    struct surface *surface;

    int width;
    int height;
    int dx;
    int dy;

    int attached_width;
    int attached_height;

    void *private;
    void (*resize_callback)(struct egl_window *, void *);
    void (*destroy_window_callback)(void *);
};

/* From: 2d5d61bc49 - Miguel A. Vico : wayland-egl: Make wl_egl_window a versioned struct */
#define WL_EGL_WINDOW_VERSION_v3 3
struct egl_window_v3 {
    const intptr_t version;

    int width;
    int height;
    int dx;
    int dy;

    int attached_width;
    int attached_height;

    void *driver_private;
    void (*resize_callback)(struct egl_window *, void *);
    void (*destroy_window_callback)(void *);

    struct surface *surface;
};

#define MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

#define CHECK_RENAMED_MEMBER(a_ver, b_ver, a_member, b_member)                      \
    do {                                                                            \
        if (offsetof(struct egl_window ## a_ver, a_member) != offsetof(struct egl_window ## b_ver, b_member)) {                    \
            printf("Backwards incompatible change detected!\n   "                    \
                   "offsetof(struct egl_window" #a_ver "::" #a_member ") != "    \
                   "offsetof(struct egl_window" #b_ver "::" #b_member ")\n");    \
            return 1;                                                               \
        }                                                                           \
                                                                                    \
        if (MEMBER_SIZE(struct egl_window ## a_ver, a_member) != MEMBER_SIZE(struct egl_window ## b_ver, b_member)) {  \
            printf("Backwards incompatible change detected!\n   "                    \
                   "MEMBER_SIZE(struct egl_window" #a_ver "::" #a_member ") != " \
                   "MEMBER_SIZE(struct egl_window" #b_ver "::" #b_member ")\n"); \
            return 1;                                                               \
        }                                                                           \
    } while (0)

#define CHECK_MEMBER(a_ver, b_ver, member) CHECK_RENAMED_MEMBER(a_ver, b_ver, member, member)
#define CHECK_MEMBER_CURRENT(a_ver, member) CHECK_MEMBER(a_ver,, member)

#define CHECK_SIZE(a_ver, b_ver)                                                    \
    do {                                                                            \
        if (sizeof(struct egl_window ## a_ver) > sizeof(struct egl_window ## b_ver)) { \
            printf("Backwards incompatible change detected!\n   "                    \
                   "sizeof(struct egl_window" #a_ver ") > "                      \
                   "sizeof(struct egl_window" #b_ver ")\n");                     \
            return 1;                                                               \
        }                                                                           \
    } while (0)

#define CHECK_SIZE_CURRENT(a_ver)                                                   \
    do {                                                                            \
        if (sizeof(struct egl_window ## a_ver) !=  sizeof(struct egl_window)) { \
            printf("Backwards incompatible change detected!\n   "                    \
                   "sizeof(struct egl_window" #a_ver ") != "                     \
                   "sizeof(struct egl_window)\n");                               \
            return 1;                                                               \
        }                                                                           \
    } while (0)

#define CHECK_VERSION(a_ver, b_ver)                                              \
    do {                                                                         \
        if ((EGL_WINDOW_VERSION ## a_ver) >=  (EGL_WINDOW_VERSION ## b_ver)) {   \
            printf("Backwards incompatible change detected!\n   "                \
                   "EGL_WINDOW_VERSION" #a_ver " >= "                            \
                   "EGL_WINDOW_VERSION" #b_ver "\n");                            \
            return 1;                                                            \
        }                                                                        \
    } while (0)

#define CHECK_VERSION_CURRENT(a_ver)                                        \
    do {                                                                    \
        if ((EGL_WINDOW_VERSION ## a_ver) != (EGL_WINDOW_VERSION)) {        \
            printf("Backwards incompatible change detected!\n   "           \
                   "EGL_WINDOW_VERSION" #a_ver " != "                       \
                   "EGL_WINDOW_VERSION\n");                                 \
            return 1;                                                       \
        }                                                                   \
    } while (0)

int main(int argc, char **argv)
{
    /* Check wl_egl_window_v1 ABI against wl_egl_window_v0 */
    CHECK_MEMBER(_v0, _v1, surface);
    CHECK_MEMBER(_v0, _v1, width);
    CHECK_MEMBER(_v0, _v1, height);
    CHECK_MEMBER(_v0, _v1, dx);
    CHECK_MEMBER(_v0, _v1, dy);
    CHECK_MEMBER(_v0, _v1, attached_width);
    CHECK_MEMBER(_v0, _v1, attached_height);

    CHECK_SIZE(_v0, _v1);

    /* Check wl_egl_window_v2 ABI against wl_egl_window_v1 */
    CHECK_MEMBER(_v1, _v2, surface);
    CHECK_MEMBER(_v1, _v2, width);
    CHECK_MEMBER(_v1, _v2, height);
    CHECK_MEMBER(_v1, _v2, dx);
    CHECK_MEMBER(_v1, _v2, dy);
    CHECK_MEMBER(_v1, _v2, attached_width);
    CHECK_MEMBER(_v1, _v2, attached_height);
    CHECK_MEMBER(_v1, _v2, private);
    CHECK_MEMBER(_v1, _v2, resize_callback);

    CHECK_SIZE(_v1, _v2);

    /* Check wl_egl_window_v3 ABI against wl_egl_window_v2 */
    CHECK_RENAMED_MEMBER(_v2, _v3, surface, version);
    CHECK_MEMBER        (_v2, _v3, width);
    CHECK_MEMBER        (_v2, _v3, height);
    CHECK_MEMBER        (_v2, _v3, dx);
    CHECK_MEMBER        (_v2, _v3, dy);
    CHECK_MEMBER        (_v2, _v3, attached_width);
    CHECK_MEMBER        (_v2, _v3, attached_height);
    CHECK_RENAMED_MEMBER(_v2, _v3, private, driver_private);
    CHECK_MEMBER        (_v2, _v3, resize_callback);
    CHECK_MEMBER        (_v2, _v3, destroy_window_callback);

    CHECK_SIZE   (_v2, _v3);
    CHECK_VERSION(_v2, _v3);

    /* Check current wl_egl_window ABI against wl_egl_window_v3 */
    CHECK_MEMBER_CURRENT(_v3, version);
    CHECK_MEMBER_CURRENT(_v3, width);
    CHECK_MEMBER_CURRENT(_v3, height);
    CHECK_MEMBER_CURRENT(_v3, dx);
    CHECK_MEMBER_CURRENT(_v3, dy);
    CHECK_MEMBER_CURRENT(_v3, attached_width);
    CHECK_MEMBER_CURRENT(_v3, attached_height);
    CHECK_MEMBER_CURRENT(_v3, driver_private);
    CHECK_MEMBER_CURRENT(_v3, resize_callback);
    CHECK_MEMBER_CURRENT(_v3, destroy_window_callback);
    CHECK_MEMBER_CURRENT(_v3, surface);

    CHECK_SIZE_CURRENT   (_v3);
    CHECK_VERSION_CURRENT(_v3);

    return 0;
}
