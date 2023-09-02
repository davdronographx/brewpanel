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
    BREWPANEL_IMAGES_ID_NULL                               = -1,
    BREWPANEL_IMAGES_ID_MAIN_SCREEN                        = 0,
    BREWPANEL_IMAGES_ID_GREEN_BUTTON_IDLE                  = 1,
    BREWPANEL_IMAGES_ID_GREEN_BUTTON_HOVER                 = 2,
    BREWPANEL_IMAGES_ID_TIMER_START_IDLE                   = 3,
    BREWPANEL_IMAGES_ID_TIMER_STOP_IDLE                    = 4,
    BREWPANEL_IMAGES_ID_TIMER_PAUSE_IDLE                   = 5,
    BREWPANEL_IMAGES_ID_TIMER_RESET_IDLE                   = 6,
    BREWPANEL_IMAGES_ID_TIMER_START_HOVER                  = 7,
    BREWPANEL_IMAGES_ID_TIMER_STOP_HOVER                   = 8,
    BREWPANEL_IMAGES_ID_TIMER_PAUSE_HOVER                  = 9,
    BREWPANEL_IMAGES_ID_TIMER_RESET_HOVER                  = 10,
    BREWPANEL_IMAGES_ID_TIMER_START_CLICKED                = 11,
    BREWPANEL_IMAGES_ID_TIMER_STOP_CLICKED                 = 12,
    BREWPANEL_IMAGES_ID_TIMER_PAUSE_CLICKED                = 13,
    BREWPANEL_IMAGES_ID_TIMER_RESET_CLICKED                = 14,
    BREWPANEL_IMAGES_ID_TIMER_START_DISABLED               = 15,
    BREWPANEL_IMAGES_ID_TIMER_STOP_DISABLED                = 16,
    BREWPANEL_IMAGES_ID_TIMER_PAUSE_DISABLED               = 17,
    BREWPANEL_IMAGES_ID_TIMER_RESET_DISABLED               = 18,
    BREWPANEL_IMAGES_ID_TIMER_PANEL_MLT                    = 19,
    BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL                   = 20,
    BREWPANEL_IMAGES_ID_CLOCK_0                            = 21,
    BREWPANEL_IMAGES_ID_CLOCK_1                            = 22,
    BREWPANEL_IMAGES_ID_CLOCK_2                            = 23,
    BREWPANEL_IMAGES_ID_CLOCK_3                            = 24,
    BREWPANEL_IMAGES_ID_CLOCK_4                            = 25,
    BREWPANEL_IMAGES_ID_CLOCK_5                            = 26,
    BREWPANEL_IMAGES_ID_CLOCK_6                            = 27,
    BREWPANEL_IMAGES_ID_CLOCK_7                            = 28,
    BREWPANEL_IMAGES_ID_CLOCK_8                            = 29,
    BREWPANEL_IMAGES_ID_CLOCK_9                            = 30,
    BREWPANEL_IMAGES_ID_CLOCK_COLON                        = 31,
    BREWPANEL_IMAGES_ID_CLOCK_AM                           = 32,
    BREWPANEL_IMAGES_ID_CLOCK_PM                           = 33,
    BREWPANEL_IMAGES_ID_RED_DIGIT_0                        = 34,
    BREWPANEL_IMAGES_ID_RED_DIGIT_1                        = 35,
    BREWPANEL_IMAGES_ID_RED_DIGIT_2                        = 36,
    BREWPANEL_IMAGES_ID_RED_DIGIT_3                        = 37,
    BREWPANEL_IMAGES_ID_RED_DIGIT_4                        = 38,
    BREWPANEL_IMAGES_ID_RED_DIGIT_5                        = 39,
    BREWPANEL_IMAGES_ID_RED_DIGIT_6                        = 40,
    BREWPANEL_IMAGES_ID_RED_DIGIT_7                        = 41,
    BREWPANEL_IMAGES_ID_RED_DIGIT_8                        = 42,
    BREWPANEL_IMAGES_ID_RED_DIGIT_9                        = 43,
    BREWPANEL_IMAGES_ID_RED_DIGIT_COLON                    = 44,
    BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE                   = 45,
    BREWPANEL_IMAGES_ID_RED_DIGIT_F                        = 46,
    BREWPANEL_IMAGES_ID_RED_DIGIT_PERCENT                  = 47,
    BREWPANEL_IMAGES_ID_HLT_TEMP_PANEL                     = 48, 
    BREWPANEL_IMAGES_ID_MLT_TEMP_PANEL                     = 49, 
    BREWPANEL_IMAGES_ID_BOIL_TEMP_PANEL                    = 50,
    BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL                  = 51,
    BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL                 = 52,
    BREWPANEL_IMAGES_ID_MODE_PANEL                         = 53,
    BREWPANEL_IMAGES_ID_MODE_MASH_ON_IDLE                  = 54, 
    BREWPANEL_IMAGES_ID_MODE_MASH_ON_HOVER                 = 55, 
    BREWPANEL_IMAGES_ID_MODE_MASH_ON_CLICKED               = 56, 
    BREWPANEL_IMAGES_ID_MODE_MASH_ON_DISABLED              = 57, 
    BREWPANEL_IMAGES_ID_MODE_MASH_OFF_IDLE                 = 58, 
    BREWPANEL_IMAGES_ID_MODE_MASH_OFF_HOVER                = 59, 
    BREWPANEL_IMAGES_ID_MODE_MASH_OFF_CLICKED              = 60, 
    BREWPANEL_IMAGES_ID_MODE_MASH_OFF_DISABLED             = 61, 
    BREWPANEL_IMAGES_ID_MODE_BOIL_ON_IDLE                  = 62, 
    BREWPANEL_IMAGES_ID_MODE_BOIL_ON_HOVER                 = 63, 
    BREWPANEL_IMAGES_ID_MODE_BOIL_ON_CLICKED               = 64, 
    BREWPANEL_IMAGES_ID_MODE_BOIL_ON_DISABLED              = 65, 
    BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_IDLE                 = 66, 
    BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_HOVER                = 67, 
    BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_CLICKED              = 68, 
    BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_DISABLED             = 69, 
    BREWPANEL_IMAGES_ID_KEYPAD_PANEL                       = 70,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_DELETE_IDLE          = 71,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_0_IDLE               = 72,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_1_IDLE               = 73,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_2_IDLE               = 74,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_3_IDLE               = 75,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_4_IDLE               = 76,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_5_IDLE               = 77,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_6_IDLE               = 78,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_7_IDLE               = 79,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_8_IDLE               = 80,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_9_IDLE               = 81,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_SET_IDLE             = 82,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_CANCEL_IDLE          = 83,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_DELETE_HOVER         = 84,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_0_HOVER              = 85,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_1_HOVER              = 86,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_2_HOVER              = 87,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_3_HOVER              = 88,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_4_HOVER              = 89,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_5_HOVER              = 90,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_6_HOVER              = 91,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_7_HOVER              = 92,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_8_HOVER              = 93,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_9_HOVER              = 94,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_SET_HOVER            = 95,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_CANCEL_HOVER         = 96,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_DELETE_CLICKED       = 97,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_0_CLICKED            = 98,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_1_CLICKED            = 99,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_2_CLICKED            = 100,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_3_CLICKED            = 101,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_4_CLICKED            = 102,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_5_CLICKED            = 103,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_6_CLICKED            = 104,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_7_CLICKED            = 105,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_8_CLICKED            = 106,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_9_CLICKED            = 107,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_SET_CLICKED          = 108,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_CANCEL_CLICKED       = 109,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_DELETE_DISABLED      = 110,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_0_DISABLED           = 111,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_1_DISABLED           = 112,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_2_DISABLED           = 113,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_3_DISABLED           = 114,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_4_DISABLED           = 115,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_5_DISABLED           = 116,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_6_DISABLED           = 117,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_7_DISABLED           = 118,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_8_DISABLED           = 119,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_9_DISABLED           = 120,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_SET_DISABLED         = 121,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_CANCEL_DISABLED      = 122,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_IDLE     = 123,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_IDLE     = 124,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_HOVER    = 125,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_HOVER    = 126,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_CLICKED  = 127,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_CLICKED  = 128,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_DISABLED = 129,
    BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_DISABLED = 130,
    BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL_INPUT            = 131,
    BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL_INPUT           = 132,
    BREWPANEL_IMAGES_ID_KEYPAD_PANEL_ACTIVE                = 133,
    BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL_ON               = 134,
    BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL_ON              = 135,
    BREWPANEL_IMAGES_ID_TIMER_PANEL_MASH_INPUT             = 136,
    BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL_INPUT             = 137,
    BREWPANEL_IMAGES_ID_OFF_TEMP_PANEL                     = 138,
    BREWPANEL_IMAGES_ID_TIMER_PANEL_OFF                    = 139,
    BREWPANEL_IMAGES_ID_COUNT                              = 140
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
    "clock-pm.png",
    "red-digit-0.png",
    "red-digit-1.png",
    "red-digit-2.png",
    "red-digit-3.png",
    "red-digit-4.png",
    "red-digit-5.png",
    "red-digit-6.png",
    "red-digit-7.png",
    "red-digit-8.png",
    "red-digit-9.png",
    "red-digit-colon.png",
    "red-digit-degree.png",
    "red-digit-f.png",
    "red-digit-percent.png",
    "hlt-temp-panel.png",
    "mlt-temp-panel.png",
    "boil-temp-panel.png",
    "mlt-element-panel.png",
    "boil-element-panel.png",
    "mode-panel.png",
    "mash-mode-on-idle.png",
    "mash-mode-on-hover.png",
    "mash-mode-on-clicked.png",
    "mash-mode-on-disabled.png",
    "mash-mode-off-idle.png",
    "mash-mode-off-hover.png",
    "mash-mode-off-clicked.png",
    "mash-mode-off-disabled.png",
    "boil-mode-on-idle.png",
    "boil-mode-on-hover.png",
    "boil-mode-on-clicked.png",
    "boil-mode-on-disabled.png",
    "boil-mode-off-idle.png",
    "boil-mode-off-hover.png",
    "boil-mode-off-clicked.png",
    "boil-mode-off-disabled.png",
    "keypad-panel.png",
    "delete-button-idle.png",
    "0-button-idle.png",
    "1-button-idle.png",
    "2-button-idle.png",
    "3-button-idle.png",
    "4-button-idle.png",
    "5-button-idle.png",
    "6-button-idle.png",
    "7-button-idle.png",
    "8-button-idle.png",
    "9-button-idle.png",
    "set-button-idle.png",
    "cancel-button-idle.png",
    "delete-button-hover.png",
    "0-button-hover.png",
    "1-button-hover.png",
    "2-button-hover.png",
    "3-button-hover.png",
    "4-button-hover.png",
    "5-button-hover.png",
    "6-button-hover.png",
    "7-button-hover.png",
    "8-button-hover.png",
    "9-button-hover.png",
    "set-button-hover.png",
    "cancel-button-hover.png",
    "delete-button-clicked.png",
    "0-button-clicked.png",
    "1-button-clicked.png",
    "2-button-clicked.png",
    "3-button-clicked.png",
    "4-button-clicked.png",
    "5-button-clicked.png",
    "6-button-clicked.png",
    "7-button-clicked.png",
    "8-button-clicked.png",
    "9-button-clicked.png",
    "set-button-clicked.png",
    "cancel-button-clicked.png",
    "delete-button-disabled.png",
    "0-button-disabled.png",
    "1-button-disabled.png",
    "2-button-disabled.png",
    "3-button-disabled.png",
    "4-button-disabled.png",
    "5-button-disabled.png",
    "6-button-disabled.png",
    "7-button-disabled.png",
    "8-button-disabled.png",
    "9-button-disabled.png",
    "set-button-disabled.png",
    "cancel-button-disabled.png",
    "element-set-idle.png",
    "element-off-idle.png",
    "element-set-hover.png",
    "element-off-hover.png",
    "element-set-clicked.png",
    "element-off-clicked.png",
    "element-set-disabled.png",
    "element-off-disabled.png",
    "mlt-element-panel-input.png",
    "boil-element-panel-input.png",
    "keypad-active.png",
    "mlt-element-panel-on.png",  
    "boil-element-panel-on.png",
    "mash-timer-panel-input.png",
    "boil-timer-panel-input.png",
    "off-element-panel.png",
    "off-timer-panel.png"
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
    "boil-timer-panel",
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
    "clock-pm",
    "red-digit-0",
    "red-digit-1",
    "red-digit-2",
    "red-digit-3",
    "red-digit-4",
    "red-digit-5",
    "red-digit-6",
    "red-digit-7",
    "red-digit-8",
    "red-digit-9",
    "red-digit-colon",
    "red-digit-degree",
    "red-digit-f",
    "red-digit-percent",
    "hlt-temp-panel",
    "mlt-temp-panel",
    "boil-temp-panel",
    "mlt-element-panel",
    "boil-element-panel",
    "mode-panel",
    "mash-mode-on-idles",
    "mash-mode-on-hover",
    "mash-mode-on-clicked",
    "mash-mode-on-disabled",
    "mash-mode-off-idle",
    "mash-mode-off-hover",
    "mash-mode-off-clicked",
    "mash-mode-off-disabled",
    "boil-mode-on-idle",
    "boil-mode-on-hover",
    "boil-mode-on-clicked",
    "boil-mode-on-disabled",
    "boil-mode-off-idle",
    "boil-mode-off-hover",
    "boil-mode-off-clicked",
    "boil-mode-off-disabled",
    "keypad-panel",
    "delete-button-idle",
    "0-button-idle",
    "1-button-idle",
    "2-button-idle",
    "3-button-idle",
    "4-button-idle",
    "5-button-idle",
    "6-button-idle",
    "7-button-idle",
    "8-button-idle",
    "9-button-idle",
    "set-button-idle",
    "cancel-button-idle",
    "delete-button-hover",
    "0-button-hover",
    "1-button-hover",
    "2-button-hover",
    "3-button-hover",
    "4-button-hover",
    "5-button-hover",
    "6-button-hover",
    "7-button-hover",
    "8-button-hover",
    "9-button-hover",
    "set-button-hover",
    "cancel-button-hover",
    "delete-button-clicked",
    "0-button-clicked",
    "1-button-clicked",
    "2-button-clicked",
    "3-button-clicked",
    "4-button-clicked",
    "5-button-clicked",
    "6-button-clicked",
    "7-button-clicked",
    "8-button-clicked",
    "9-button-clicked",
    "set-button-clicked",
    "cancel-button-clicked",
    "delete-button-disabled",
    "0-button-disabled",
    "1-button-disabled",
    "2-button-disabled",
    "3-button-disabled",
    "4-button-disabled",
    "5-button-disabled",
    "6-button-disabled",
    "7-button-disabled",
    "8-button-disabled",
    "9-button-disabled",
    "set-button-disabled",
    "cancel-button-disabled",
    "element-set-idle",
    "element-off-idle",
    "element-set-hover",
    "element-off-hover",
    "element-set-clicked",
    "element-off-clicked",
    "element-set-disabled",
    "element-off-disabled",
    "mlt-element-panel-input",
    "boil-element-panel-input",
    "keypad-active",
    "mlt-element-panel-on",  
    "boil-element-panel-on",
    "mash-timer-panel-input",
    "boil-timer-panel-input",
    "off-element-panel",
    "off-timer-panel"
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

struct BrewPanelImageInstance {
    image_id image_id;
    u32      x_offset;
    u32      y_offset;  
};

typedef BrewPanelImagesFile       images_file;
typedef BrewPanelImagesFileIndex  image_index;
typedef BrewPanelImagesFileHeader images_file_header;
typedef BrewPanelImageInstance    image_instance;
typedef u8                        image_instance_id;

struct BrewPanelImagesStore {
    BrewPanelImagesFile images_file;
    image_instance      image_instances[BREWPANEL_IMAGES_COUNT];
    image_instance_id   image_instance_count;
    mem_data            draw_buffer;
};

typedef BrewPanelImagesStore images_store;

#define brewpanel_images_index(images_state,image_id)   images_state->images_file.file_header.image_indexes[image_id]
#define brewpanel_images_address(images_state,image_id) (RGBAPixel*)(&images_state->images_file.image_data[brewpanel_images_index(images_state,image_id).image_offset])

#endif //BREWPANEL_IMAGES_HPP