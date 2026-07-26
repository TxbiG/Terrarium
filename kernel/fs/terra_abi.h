#ifndef TERRARIUM_FS_TERRA_ABI_H
#define TERRARIUM_FS_TERRA_ABI_H

#include "types.h"

#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif

#define TERRA_DIRENT_NAME_MAX 256

#define TERRA_DT_UNKNOWN 0
#define TERRA_DT_FIFO 1
#define TERRA_DT_CHR 2
#define TERRA_DT_DIR 4
#define TERRA_DT_BLK 6
#define TERRA_DT_REG 8
#define TERRA_DT_LNK 10
#define TERRA_DT_SOCK 12

#define CDT_UNKNOWN TERRA_DT_UNKNOWN
#define CDT_FIFO TERRA_DT_FIFO
#define CDT_CHR TERRA_DT_CHR
#define CDT_DIR TERRA_DT_DIR
#define CDT_BLK TERRA_DT_BLK
#define CDT_REG TERRA_DT_REG
#define CDT_LNK TERRA_DT_LNK
#define CDT_SOCK TERRA_DT_SOCK

#define TERRA_S_IFMT 00170000
#define TERRA_S_IFSOCK 0140000
#define TERRA_S_IFLNK 0120000
#define TERRA_S_IFREG 0100000
#define TERRA_S_IFBLK 0060000
#define TERRA_S_IFDIR 0040000
#define TERRA_S_IFCHR 0020000
#define TERRA_S_IFIFO 0010000

#define TERRA_S_IRUSR 0400
#define TERRA_S_IWUSR 0200
#define TERRA_S_IXUSR 0100
#define TERRA_S_IRGRP 0040
#define TERRA_S_IWGRP 0020
#define TERRA_S_IXGRP 0010
#define TERRA_S_IROTH 0004
#define TERRA_S_IWOTH 0002
#define TERRA_S_IXOTH 0001

#define S_IFREG TERRA_S_IFREG
#define S_IFDIR TERRA_S_IFDIR
#define S_IFLNK TERRA_S_IFLNK
#define S_IRUSR TERRA_S_IRUSR
#define S_IWUSR TERRA_S_IWUSR
#define S_IXUSR TERRA_S_IXUSR
#define S_IRGRP TERRA_S_IRGRP
#define S_IROTH TERRA_S_IROTH

typedef struct terra_stat {
    uint64 st_dev;
    uint64 st_ino;
    uint32 st_mode;
    uint32 st_nlink;
    uint32 st_uid;
    uint32 st_gid;
    uint64 st_rdev;
    uint64 st_size;
    uint64 st_blksize;
    uint64 st_blocks;
    uint64 st_atime;
    uint64 st_mtime;
    uint64 st_ctime;
} terra_stat_t;

typedef struct terra_dirent {
    uint64 d_ino;
    uint64 d_off;
    uint16 d_reclen;
    uint8 d_type;
    char d_name[TERRA_DIRENT_NAME_MAX];
} terra_dirent_t;

typedef struct terra_socket_addr {
    uint16 family;
    char data[108];
} terra_socket_addr_t;

typedef struct terra_iovec {
    void *base;
    size_t len;
} terra_iovec_t;

typedef struct terra_msg_header {
    void *name;
    uint32 name_len;
    terra_iovec_t *iov;
    size_t iov_len;
    void *control;
    size_t control_len;
    int flags;
} terra_msg_header_t;

typedef struct terra_input_id {
    uint16 bustype;
    uint16 vendor;
    uint16 product;
    uint16 version;
} terra_input_id_t;

typedef struct termios {
    uint32 c_iflag;
    uint32 c_oflag;
    uint32 c_cflag;
    uint32 c_lflag;
    uint8 c_cc[32];
} terra_termios_t;

typedef struct winsize {
    uint16 ws_row;
    uint16 ws_col;
    uint16 ws_xpixel;
    uint16 ws_ypixel;
} terra_winsize_t;

#endif
