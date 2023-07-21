#ifndef BREWPANEL_TIMER_CONTROL_HPP
#define BREWPANEL_TIMER_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-buttons.hpp"
#include "brewpanel-images.hpp"

#define BREWPANEL_TIMER_CONTROL_START_X 315
#define BREWPANEL_TIMER_CONTROL_START_Y 325
#define BREWPANEL_TIMER_CONTROL_STOP_X  315
#define BREWPANEL_TIMER_CONTROL_STOP_Y  275
#define BREWPANEL_TIMER_CONTROL_PAUSE_X 443
#define BREWPANEL_TIMER_CONTROL_PAUSE_Y 325
#define BREWPANEL_TIMER_CONTROL_RESET_X 443
#define BREWPANEL_TIMER_CONTROL_RESET_Y 275

#define BREWPANEL_TIMER_PANEL_X 300
#define BREWPANEL_TIMER_PANEL_Y 255

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

struct BrewPanelTimerControl {
    bool                         redraw;
    BrewPanelTimerControlButtons buttons;
    BrewPanelTimerPanel          panel;
};

#define brewpanel_timer_control_create_start_button(buttons,images) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_on_start_button_click,BREWPANEL_IMAGES_ID_TIMER_START_IDLE,BREWPANEL_IMAGES_ID_TIMER_START_HOVER,BREWPANEL_IMAGES_ID_TIMER_START_CLICKED,BREWPANEL_IMAGES_ID_TIMER_START_DISABLED,BREWPANEL_TIMER_CONTROL_START_X,BREWPANEL_TIMER_CONTROL_START_Y)
#define brewpanel_timer_control_create_stop_button(buttons,images)  brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_on_start_button_click,BREWPANEL_IMAGES_ID_TIMER_STOP_IDLE, BREWPANEL_IMAGES_ID_TIMER_STOP_HOVER, BREWPANEL_IMAGES_ID_TIMER_STOP_CLICKED, BREWPANEL_IMAGES_ID_TIMER_STOP_DISABLED, BREWPANEL_TIMER_CONTROL_STOP_X,BREWPANEL_TIMER_CONTROL_STOP_Y)
#define brewpanel_timer_control_create_pause_button(buttons,images) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_on_start_button_click,BREWPANEL_IMAGES_ID_TIMER_PAUSE_IDLE,BREWPANEL_IMAGES_ID_TIMER_PAUSE_HOVER,BREWPANEL_IMAGES_ID_TIMER_PAUSE_CLICKED,BREWPANEL_IMAGES_ID_TIMER_PAUSE_DISABLED,BREWPANEL_TIMER_CONTROL_PAUSE_X,BREWPANEL_TIMER_CONTROL_PAUSE_Y)
#define brewpanel_timer_control_create_reset_button(buttons,images) brewpanel_buttons_create_button(buttons,images,brewpanel_timer_control_on_start_button_click,BREWPANEL_IMAGES_ID_TIMER_RESET_IDLE,BREWPANEL_IMAGES_ID_TIMER_RESET_HOVER,BREWPANEL_IMAGES_ID_TIMER_RESET_CLICKED,BREWPANEL_IMAGES_ID_TIMER_RESET_DISABLED,BREWPANEL_TIMER_CONTROL_RESET_X,BREWPANEL_TIMER_CONTROL_RESET_Y)


#endif //BREWPANEL_TIMER_CONTROL_HPP