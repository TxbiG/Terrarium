#ifdef KERNEL_MODULE_AUDIO_H
#define KERNEL_MODULE_AUDIO_H

/* 
Hardware initialization
Audio data transfers (usually via DMA)
Interrupt handling
Sample rate configuration
Buffer management
Channel mapping and synchronization
*/

// Clock Drift Compensation / Sync (with HDMI or BT): Requires Bluetooth or amplifiers


#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/dma-mapping.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/types.h>

#define MYSOUND_NAME        "mysound"
#define MYSOUND_MINOR       0
#define MYSOUND_BUFFER_SIZE (64 * 1024)  // 64 KB ring buffer

#define MYSOUND_MAX_CHANNELS 8
#define MYSOUND_MIN_RATE     44100
#define MYSOUND_MAX_RATE     192000

// ------------------- IOCTL Command Definitions ------------------- //

#define MYSOUND_IOC_MAGIC  'S'

#define MYSOUND_IOC_SET_FORMAT     _IOW(MYSOUND_IOC_MAGIC, 1, struct mysound_format)
#define MYSOUND_IOC_START_STREAM   _IO(MYSOUND_IOC_MAGIC, 2)
#define MYSOUND_IOC_STOP_STREAM    _IO(MYSOUND_IOC_MAGIC, 3)
#define MYSOUND_IOC_GET_STATUS     _IOR(MYSOUND_IOC_MAGIC, 4, struct mysound_status)

// ------------------- Data Structures ------------------- //

struct mysound_format {
    uint32_t sample_rate;
    uint16_t channels;
    uint16_t bit_depth;  // 16, 24, 32
};

struct mysound_status {
    uint32_t read_pos;
    uint32_t write_pos;
    uint32_t buffer_size;
    uint8_t  running;
};

// Main device state
struct mysound_device {
    struct pci_dev *pdev;
    struct cdev cdev;
    struct device *dev;
    void __iomem *mmio_base;

    // DMA buffer
    uint8_t *dma_area;
    dma_addr_t dma_addr;
    size_t dma_bytes;

    // Buffer positions
    size_t read_pos;
    size_t write_pos;

    // Audio configuration
    struct mysound_format format;

    // IRQ and sync
    int irq;
    spinlock_t lock;
    wait_queue_head_t wait_queue;
    uint8_t stream_running;

    // Channel map
    char *channel_labels[MYSOUND_MAX_CHANNELS];  // e.g., "FL", "FR", "C", etc.
};

// ------------------- File Operation Prototypes ------------------- //

int mysound_open(struct inode *inode, struct file *file);
int mysound_release(struct inode *inode, struct file *file);
ssize_t mysound_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
long mysound_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
__poll_t mysound_poll(struct file *file, poll_table *wait);
int mysound_mmap(struct file *file, struct vm_area_struct *vma);

// ------------------- Interrupt and DMA Handlers ------------------- //

irqreturn_t mysound_irq_handler(int irq, void *dev_id);
int mysound_init_dma(struct mysound_device *dev);
void mysound_cleanup_dma(struct mysound_device *dev);

// ------------------- Hardware Init/Shutdown ------------------- //

int mysound_hw_init(struct mysound_device *dev);
void mysound_hw_shutdown(struct mysound_device *dev);

// ------------------- PCI Driver ------------------- //

extern const struct pci_device_id mysound_pci_ids[];
int mysound_probe(struct pci_dev *pdev, const struct pci_device_id *id);
void mysound_remove(struct pci_dev *pdev);



#endif // KERNEL_MODULE_AUDIO_H