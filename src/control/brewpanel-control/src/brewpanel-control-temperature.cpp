#pragma once

#include "brewpanel-control-temperature.hpp"

internal BrewpanelControlTemperatureState
brewpanel_control_temperature_init() {

    BrewpanelControlTemperatureState temp_state = {0};

    return(temp_state);
}

internal void
brewpanel_control_temperature_update(
    BrewpanelControlTemperatureState* temp_state) {

    //TODO: this is temporary to populate the heartbeat message
    temp_state->hlt_temp = 101;
    temp_state->mlt_temp = 210;
    temp_state->boil_temp = 156;

}   
