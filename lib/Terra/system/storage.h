#ifndef LIBSTORAGE_H
#define LIBSTORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Storage device types */
typedef enum {
    STORAGE_TYPE_UNKNOWN = 0,
    STORAGE_TYPE_HDD,
    STORAGE_TYPE_SSD,
    STORAGE_TYPE_NVME,
    STORAGE_TYPE_REMOVABLE,
    STORAGE_TYPE_EMMC,
    STORAGE_TYPE_UFS
} storage_type_t;

/* Block device info */
typedef struct {
    int id;
    char name[64];
    storage_type_t type;
    unsigned long long size_bytes;
    unsigned int block_size;
    int removable;
} storage_device_t;

/* Partition info */
typedef struct {
    int id;
    unsigned long long start_lba;
    unsigned long long size_lba;
    char filesystem[32];
} storage_partition_t;

/* Storage API */
int storage_init(void);
int storage_shutdown(void);

/* Device enumeration */
int storage_get_device_count(void);
int storage_get_device(int index, storage_device_t *device);

/* Partition management */
int storage_get_partition_count(int device_id);
int storage_get_partition(int device_id, int index, storage_partition_t *part);

/* Mounting */
int storage_mount(int device_id, int partition_id, const char *path);
int storage_unmount(const char *path);

/* Disk operations */
int storage_flush(int device_id);
int storage_eject(int device_id);

#ifdef __cplusplus
}
#endif

#endif /* LIBSTORAGE_H */
