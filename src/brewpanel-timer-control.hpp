#ifndef BREWPANEL_TIMER_CONTROL_HPP
#define BREWPANEL_TIMER_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-buttons.hpp"
#include "brewpanel-images.hpp"

struct BrewPanelTimerControlButtons {
    button_id start_button_id;
    button_id stop_button_id;
    button_id pause_button_id;
    button_id reset_button_id;
};

struct BrewPanelTimerControl {
    BrewPanelTimerControlButtons buttons;
    image_id                     panel_image;
};

#endif //BREWPANEL_TIMER_CONTROL_HPP