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

typedef float  f32;
typedef double f64;

typedef u8        mem_byte;
typedef mem_byte* mem_data;

typedef void* file_handle;
typedef void* controller_handle;
typedef void* thread_handle;

typedef char* str;

struct BrewPanelControllerInfo {
    str vendor_id;
    str product_id;
    str serial_number;
};

struct RGBAPixel {
    u8 blue;
    u8 green;
    u8 red;
    u8 alpha;
};

struct BrewPanelInput {
    bool click;
    u32  mouse_x_pos;
    u32  mouse_y_pos;
};

struct BrewPanelSystemTime {
    u8 hours;
    u8 minutes;
    u8 seconds;
};

enum BrewPanelMode : u8 {
    BREWPANEL_MODE_OFF = 0,
    BREWPANEL_MODE_MASH = 1,
    BREWPANEL_MODE_BOIL = 2
};
typedef BrewPanelMode panel_mode;

#define BREWPANEL_CONTROL_COMM_DATA_BUFFER_SIZE 368

struct BrewPanelControlCommData {
    u64               bytes_to_write;
    u64               bytes_read;
    mem_byte          write_buffer[BREWPANEL_CONTROL_COMM_DATA_BUFFER_SIZE];
    mem_byte          read_buffer[BREWPANEL_CONTROL_COMM_DATA_BUFFER_SIZE];
    controller_handle controller;
};

typedef BrewPanelControlCommData controller_comm_data;

#define brewpanel_kilobytes(value) ((u64)value * 1024)
#define brewpanel_megabytes(value) (brewpanel_kilobytes(value) * 1024)
#define brewpanel_gigabytes(value) (brewpanel_megabytes(value) * 1024)
#define brewpanel_terabytes(value) (brewpanel_gigabytes(value) * 1024)

#define BREW_PANEL_WIDTH_PIXELS  1024
#define BREW_PANEL_HEIGHT_PIXELS 600
#define BREW_PANEL_PIXEL_COUNT   614400

#define BREW_PANEL_ROW_BYTES    (BREW_PANEL_WIDTH_PIXELS * sizeof(RGBAPixel))
#define BREW_PANEL_BYTES_COUNT  (BREW_PANEL_PIXEL_COUNT  * sizeof(RGBAPixel))

#endif //BREWPANEL_HPP