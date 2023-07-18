#ifndef BREWPANEL_BUTTONS_HPP
#define BREWPANEL_BUTTONS_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-memory.hpp"
#include "brewpanel-images.hpp"

#define BREW_PANEL_BUTTONS_MAX 32
#define BREW_PANEL_BUTTONS_NULL -1

enum BrewPanelButtonState : u8
{
    BREWPANEL_BUTTON_STATE_IDLE     = 0,
    BREWPANEL_BUTTON_STATE_HOVER    = 1,
    BREWPANEL_BUTTON_STATE_CLICKED  = 2,
    BREWPANEL_BUTTON_STATE_DISABLED = 3,
};

struct BrewPanelButtonImages {
    BrewPanelImagesId idle[BREW_PANEL_BUTTONS_MAX];
    BrewPanelImagesId hover[BREW_PANEL_BUTTONS_MAX];
    BrewPanelImagesId clicked[BREW_PANEL_BUTTONS_MAX];
    BrewPanelImagesId disabled[BREW_PANEL_BUTTONS_MAX];
};

struct BrewPanelButtonOffsets {
    u32 x_pixels;
    u32 y_pixels;
};

typedef void
(*func_button_click_callback)
(void* data);

struct BrewPanelButtonStore {
    s8                         button_count;
    BrewPanelButtonImages      images;
    BrewPanelButtonOffsets     offsets[BREW_PANEL_BUTTONS_MAX];
    func_button_click_callback on_click_callbacks[BREW_PANEL_BUTTONS_MAX];
    s8*                        button_id_matrix;
};



#endif //BREWPANEL_BUTTONS_HPP