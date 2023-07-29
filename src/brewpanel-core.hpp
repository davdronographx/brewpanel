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

struct BrewPanelBackBuffer {
    RGBAPixel pixels[BREW_PANEL_PIXEL_COUNT];
};

struct BrewPanelState{
    BrewPanelMemory       memory;
    BrewPanelBackBuffer   back_buffer;
    BrewPanelImagesState  images;
    BrewPanelButtonStore  button_store;
    BrewPanelTimers       timers;
    BrewPanelClock        clock;
    comm_handler          comm_handler;
};

global BrewPanelState* brewpanel_state;

#define brewpanel_back_buffer_data() (mem_data)brewpanel_state->back_buffer.pixels      

#define brewpanel_core_render_main_screen() brewpanel_images_draw_image(&brewpanel_state->images,BREWPANEL_IMAGES_ID_MAIN_SCREEN,0,0,(mem_data)brewpanel_state->back_buffer.pixels)

#endif //BREWPANEL_CORE_HPP