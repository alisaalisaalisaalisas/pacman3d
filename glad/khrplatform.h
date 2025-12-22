/*
** Copyright (c) 2008-2018 The Khronos Group Inc.
** Minimal khrplatform.h for GLAD
*/

#ifndef __khrplatform_h_
#define __khrplatform_h_

#include <stdint.h>

typedef int32_t  khronos_int32_t;
typedef uint32_t khronos_uint32_t;
typedef int64_t  khronos_int64_t;
typedef uint64_t khronos_uint64_t;
typedef float    khronos_float_t;
typedef int8_t   khronos_int8_t;
typedef uint8_t  khronos_uint8_t;
typedef int16_t  khronos_int16_t;
typedef uint16_t khronos_uint16_t;

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
    typedef int64_t  khronos_intptr_t;
    typedef int64_t  khronos_ssize_t;
#else
    typedef int32_t  khronos_intptr_t;
    typedef int32_t  khronos_ssize_t;
#endif

#endif // __khrplatform_h_
