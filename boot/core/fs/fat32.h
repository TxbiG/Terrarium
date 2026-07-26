#include <stdint.h>
#include <ctype.h>

typedef struct {
    char title[64];
    char kernel_path[128];
    char params[256];
    uint8_t filesystem_type; // FAT32, EXT4, etc.
    uint32_t partition_number;
} boot_entry_t;

typedef int (*disk_read_func_t)(uint64_t lba, uint32_t sector_count, void* buffer);

static disk_read_func_t disk_read = 0;

// BIOS INT13 implementation (real mode) - simplified example stub
int disk_read_bios(uint64_t lba, uint32_t sector_count, void* buffer) {
    // In reality, requires switching to real mode or unreal mode,
    // calling INT 13h with LBA extensions.
    // Here is a stub showing intent:
    // You must implement real mode calls in asm or a real mode stub.
    return -1;  // Stub fail
}

// UEFI implementation - simplified stub
int disk_read_uefi(uint64_t lba, uint32_t sector_count, void* buffer) {
    // In UEFI environment, call EFI_BLOCK_IO_PROTOCOL ReadBlocks
    // This is platform-dependent and needs UEFI headers / environment
    return -1;  // Stub fail
}

// Initialization detects environment and sets disk_read pointer
void disk_init() {
    // Simplified detection example:
    // if (UEFI detected) disk_read = disk_read_uefi;
    // else disk_read = disk_read_bios;
    disk_read = disk_read_bios;  // Default stub
}

int disk_read_sectors(uint64_t lba, uint32_t sector_count, void* buffer) {
    if (!disk_read) return -1;
    return disk_read(lba, sector_count, buffer);
}



#include <stdint.h>
#include <string.h>

#define SECTOR_SIZE 512
#define MAX_FILENAME_LEN 12  // 8.3 format + null

// BIOS Parameter Block (BPB) FAT32 layout (partial)
typedef struct {
    uint8_t  jump_boot[3];
    uint8_t  oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t  num_fats;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t  media;
    uint16_t fat_size_16;
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;

    // FAT32 specific
    uint32_t fat_size_32;
    uint16_t ext_flags;
    uint16_t fs_version;
    uint32_t root_cluster;
    uint16_t fs_info;
    uint16_t backup_boot_sector;
    uint8_t  reserved[12];
    uint8_t  drive_number;
    uint8_t  reserved1;
    uint8_t  boot_signature;
    uint32_t volume_id;
    uint8_t  volume_label[11];
    uint8_t  fs_type[8];
} bpb_fat32_t;

// Directory entry (short)
typedef struct {
    uint8_t name[8];
    uint8_t ext[3];
    uint8_t attr;
    uint8_t ntres;
    uint8_t create_time_tenths;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t write_time;
    uint16_t write_date;
    uint16_t first_cluster_low;
    uint32_t file_size;
} fat_dir_entry_t;

// Globals (should ideally be encapsulated)
bpb_fat32_t bpb;
uint32_t fat_start_lba;
uint32_t cluster_size;
uint32_t root_dir_first_cluster;

// Buffer for reading sectors
static uint8_t sector_buffer[SECTOR_SIZE];

// Helper: Read one sector from disk
int fat32_read_sector(uint32_t lba) {
    return disk_read_sectors(lba, 1, sector_buffer);
}

// Parse BPB from sector 0
int fat32_read_bpb() {
    if (fat32_read_sector(0) != 0) return -1;
    memcpy(&bpb, sector_buffer, sizeof(bpb_fat32_t));

    fat_start_lba = bpb.reserved_sector_count;
    cluster_size = bpb.sectors_per_cluster * bpb.bytes_per_sector;
    root_dir_first_cluster = bpb.root_cluster;
    return 0;
}

// Convert cluster number to first sector number
uint32_t fat32_cluster_to_lba(uint32_t cluster) {
    return fat_start_lba + bpb.num_fats * bpb.fat_size_32 +
           (cluster - 2) * bpb.sectors_per_cluster;
}

