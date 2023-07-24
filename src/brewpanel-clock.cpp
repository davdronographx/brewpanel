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

    if (clock_state->system_time.hours > 12) {
        clock_state -= 12;
        clock_state->am_pm_face = BREWPANEL_IMAGES_ID_CLOCK_PM;
    } 
    else {
        clock_state->am_pm_face = BREWPANEL_IMAGES_ID_CLOCK_AM;
    }

    u8 hours_tens = (clock_state->system_time.hours / 10) % 10;
    u8 hours_ones = clock_state->system_time.hours - (hours_tens * 10);

    u8 minutes_tens = (clock_state->system_time.minutes / 10) % 10;
    u8 minutes_ones = clock_state->system_time.minutes - (minutes_tens * 10);

    u8 seconds_tens = (clock_state->system_time.seconds / 10) % 10;
    u8 seconds_ones = clock_state->system_time.seconds - (seconds_tens * 10);

    clock_state->hours_faces.tens_face = brewpanel_clock_glyph_table[hours_tens];
    clock_state->hours_faces.ones_face = brewpanel_clock_glyph_table[hours_ones];

    clock_state->minutes_faces.tens_face = brewpanel_clock_glyph_table[minutes_tens];
    clock_state->minutes_faces.ones_face = brewpanel_clock_glyph_table[minutes_ones];

    clock_state->seconds_faces.tens_face = brewpanel_clock_glyph_table[seconds_tens];
    clock_state->seconds_faces.ones_face = brewpanel_clock_glyph_table[seconds_ones];

    brewpanel_nop();
}