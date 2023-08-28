#ifndef BREWPANEL_TIMER_CONTROL_HPP
#define BREWPANEL_TIMER_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-buttons.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-keypad.hpp"

#define BREWPANEL_TIMER_CONTROL_MLT_DIGITS_OFFSET_X  320
#define BREWPANEL_TIMER_CONTROL_BOIL_DIGITS_OFFSET_X 620

#define BREWPANEL_TIMER_PANEL_X     299
#define BREWPANEL_TIMER_PANEL_Y     355
#define BREWPANEL_TIMER_DIGIT_X     318
#define BREWPANEL_TIMER_DIGIT_Y     376
#define BREWPANEL_TIMER_DIGIT_WIDTH 30

#define BREWPANEL_TIMER_BUTTON_X        575
#define BREWPANEL_TIMER_BUTTON_TOP_Y    415
#define BREWPANEL_TIMER_BUTTON_BOTTOM_Y 365

const image_id brewpanel_timer_glyph_table[] = {
    BREWPANEL_IMAGES_ID_RED_DIGIT_0,
    BREWPANEL_IMAGES_ID_RED_DIGIT_1,
    BREWPANEL_IMAGES_ID_RED_DIGIT_2,
    BREWPANEL_IMAGES_ID_RED_DIGIT_3,
    BREWPANEL_IMAGES_ID_RED_DIGIT_4,
    BREWPANEL_IMAGES_ID_RED_DIGIT_5,
    BREWPANEL_IMAGES_ID_RED_DIGIT_6,
    BREWPANEL_IMAGES_ID_RED_DIGIT_7,
    BREWPANEL_IMAGES_ID_RED_DIGIT_8,
    BREWPANEL_IMAGES_ID_RED_DIGIT_9
};

struct BrewPanelTimerDigitImages {
    image_id tens_face;
    image_id ones_face;
};

struct BrewPanelTimerDigits {
    BrewPanelTimerDigitImages hours;
    BrewPanelTimerDigitImages minutes;
    BrewPanelTimerDigitImages seconds;
};

struct BrewPanelTimerControlButtons {
    button_id start_button_id;
    button_id stop_button_id;
    button_id pause_button_id;
    button_id reset_button_id;
};

struct BrewPanelTimerTimestamp {
    u32 hours;
    u32 minutes;
    u32 seconds;
};

typedef BrewPanelTimerTimestamp timer_timestamp;

enum BrewPanelTimerState : u8 {
    BREWPANEL_TIMER_STATE_IDLE    = 0,
    BREWPANEL_TIMER_STATE_SET     = 1,
    BREWPANEL_TIMER_STATE_RUNNING = 2,
    BREWPANEL_TIMER_STATE_PAUSED  = 3,
    BREWPANEL_TIMER_STATE_EXPIRED = 4
};

typedef BrewPanelTimerState timer_state;

struct BrewPanelTimer {
    bool                         redraw;
    BrewPanelTimerControlButtons buttons;
    u32                          set_time_seconds;
    u32                          elapsed_time_seconds;
    timer_state                  state;
    timer_state                  previous_state;
    BrewPanelTimerDigits         digits;
    image_instance_id            panel_image;
    image_instance_id            colon_1;
    image_instance_id            colon_2;
    keypad_input                 keypad_input;
};

typedef BrewPanelTimer timer;

struct BrewPanelTimerControl {
    BrewPanelTimer mash_timer;
    BrewPanelTimer boil_timer;
};
typedef BrewPanelTimerControl timer_control;

#define brewpanel_timer_control_create_reset_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_on_reset_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_RESET_IDLE,BREWPANEL_IMAGES_ID_TIMER_RESET_HOVER,BREWPANEL_IMAGES_ID_TIMER_RESET_CLICKED,BREWPANEL_IMAGES_ID_TIMER_RESET_DISABLED,BREWPANEL_TIMER_BUTTON_X,BREWPANEL_TIMER_BUTTON_TOP_Y)
#define brewpanel_timer_control_create_stop_button( buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_on_stop_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_STOP_IDLE, BREWPANEL_IMAGES_ID_TIMER_STOP_HOVER, BREWPANEL_IMAGES_ID_TIMER_STOP_CLICKED, BREWPANEL_IMAGES_ID_TIMER_STOP_DISABLED,  BREWPANEL_TIMER_BUTTON_X,BREWPANEL_TIMER_BUTTON_BOTTOM_Y)
#define brewpanel_timer_control_create_start_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_on_start_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_START_IDLE,BREWPANEL_IMAGES_ID_TIMER_START_HOVER,BREWPANEL_IMAGES_ID_TIMER_START_CLICKED,BREWPANEL_IMAGES_ID_TIMER_START_DISABLED,BREWPANEL_TIMER_BUTTON_X,BREWPANEL_TIMER_CONTROL_START_Y)
#define brewpanel_timer_control_create_pause_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_on_pause_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_PAUSE_IDLE,BREWPANEL_IMAGES_ID_TIMER_PAUSE_HOVER,BREWPANEL_IMAGES_ID_TIMER_PAUSE_CLICKED,BREWPANEL_IMAGES_ID_TIMER_PAUSE_DISABLED,BREWPANEL_TIMER_BUTTON_X,BREWPANEL_TIMER_CONTROL_PAUSE_Y)

#endif //BREWPANEL_TIMER_CONTROL_HPP