// Read FAT entry for a cluster
int fat32_read_fat_entry(uint32_t cluster, uint32_t* next_cluster) {
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = fat_start_lba + (fat_offset / SECTOR_SIZE);
    uint32_t ent_offset = fat_offset % SECTOR_SIZE;

    if (fat32_read_sector(fat_sector) != 0) return -1;
    *next_cluster = *(uint32_t*)&sector_buffer[ent_offset] & 0x0FFFFFFF;
    return 0;
}

// Compare 8.3 filenames (both uppercase, padded with spaces)
int fat32_compare_filename(const fat_dir_entry_t* entry, const char* filename) {
    char formatted[13] = {0};  // 8 + '.' + 3 + '\0'
    int i;

    // Format filename to 8.3 padded with spaces and uppercase
    // Input example: "KERNEL.BIN"
    const char* dot = strchr(filename, '.');
    if (!dot) return -1;

    int name_len = dot - filename;
    if (name_len > 8) return -1;

    for (i = 0; i < 8; i++) {
        if (i < name_len)
            formatted[i] = toupper(filename[i]);
        else
            formatted[i] = ' ';
    }
    formatted[8] = '.';

    int ext_len = strlen(dot + 1);
    if (ext_len > 3) return -1;

    for (i = 0; i < 3; i++) {
        if (i < ext_len)
            formatted[9 + i] = toupper(dot[1 + i]);
        else
            formatted[9 + i] = ' ';
    }
    formatted[12] = '\0';

    // Compare with entry
    return memcmp(entry->name, formatted, 11);
}

// Read cluster data into buffer (cluster_size bytes)
int fat32_read_cluster(uint32_t cluster, void* buffer) {
    uint32_t first_sector = fat32_cluster_to_lba(cluster);
    for (uint32_t i = 0; i < bpb.sectors_per_cluster; i++) {
        if (disk_read_sectors(first_sector + i, 1, (uint8_t*)buffer + i * SECTOR_SIZE) != 0)
            return -1;
    }
    return 0;
}

// Find file in root directory and load entire contents into dest buffer
int fat32_read_file(const char* filename, void* dest) {
    uint32_t cluster = root_dir_first_cluster;
    fat_dir_entry_t entries[SECTOR_SIZE / sizeof(fat_dir_entry_t)];

    while (1) {
        if (fat32_read_cluster(cluster, entries) != 0)
            return -1;

        for (int i = 0; i < SECTOR_SIZE / sizeof(fat_dir_entry_t); i++) {
            fat_dir_entry_t* entry = &entries[i];

            // Check empty or deleted entry
            if (entry->name[0] == 0x00)
                return -1; // End of directory, file not found
            if (entry->name[0] == 0xE5)
                continue;  // Deleted entry

            if (!(entry->attr & 0x0F)) { // Normal file or dir (skip long names)
                if (fat32_compare_filename(entry, filename) == 0) {
                    // Found the file
                    uint32_t file_size = entry->file_size;
                    uint32_t first_cluster = (entry->first_cluster_high << 16) | entry->first_cluster_low;
                    uint32_t bytes_read = 0;
                    uint8_t* dest_ptr = (uint8_t*)dest;
                    uint32_t current_cluster = first_cluster;

                    while (current_cluster < 0x0FFFFFF8) {
                        if (fat32_read_cluster(current_cluster, dest_ptr) != 0)
                            return -1;

                        dest_ptr += cluster_size;
                        bytes_read += cluster_size;

                        if (bytes_read >= file_size)
                            return 0; // Done

                        if (fat32_read_fat_entry(current_cluster, &current_cluster) != 0)
                            return -1;
                    }
                    return 0;
                }
            }
        }

        // Read next cluster in root directory chain
        uint32_t next_cluster;
        if (fat32_read_fat_entry(cluster, &next_cluster) != 0)
            return -1;
        if (next_cluster >= 0x0FFFFFF8)
            break; // End of root dir
        cluster = next_cluster;
    }

    return -1; // Not found
}
