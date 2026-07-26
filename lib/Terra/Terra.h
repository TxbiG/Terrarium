#ifndef TERRARIUM_LIB_H
#define TERRARIUM_LIB_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#if defined(__x86_64__) || defined(_M_X64)
    #define OS_ARCH_X86_64
#elif defined(__i386__) || defined(_M_IX86)
    #define OS_ARCH_X86
#elif defined(__aarch64__)
    #define OS_ARCH_AARCH64
#elif defined(__arm__)
    #define OS_ARCH_ARM
#else
    #define OS_ARCH_UNKNOWN
#endif

#ifdef __cplusplus
}
#endif

#endif // TERRARIUM_LIB_H
