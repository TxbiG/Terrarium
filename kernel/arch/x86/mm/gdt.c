#include "gdt.h"
#include "stdint.h"

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
struct gdt_entry_t {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr_t {
    uint16_t limit;             // The upper 16 bits of all selector limits.
    uint32_t base;              // The address of the first gdt_entry_t struct.
} __attribute__((packed));

static struct gdt_entry_t gdt_entries[5];
static struct gdt_ptr_t gdt_ptr;

// Lets us access our ASM functions from our C code.
extern void load_gdt(gdt_ptr_t* gdt_ptr);

static void gdt_set_gate(int32_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[entry].base_low = (base & 0xFFFF);
    gdt_entries[entry].base_middle = (base >> 16) & 0xFF;
    gdt_entries[entry].base_high = (base >> 24) & 0xFF;

    gdt_entries[entry].limit_low = (limit & 0xFFFF);
    gdt_entries[entry].granularity = (limit >> 16) & 0x0F;

    gdt_entries[entry].granularity |= gran & 0xF0;
    gdt_entries[entry].access = access;
}

void init_gdt() {
    gdt_ptr.limit = (sizeof(struct gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_entries[0] = (struct gdt_entry_t){0, 0, 0, 0, 0, 0};              // Null segment
    gdt_entries[1] = (struct gdt_entry_t){0xFFFF, 0, 0, 0x9A, 0xCF, 0};   // Code segment
    gdt_entries[2] = (struct gdt_entry_t){0xFFFF, 0, 0, 0x92, 0xCF, 0};   // Data segment

    gdt_set_gate(0,0,0,0,0);                    //Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); //User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); //User mode data segment

    load_gdt(&gdt_ptr);
}