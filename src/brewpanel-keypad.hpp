#ifndef BREWPANEL_KEYPAD_HPP
#define BREWPANEL_KEYPAD_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-buttons.hpp"

#define BREWPANEL_KEYPAD_OFFSET_X 740
#define BREWPANEL_KEYPAD_OFFSET_Y 30

struct BrewPanelKeypad {
    bool              redraw;
    image_instance_id panel_id;
};
typedef BrewPanelKeypad keypad;

#endif //BREWPANEL_KEYPAD_HPP