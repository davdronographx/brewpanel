#ifndef BREWPANEL_CORE_HPP
#define BREWPANEL_CORE_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-memory.hpp"
#include "brewpanel-images.hpp"

struct BrewPanelBackBuffer {
    RGBAPixel pixels[BREW_PANEL_PIXEL_COUNT];
};

struct BrewPanelState{
    BrewPanelMemory      memory;
    BrewPanelBackBuffer  back_buffer;
    BrewPanelImagesState images;
};

global BrewPanelState* brewpanel_state;

#define brewpanel_back_buffer_data() (mem_data)brewpanel_state->back_buffer.pixels      

#endif //BREWPANEL_CORE_HPP