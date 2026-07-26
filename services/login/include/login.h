/*!
 * @file Login.h
 *
 * 
 */

// input handling
// profile selection
// proper authenication
// focus & cursor
// fully customisable (Desktop / Mobile & Tablet / raspberry pi)
// TV and console and XR not needed

#ifndef LOGIN_H
#define LOGIN_H

#include <compositor/compositor.h>

struct login_t {
    struct compositor_t *comp;
    struct wlr_scene_tree *window;
    char username[64];
    char profile[16];
};

struct login_t* login_create(struct compositor_t* comp);
void login_run(struct login_t* gui);
void login_destroy(struct login_t* gui);

#if defined(TERRARIUM_DESKTOP) || defined(TERRARIUM_MOBILE) || defined(TERRARIUM_TABLET) || defined(TERRARIUM_RASPBERRY_PI)

#else // TERRARIUM_SERVER

#endif

#endif // LOGIN_H