#ifndef BREWPANEL_BUTTONS_HPP
#define BREWPANEL_BUTTONS_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-memory.hpp"
#include "brewpanel-images.hpp"

#define BREW_PANEL_MAX_BUTTONS 32

enum BrewPanelButtonState : u8
{
    BREWPANEL_BUTTON_STATE_IDLE     = 0,
    BREWPANEL_BUTTON_STATE_HOVER    = 0,
    BREWPANEL_BUTTON_STATE_CLICKED  = 0,
    BREWPANEL_BUTTON_STATE_DISABLED = 0,
};

struct BrewPanelButtonImages {
    BrewPanelImagesId idle[BREW_PANEL_MAX_BUTTONS];
    BrewPanelImagesId hover[BREW_PANEL_MAX_BUTTONS];
    BrewPanelImagesId clicked[BREW_PANEL_MAX_BUTTONS];
    BrewPanelImagesId disabled[BREW_PANEL_MAX_BUTTONS];
};

struct BrewPanelButtonOffsets {
    u32 x_pixels;
    u32 y_pixels;
};

typedef void
(*func_button_click_callback)
(void* data);

struct BrewPanelButtonStore {
    u8                         button_count;
    BrewPanelButtonImages      images;
    BrewPanelButtonOffsets     offsets[BREW_PANEL_MAX_BUTTONS];
    func_button_click_callback on_click_callbacks[BREW_PANEL_MAX_BUTTONS];
};



#endif //BREWPANEL_BUTTONS_HPP