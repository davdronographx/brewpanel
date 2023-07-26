#ifndef BREWPANEL_IMAGES_HPP
#define BREWPANEL_IMAGES_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-platform-api.hpp"

#define BREWPANEL_IMAGES_FILE "brewpanel-images.bpi"

#define BREWPANEL_IMAGES_COUNT           128
#define BREWPANEL_IMAGES_NAME_LENGTH     32
#define BREWPANEL_IMAGES_COLOR_CHANNELS  4
#define BREWPANEL_IMAGES_ALLOCATION_SIZE BREW_PANEL_PIXEL_COUNT

//TODO: rename
struct BrewPanelImage {
    s32        width_pixels;
    s32        height_pixels;
    s32        channels;
    RGBAPixel* pixels;
    mem_data   temp_image_file_data;
};

enum BrewPanelImagesId : s16 {
    BREWPANEL_IMAGES_ID_NULL                 = -1,
    BREWPANEL_IMAGES_ID_MAIN_SCREEN          = 0,
    BREWPANEL_IMAGES_ID_GREEN_BUTTON_IDLE    = 1,
    BREWPANEL_IMAGES_ID_GREEN_BUTTON_HOVER   = 2,
    BREWPANEL_IMAGES_ID_TIMER_START_IDLE     = 3,
    BREWPANEL_IMAGES_ID_TIMER_STOP_IDLE      = 4,
    BREWPANEL_IMAGES_ID_TIMER_PAUSE_IDLE     = 5,
    BREWPANEL_IMAGES_ID_TIMER_RESET_IDLE     = 6,
    BREWPANEL_IMAGES_ID_TIMER_START_HOVER    = 7,
    BREWPANEL_IMAGES_ID_TIMER_STOP_HOVER     = 8,
    BREWPANEL_IMAGES_ID_TIMER_PAUSE_HOVER    = 9,
    BREWPANEL_IMAGES_ID_TIMER_RESET_HOVER    = 10,
    BREWPANEL_IMAGES_ID_TIMER_START_CLICKED  = 11,
    BREWPANEL_IMAGES_ID_TIMER_STOP_CLICKED   = 12,
    BREWPANEL_IMAGES_ID_TIMER_PAUSE_CLICKED  = 13,
    BREWPANEL_IMAGES_ID_TIMER_RESET_CLICKED  = 14,
    BREWPANEL_IMAGES_ID_TIMER_START_DISABLED = 15,
    BREWPANEL_IMAGES_ID_TIMER_STOP_DISABLED  = 16,
    BREWPANEL_IMAGES_ID_TIMER_PAUSE_DISABLED = 17,
    BREWPANEL_IMAGES_ID_TIMER_RESET_DISABLED = 18,
    BREWPANEL_IMAGES_ID_TIMER_PANEL_MLT      = 19,
    BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL     = 20,
    BREWPANEL_IMAGES_ID_CLOCK_0              = 21,
    BREWPANEL_IMAGES_ID_CLOCK_1              = 22,
    BREWPANEL_IMAGES_ID_CLOCK_2              = 23,
    BREWPANEL_IMAGES_ID_CLOCK_3              = 24,
    BREWPANEL_IMAGES_ID_CLOCK_4              = 25,
    BREWPANEL_IMAGES_ID_CLOCK_5              = 26,
    BREWPANEL_IMAGES_ID_CLOCK_6              = 27,
    BREWPANEL_IMAGES_ID_CLOCK_7              = 28,
    BREWPANEL_IMAGES_ID_CLOCK_8              = 29,
    BREWPANEL_IMAGES_ID_CLOCK_9              = 30,
    BREWPANEL_IMAGES_ID_CLOCK_COLON          = 31,
    BREWPANEL_IMAGES_ID_CLOCK_AM             = 32,
    BREWPANEL_IMAGES_ID_CLOCK_PM             = 33,
    BREWPANEL_IMAGES_ID_COUNT                = 34
};

typedef s16 image_id;

global str brewpanel_images_file_paths[] = {
    "main-screen.png",
    "green-button-idle.png",
    "green-button-hover.png",
    "start-idle.png",
    "stop-idle.png",
    "pause-idle.png",
    "reset-idle.png",
    "start-hover.png",
    "stop-hover.png",
    "pause-hover.png",
    "reset-hover.png",
    "start-clicked.png",
    "stop-clicked.png",
    "pause-clicked.png",
    "reset-clicked.png",
    "start-disabled.png",
    "stop-disabled.png",
    "pause-disabled.png",
    "reset-disabled.png",
    "mlt-timer-panel.png",
    "boil-timer-panel.png",
    "clock-0.png",
    "clock-1.png",
    "clock-2.png",
    "clock-3.png",
    "clock-4.png",
    "clock-5.png",
    "clock-6.png",
    "clock-7.png",
    "clock-8.png",
    "clock-9.png",
    "clock-colon.png",
    "clock-am.png",
    "clock-pm.png"
};

global str brewpanel_images_image_names[] = {
    "main-screen",
    "green-button-idle",
    "green-button-hover",
    "start-idle",
    "stop-idle",
    "pause-idle",
    "reset-idle",
    "start-hover",
    "stop-hover",
    "pause-hover",
    "reset-hover",
    "start-clicked",
    "stop-clicked",
    "pause-clicked",
    "reset-clicked",
    "start-disabled",
    "stop-disabled",
    "pause-disabled",
    "reset-disabled",
    "mlt-timer-panel",
    "boils-timer-panel",
    "clock-0",
    "clock-1",
    "clock-2",
    "clock-3",
    "clock-4",
    "clock-5",
    "clock-6",
    "clock-7",
    "clock-8",
    "clock-9",
    "clock-colon",
    "clock-am",
    "clock-pm"
};

struct BrewPanelImagesFileIndex {
    u16  image_id;
    u32  image_offset;
    u32  image_width_pixels;
    u32  image_height_pixels;
    u32  image_size_pixels;
    u32  image_row_size_bytes;
    u32  image_size_bytes;    
    char image_name[BREWPANEL_IMAGES_NAME_LENGTH];
};

struct BrewPanelImagesFileHeader {
    char                     verifiction[3];
    u16                      image_count;
    u32                      image_data_size;
    BrewPanelImagesFileIndex image_indexes[BREWPANEL_IMAGES_ID_COUNT];
};

struct BrewPanelImagesFile {
    file_handle               file_handle;
    BrewPanelImagesFileHeader file_header;
    BrewPanelImage            temp_image;
    mem_data                  image_data;
};

struct BrewPanelImagesState {
    BrewPanelImagesFile images_file;
};

#define brewpanel_images_index(images_state,image_id)   images_state->images_file.file_header.image_indexes[image_id]
#define brewpanel_images_address(images_state,image_id) (RGBAPixel*)(&images_state->images_file.image_data[brewpanel_images_index(images_state,image_id).image_offset]);

#endif //BREWPANEL_IMAGES_HPP