#ifndef BREWPANEL_TIMER_CONTROL_HPP
#define BREWPANEL_TIMER_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-buttons.hpp"
#include "brewpanel-images.hpp"

#define BREWPANEL_TIMER_CONTROL_MLT_DIGITS_OFFSET_X  320
#define BREWPANEL_TIMER_CONTROL_BOIL_DIGITS_OFFSET_X 620

#define BREWPANEL_TIMER_CONTROL_START_X 315
#define BREWPANEL_TIMER_CONTROL_STOP_X  315
#define BREWPANEL_TIMER_CONTROL_PAUSE_X 443
#define BREWPANEL_TIMER_CONTROL_RESET_X 443
#define BREWPANEL_TIMER_PANEL_X         300
#define BREWPANEL_TIMER_CONTROL_START_Y 325
#define BREWPANEL_TIMER_CONTROL_STOP_Y  275
#define BREWPANEL_TIMER_CONTROL_PAUSE_Y 325
#define BREWPANEL_TIMER_CONTROL_RESET_Y 275
#define BREWPANEL_TIMER_PANEL_Y         255

#define BREWPANEL_TIMER_CONTROL_BOIL_START_X (BREWPANEL_TIMER_CONTROL_START_X + 300)
#define BREWPANEL_TIMER_CONTROL_BOIL_STOP_X  (BREWPANEL_TIMER_CONTROL_STOP_X + 300)
#define BREWPANEL_TIMER_CONTROL_BOIL_PAUSE_X (BREWPANEL_TIMER_CONTROL_PAUSE_X + 300)
#define BREWPANEL_TIMER_CONTROL_BOIL_RESET_X (BREWPANEL_TIMER_CONTROL_RESET_X + 300)
#define BREWPANEL_TIMER_PANEL_BOIL_X         (BREWPANEL_TIMER_PANEL_X + 300)
#define BREWPANEL_TIMER_CONTROL_BOIL_START_Y 325
#define BREWPANEL_TIMER_CONTROL_BOIL_STOP_Y  275
#define BREWPANEL_TIMER_CONTROL_BOIL_PAUSE_Y 325
#define BREWPANEL_TIMER_CONTROL_BOIL_RESET_Y 275
#define BREWPANEL_TIMER_PANEL_BOIL_Y         255

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

struct BrewPanelTimerPanel {
    image_id panel_image;
    u32 x_offset;
    u32 y_offset;
};

struct BrewPanelTimerTimestamp {
    u32 hours;
    u32 minutes;
    u32 seconds;
};

enum BrewPanelTimerState : u8 {
    BREWPANEL_TIMER_STATE_IDLE    = 0,
    BREWPANEL_TIMER_STATE_SET     = 1,
    BREWPANEL_TIMER_STATE_RUNNING = 2,
    BREWPANEL_TIMER_STATE_PAUSED  = 3,
    BREWPANEL_TIMER_STATE_EXPIRED = 4
};

struct BrewPanelTimerControl {
    bool                         redraw;
    BrewPanelTimerControlButtons buttons;
    BrewPanelTimerPanel          panel;
    u32                          set_time_seconds;
    u32                          elapsed_time_seconds;
    BrewPanelTimerState          state;
    BrewPanelTimerDigits         digits;
};

struct BrewPanelTimers {
    BrewPanelTimerControl mash_lauter_timer;
    BrewPanelTimerControl boil_timer;
};

#define brewpanel_timer_control_create_mlt_start_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_mlt_on_start_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_START_IDLE,BREWPANEL_IMAGES_ID_TIMER_START_HOVER,BREWPANEL_IMAGES_ID_TIMER_START_CLICKED,BREWPANEL_IMAGES_ID_TIMER_START_DISABLED,BREWPANEL_TIMER_CONTROL_START_X,BREWPANEL_TIMER_CONTROL_START_Y)
#define brewpanel_timer_control_create_mlt_stop_button( buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_mlt_on_stop_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_STOP_IDLE, BREWPANEL_IMAGES_ID_TIMER_STOP_HOVER, BREWPANEL_IMAGES_ID_TIMER_STOP_CLICKED, BREWPANEL_IMAGES_ID_TIMER_STOP_DISABLED, BREWPANEL_TIMER_CONTROL_STOP_X,BREWPANEL_TIMER_CONTROL_STOP_Y)
#define brewpanel_timer_control_create_mlt_pause_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_mlt_on_pause_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_PAUSE_IDLE,BREWPANEL_IMAGES_ID_TIMER_PAUSE_HOVER,BREWPANEL_IMAGES_ID_TIMER_PAUSE_CLICKED,BREWPANEL_IMAGES_ID_TIMER_PAUSE_DISABLED,BREWPANEL_TIMER_CONTROL_PAUSE_X,BREWPANEL_TIMER_CONTROL_PAUSE_Y)
#define brewpanel_timer_control_create_mlt_reset_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_mlt_on_reset_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_RESET_IDLE,BREWPANEL_IMAGES_ID_TIMER_RESET_HOVER,BREWPANEL_IMAGES_ID_TIMER_RESET_CLICKED,BREWPANEL_IMAGES_ID_TIMER_RESET_DISABLED,BREWPANEL_TIMER_CONTROL_RESET_X,BREWPANEL_TIMER_CONTROL_RESET_Y)

#define brewpanel_timer_control_create_boil_start_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_boil_on_start_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_START_IDLE,BREWPANEL_IMAGES_ID_TIMER_START_HOVER,BREWPANEL_IMAGES_ID_TIMER_START_CLICKED,BREWPANEL_IMAGES_ID_TIMER_START_DISABLED,BREWPANEL_TIMER_CONTROL_BOIL_START_X,BREWPANEL_TIMER_CONTROL_BOIL_START_Y)
#define brewpanel_timer_control_create_boil_stop_button(buttons, images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_boil_on_stop_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_STOP_IDLE, BREWPANEL_IMAGES_ID_TIMER_STOP_HOVER, BREWPANEL_IMAGES_ID_TIMER_STOP_CLICKED, BREWPANEL_IMAGES_ID_TIMER_STOP_DISABLED, BREWPANEL_TIMER_CONTROL_BOIL_STOP_X, BREWPANEL_TIMER_CONTROL_BOIL_STOP_Y)
#define brewpanel_timer_control_create_boil_pause_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_boil_on_pause_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_PAUSE_IDLE,BREWPANEL_IMAGES_ID_TIMER_PAUSE_HOVER,BREWPANEL_IMAGES_ID_TIMER_PAUSE_CLICKED,BREWPANEL_IMAGES_ID_TIMER_PAUSE_DISABLED,BREWPANEL_TIMER_CONTROL_BOIL_PAUSE_X,BREWPANEL_TIMER_CONTROL_BOIL_PAUSE_Y)
#define brewpanel_timer_control_create_boil_reset_button(buttons,images,timer) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_boil_on_reset_button_click,(mem_data)(&timer),BREWPANEL_IMAGES_ID_TIMER_RESET_IDLE,BREWPANEL_IMAGES_ID_TIMER_RESET_HOVER,BREWPANEL_IMAGES_ID_TIMER_RESET_CLICKED,BREWPANEL_IMAGES_ID_TIMER_RESET_DISABLED,BREWPANEL_TIMER_CONTROL_BOIL_RESET_X,BREWPANEL_TIMER_CONTROL_BOIL_RESET_Y)

#endif //BREWPANEL_TIMER_CONTROL_HPP