#ifndef TERRARIUM_SYSTEM_AUTH_PAM_H
#define TERRARIUM_SYSTEM_AUTH_PAM_H

typedef struct terra_pam_context {
    const char *service;
    const char *user;
} terra_pam_context_t;

int terra_pam_authenticate(terra_pam_context_t *context, const char *password);
int terra_pam_open_session(terra_pam_context_t *context);
int terra_pam_close_session(terra_pam_context_t *context);

#endif
