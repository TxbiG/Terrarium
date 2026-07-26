#ifndef TERRARIUM_DEBUG_H
#define TERRARIUM_DEBUG_H

#include <types.h>

// DEBUGGING
#ifdef KERNEL_DEBUG
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
// Core logging function
static inline void Moss_Log(const char* prefix, const char* msg, const char* color, ...) {
    char formatBuffer[8192];
    snprintf(formatBuffer, sizeof(formatBuffer), "%s%s%s\x1b[0m\n", color, prefix, msg);

    va_list args;
    va_start(args, color);
    vprintf(formatBuffer, args);
    va_end(args);
}
#define TRACE(msg, ...) Moss_Log("TRACE: ", msg, "\x1b[37m", ##__VA_ARGS__) // White
#define DEBUG(msg, ...) Moss_Log("DEBUG: ", msg, "\x1b[36m", ##__VA_ARGS__) // Cyan
#define INFO(msg, ...)  Moss_Log("INFO:  ", msg, "\x1b[32m", ##__VA_ARGS__) // Green
#define WARN(msg, ...)  Moss_Log("WARN:  ", msg, "\x1b[33m", ##__VA_ARGS__) // Yellow
#define ERROR(msg, ...) Moss_Log("ERROR: ", msg, "\x1b[31m", ##__VA_ARGS__) // Red
#define FATAL(msg, ...) Moss_Log("FATAL: ", msg, "\x1b[35m", ##__VA_ARGS__) // Magenta

#define CHECK(x, msg, ...)                                            \
    do {                                                              \
        if (!(x)) {                                                   \
            FATAL("Assertion Failed: " + msg, ##__VA_ARGS__);         \
            abort();                                             	  \
        }                                                             \
    } while (0)

// Assertion with logging
#define MOSS_ASSERT(x, msg, ...)                                      \
    do {                                                              \
        if (!(x)) {                                                   \
            FATAL("Assertion Failed: " + msg, ##__VA_ARGS__);         \
            DEBUG_BREAK();                                            \
            exit(1);                                       			  \
        }                                                             \
    } while (0)
#else
	#define TRACE(msg, ...)
	#define DEBUG(msg, ...)
	#define INFO(msg, ...)
	#define WARN(msg, ...)
	#define ERROR(msg, ...)
	#define FATAL(msg, ...)
	#define CHECK(x, msg, ...)
	#define ASSERT(x, msg, ...)
#endif // KERNEL_DEBUG



// Memory management
KERNEL_API void* Moss_malloc(size_t size);
KERNEL_API void* Moss_calloc(size_t nmemb, size_t size);
KERNEL_API void* Moss_realloc(void *mem, size_t size);
KERNEL_API void* Moss_aligned_alloc(size_t alignment, size_t size);
KERNEL_API void  Moss_aligned_free(void* mem);
KERNEL_API void  Moss_free(void* mem);

#endif // TERRARIUM_DEBUG_H
