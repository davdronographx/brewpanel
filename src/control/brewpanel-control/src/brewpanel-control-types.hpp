#ifndef BREWPANEL_CONTROL_TYPES_HPP
#define BREWPANEL_CONTROL_TYPES_HPP

#include <Arduino.h>

#define internal static
#define local    static
#define global   static

typedef char  s8;
typedef short s16;
typedef int   s32;
typedef long  s64;

typedef unsigned long u64;

typedef float  f32;
typedef double f64;

typedef u8        mem_byte;
typedef mem_byte* mem_data;

typedef void* file_handle;

typedef char* str;

#endif //BREWPANEL_CONTROL_TYPES_HPP