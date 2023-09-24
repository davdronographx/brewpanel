#ifndef BREWPANEL_MODE_CONTROL_HPP
#define BREWPANEL_MODE_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-buttons.hpp"

#define BREWPANEL_MODE_PANEL_X_OFFSET 299
#define BREWPANEL_MODE_PANEL_Y_OFFSET 25

#define BREWPANEL_MODE_MASH_BUTTON_Y_OFFSET 80
#define BREWPANEL_MODE_BOIL_BUTTON_Y_OFFSET 30
#define BREWPANEL_MODE_BUTTONS_X_OFFSET     302

struct BrewPanelModeControl {
    bool                redraw;
    image_instance_id   panel_id;
    button_id           mash_mode_button;
    button_id           boil_mode_button;
    panel_mode          mode;
};
typedef BrewPanelModeControl mode_control;

#endif //BREWPANEL_MODE_CONTROL_HPP