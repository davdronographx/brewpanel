#pragma once

#include "brewpanel-clock.hpp"

internal BrewPanelClock 
brewpanel_clock_create() {

    BrewPanelClock clock = {0};

    clock.system_time = brewpanel_platform_system_time_get();

    return(clock);
}

internal void
brewpanel_clock_update(
    BrewPanelClock* clock_state
) {

    clock_state->system_time = brewpanel_platform_system_time_get();
}