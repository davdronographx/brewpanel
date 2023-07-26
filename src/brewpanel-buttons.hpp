#ifndef BREWPANEL_BUTTONS_HPP
#define BREWPANEL_BUTTONS_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-memory.hpp"
#include "brewpanel-images.hpp"

#define BREW_PANEL_BUTTONS_MAX 32
#define BREW_PANEL_BUTTONS_NULL -1

typedef s8 button_id;

enum BrewPanelButtonState : u8
{
    BREWPANEL_BUTTON_STATE_NULL     = 0,
    BREWPANEL_BUTTON_STATE_IDLE     = 1,
    BREWPANEL_BUTTON_STATE_HOVER    = 2,
    BREWPANEL_BUTTON_STATE_CLICKED  = 3,
    BREWPANEL_BUTTON_STATE_DISABLED = 4
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
(mem_data data);

struct BrewPanelButtonStore {
    s8                         button_count;
    BrewPanelButtonImages      images;
    BrewPanelButtonState       states[BREW_PANEL_BUTTONS_MAX];
    BrewPanelButtonState       draw_state[BREW_PANEL_BUTTONS_MAX];
    BrewPanelButtonOffsets     offsets[BREW_PANEL_BUTTONS_MAX];
    func_button_click_callback on_click_callbacks[BREW_PANEL_BUTTONS_MAX];
    mem_data                   on_click_payloads[BREW_PANEL_BUTTONS_MAX];
    s8*                        button_id_matrix;
};



#endif //BREWPANEL_BUTTONS_HPP