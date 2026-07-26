#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct {
} ICO;

ICO ico();

typedef struct {
    // SOI marker
    uint16_t soiMarker;    // Start of Image marker (0xFFD8)

    // Metadata
    uint16_t width;        // Image width
    uint16_t height;       // Image height
    uint8_t colorComponents; // Number of color components (e.g., 3 for RGB)

    // Compressed image data
    uint8_t *imageData;    // Pointer to compressed image data
    size_t imageSize;      // Size of the compressed image data
} JPG;

typedef struct {
    // File signature
    uint8_t signature[8];  // PNG signature (0x89 50 4E 47 0D 0A 1A 0A)

    // IHDR chunk (mandatory)
    uint32_t width;        // Image width
    uint32_t height;       // Image height
    uint8_t bitDepth;      // Bit depth
    uint8_t colorType;     // Color type (e.g., grayscale, truecolor, etc.)
    uint8_t compression;   // Compression method
    uint8_t filter;        // Filter method
    uint8_t interlace;     // Interlace method

    // Additional chunks
    uint8_t *chunkData;    // Pointer to the raw chunk data (e.g., IDAT, PLTE)
    size_t chunkDataSize;  // Size of the raw chunk data
} PNG;

typedef struct {
    // Header
    char signature[3];     // Signature ("GIF")
    char version[3];       // Version ("87a" or "89a")

    // Logical screen descriptor
    uint16_t width;        // Logical screen width
    uint16_t height;       // Logical screen height
    uint8_t packedFields;  // Packed fields for global color table
    uint8_t bgColorIndex;  // Background color index
    uint8_t pixelAspect;   // Pixel aspect ratio

    // Global color table
    uint8_t *globalColorTable; // Pointer to the global color table
    size_t globalColorTableSize; // Size of the global color table

    // Image data
    uint8_t *imageData;    // Pointer to the raw image data blocks
    size_t imageDataSize;  // Size of the image data
} GIF;

typedef struct {
} TIFF;

typedef struct {
    // File header
    uint16_t fileType;     // File type, always "BM" (0x4D42)
    uint32_t fileSize;     // Size of the file in bytes
    uint16_t reserved1;    // Reserved, always 0
    uint16_t reserved2;    // Reserved, always 0
    uint32_t offsetData;   // Offset to the start of the pixel data

    // Info header
    uint32_t headerSize;   // Size of this header (40 bytes)
    int32_t width;         // Bitmap width in pixels
    int32_t height;        // Bitmap height in pixels
    uint16_t planes;       // Number of color planes (must be 1)
    uint16_t bitCount;     // Bits per pixel (1, 4, 8, 16, 24, or 32)
    uint32_t compression;  // Compression method
    uint32_t imageSize;    // Size of raw bitmap data
    int32_t xPelsPerMeter; // Horizontal resolution (pixels per meter)
    int32_t yPelsPerMeter; // Vertical resolution (pixels per meter)
    uint32_t clrUsed;      // Number of colors in the color palette
    uint32_t clrImportant; // Important colors

    // Pixel data
    uint8_t *pixelData;    // Pointer to pixel data
} BITMAP;

typedef struct 
{
} SVG;

#ifdef __cplusplus
}
#endif  // __cplusplus