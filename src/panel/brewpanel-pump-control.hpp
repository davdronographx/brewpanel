#ifndef BREWPANEL_PUMP_CONTROL_HPP
#define BREWPANEL_PUMP_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-buttons.hpp"

#define BREWPANEL_PUMPS_PANEL_X_OFFSET 527
#define BREWPANEL_PUMPS_PANEL_Y_OFFSET 25

struct BrewPanelPumpControl {
    bool              redraw;
    image_instance_id panel_id;
    button_id         water_on_button;
    button_id         water_off_button;
    button_id         wort_on_button;
    button_id         wort_off_button;
};

typedef BrewPanelPumpControl pump_control;

#endif //BREWPANEL_PUMP_CONTROL_HPP