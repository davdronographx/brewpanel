#ifndef BREWPANEL_CORE_HPP
#define BREWPANEL_CORE_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-memory.hpp"

#define BREW_PANEL_WIDTH_PIXELS  1024
#define BREW_PANEL_HEIGHT_PIXELS 600
#define BREW_PANEL_PIXEL_COUNT   (BREW_PANEL_WIDTH_PIXELS * BREW_PANEL_HEIGHT_PIXELS)

struct BrewPanelBackBuffer {
    RGBAPixel pixels[BREW_PANEL_PIXEL_COUNT];
};

struct BrewPanelState{
    BrewPanelMemory     memory;
    BrewPanelBackBuffer back_buffer;
};

#endif //BREWPANEL_CORE_HPP