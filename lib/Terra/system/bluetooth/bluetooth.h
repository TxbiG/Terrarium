#ifndef LIBBLUETOOTH_H
#define LIBBLUETOOTH_H

#ifdef __cplusplus
extern "C" {
#endif

#define BT_ADDR_LEN 6

typedef struct {
    unsigned char address[BT_ADDR_LEN];
    char name[64];
    int connected;
    int paired;
    int type; // e.g., audio, HID, misc
} bt_device_t;

/* Initialize/Shutdown Bluetooth stack */
int bluetooth_init(void);
int bluetooth_shutdown(void);

/* Scan for devices */
int bluetooth_start_scan(void);
int bluetooth_stop_scan(void);
int bluetooth_get_device_count(void);
int bluetooth_get_device(int index, bt_device_t *device);

/* Pairing / connection */
int bluetooth_pair_device(const unsigned char *address);
int bluetooth_unpair_device(const unsigned char *address);
int bluetooth_connect_device(const unsigned char *address);
int bluetooth_disconnect_device(const unsigned char *address);

/* Data transfer / profiles */
int bluetooth_send_file(const unsigned char *address, const char *file_path);
int bluetooth_receive_file(const unsigned char *address, const char *dest_path);

/* Optional: audio streaming */
int bluetooth_start_audio_stream(const unsigned char *address);
int bluetooth_stop_audio_stream(const unsigned char *address);

#ifdef __cplusplus
}
#endif

#endif /* LIBBLUETOOTH_H */
