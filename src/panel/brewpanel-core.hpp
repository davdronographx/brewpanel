#ifndef BREWPANEL_CORE_HPP
#define BREWPANEL_CORE_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-memory.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-core.hpp"
#include "brewpanel-buttons.hpp"
#include "brewpanel-timer-control.hpp"
#include "brewpanel-clock.hpp"
#include "brewpanel-communication.hpp"
#include "brewpanel-temp-control.hpp"
#include "brewpanel-mode-control.hpp"
#include "brewpanel-keypad.hpp"

struct BrewPanelBackBuffer {
    RGBAPixel pixels[BREW_PANEL_PIXEL_COUNT];
};

struct BrewPanelState{
    BrewPanelMemory         memory;
    BrewPanelBackBuffer     back_buffer;
    images_store            images;
    button_store            button_store;
    BrewPanelTimerControl   timer_control;
    BrewPanelClock          clock;
    comm_handler            comm_handler;
    temp_control            temp_control;
    mode_control            mode_control;
    keypad                  keypad;
    image_instance_id       main_screen;
};

global BrewPanelState* brewpanel_state;

#define brewpanel_back_buffer_data() (mem_data)brewpanel_state->back_buffer.pixels      

#define brewpanel_core_render_main_screen() brewpanel_images_draw_image_immediate(&brewpanel_state->images,BREWPANEL_IMAGES_ID_MAIN_SCREEN,0,0)

#endif //BREWPANEL_CORE_HPP