#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct {
    uint32_t scalarType;    // SFNT version (e.g., 0x4F54544F for OTF)
    uint16_t numTables;     // Number of tables in the font file
    uint16_t searchRange;   // Maximum power of 2 <= numTables * 16
    uint16_t entrySelector; // Log2 of searchRange / 16
    uint16_t rangeShift;    // numTables * 16 - searchRange

    // Table directory
    struct {
        char tag[4];        // Table tag (e.g., "CFF ", "cmap", "head")
        uint32_t checksum;  // Checksum for the table
        uint32_t offset;    // Offset from the beginning of the file
        uint32_t length;    // Length of the table in bytes
    } *tables;              // Array of table directories
} otfData;

typedef struct {
} fonData;

typedef struct {
    uint32_t scalarType;    // SFNT version (e.g., 0x00010000 for TTF)
    uint16_t numTables;     // Number of tables in the font file
    uint16_t searchRange;   // Maximum power of 2 <= numTables * 16
    uint16_t entrySelector; // Log2 of searchRange / 16
    uint16_t rangeShift;    // numTables * 16 - searchRange

    // Table directory
    struct {
        char tag[4];        // Table tag (e.g., "cmap", "glyf", "head")
        uint32_t checksum;  // Checksum for the table
        uint32_t offset;    // Offset from the beginning of the file
        uint32_t length;    // Length of the table in bytes
    } *tables;              // Array of table directories
} ttfData;

typedef struct {
    uint32_t signature;     // WOFF signature (0x774F4646 or 'wOFF')
    uint32_t flavor;        // Font type (e.g., 0x00010000 for TTF)
    uint32_t length;        // Total size of the WOFF file
    uint16_t numTables;     // Number of tables in the font data
    uint16_t reserved;      // Reserved, must be 0
    uint32_t totalSfntSize; // Uncompressed size of the font data
    uint32_t majorVersion;  // Major version
    uint32_t minorVersion;  // Minor version
    uint32_t metaOffset;    // Offset to metadata block
    uint32_t metaLength;    // Length of metadata block
    uint32_t metaOrigLength;// Original length of metadata block
    uint32_t privOffset;    // Offset to private data block
    uint32_t privLength;    // Length of private data block
} woffData;

typedef struct {
} woff2Data;

#ifdef __cplusplus
}
#endif  // __cplusplus