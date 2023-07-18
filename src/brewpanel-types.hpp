#ifndef BREWPANEL_HPP
#define BREWPANEL_HPP

#include <stdint.h>

#define brewpanel_assert(x) if(!(x)) *((int*)(NULL)) = 1
#define brewpanel_nop()     brewpanel_assert(true)
#define brewpanel_panic()   brewpanel_assert(false)

#define internal static
#define local    static
#define global   static

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8        mem_byte;
typedef mem_byte* mem_data;

typedef void* file_handle;

typedef char* str;

struct RGBAPixel {
    u8 blue;
    u8 green;
    u8 red;
    u8 alpha;
};

#define brewpanel_kilobytes(value) ((u64)value * 1024)
#define brewpanel_megabytes(value) (brewpanel_kilobytes(value) * 1024)
#define brewpanel_gigabytes(value) (brewpanel_megabytes(value) * 1024)
#define brewpanel_terabytes(value) (brewpanel_gigabytes(value) * 1024)

#define BREW_PANEL_WIDTH_PIXELS  1024
#define BREW_PANEL_HEIGHT_PIXELS 600
#define BREW_PANEL_PIXEL_COUNT   614400


#endif //BREWPANEL_HPP