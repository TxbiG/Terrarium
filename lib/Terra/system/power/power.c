#include "power.h"

// Port addresses for keyboard controller reboot method
#define KEYBOARD_CONTROLLER 0x64
#define KEYBOARD_RESET 0xFE

// I/O port access functions (x86-specific)
static inline void outb(unsigned short port, unsigned char val) { __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port)); }

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Reboot by sending 0xFE to keyboard controller
void power_reboot(void) {
    // Wait until keyboard controller input buffer is empty
    while (inb(KEYBOARD_CONTROLLER) & 0x02);
    outb(KEYBOARD_CONTROLLER, KEYBOARD_RESET);

    // If reboot fails, halt CPU
    while (1) {
        __asm__ __volatile__("hlt");
    }
}

// Shutdown via ACPI (simplified example, real ACPI requires parsing tables)
void power_shutdown(void) {
    // Typically involves writing to ACPI PM1a control block to power off
    // Here, just halt CPU as a placeholder
    while (1) {
        __asm__ __volatile__("hlt");
    }
}

// Placeholder for hibernate
void power_hibernate(void) {
    // TODO: Implement hibernation using ACPI or other method
}

