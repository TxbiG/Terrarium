#ifndef KERNEL_STD_H
#define KERNEL_STD_H


#define KERNEL_API extern

typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed long long    int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

#if defined(TERRA64)
typedef unsigned long long  size_t;
typedef signed long long ssize_t;
#else
typedef unsigned int        size_t;
typedef signed long long    ssize_t;
#endif

// Signed
#define MAX_INT8    ((int8)(0x7F))
#define MAX_INT16   ((int16)(0x7FFF))
#define MAX_INT32   ((int32)(0x7FFFFFFF))
#define MAX_INT64   ((int64)(0x7FFFFFFFFFFFFFFF))
#define MIN_INT8    ((int8)(~0x7F))
#define MIN_INT16   ((int16)~0x7FFF)
#define MIN_INT32   ((int32)(~0x7FFFFFFF))
#define MIN_INT64   ((int64)(~0x7FFFFFFFFFFFFFFF))

// Unsigned
#define MAX_UINT8   ((uint8)(0xFF))
#define MAX_UINT16  ((uint16)(0xFFFF))
#define MAX_UINT32  ((uint32)(0xFFFFFFFFu))
#define MAX_UINT64  ((uint64)(0xFFFFFFFFFFFFFFFF))
#define MIN_UINT8   ((uint8)0x00)
#define MIN_UINT16  ((uint16)0x0000)
#define MIN_UINT32  ((uint32)0x00000000)
#define MIN_UINT64  ((uint64)(0x0000000000000000))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // KERNEL_STD_H
