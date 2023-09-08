#ifndef BREWPANEL_CONTROL_TEMPERATURE_HPP
#define BREWPANEL_CONTROL_TEMPERATURE_HPP

#include "brewpanel-control-types.hpp"

struct BrewpanelControlTemperatureState {
    u8 hlt_temp;
    u8 mlt_temp;
    u8 boil_temp;
    u8 mlt_set_temp;
    u8 boil_set_percent;
};

typedef BrewpanelControlTemperatureState temp_state;

#endif //BREWPANEL_CONTROL_TEMPERATURE_HPP