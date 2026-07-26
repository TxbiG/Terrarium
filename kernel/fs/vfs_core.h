#ifndef TERRARIUM_VFS_CORE_H
#define TERRARIUM_VFS_CORE_H

#include "fs.h"

#ifdef __cplusplus
extern "C" {
#endif

void terra_vfs_reset(void);
int terra_vfs_bootstrap(void);

#ifdef __cplusplus
}
#endif

#endif
