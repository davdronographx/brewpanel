#pragma once

#include "brewpanel-timer-control.hpp"
#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"


internal void
brewpanel_timer_control_on_start_button_click(
    void* payload) {

}


internal BrewPanelTimerControl
brewpanel_timer_control_create(
    BrewPanelButtonStore* button_store,
    BrewPanelImagesState* image_store
) {

    BrewPanelTimerControl timer_control = {0};

    button_id start_button = 
        brewpanel_buttons_create_button(
            button_store,
            image_store,
            brewpanel_timer_control_on_start_button_click,
            
        );

    return(timer_control);
}