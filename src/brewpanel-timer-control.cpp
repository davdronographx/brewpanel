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

    timer_control.buttons.start_button_id = brewpanel_timer_control_create_start_button(button_store,image_store);
    timer_control.buttons.stop_button_id  = brewpanel_timer_control_create_stop_button(button_store,image_store);
    timer_control.buttons.pause_button_id = brewpanel_timer_control_create_pause_button(button_store,image_store);
    timer_control.buttons.reset_button_id = brewpanel_timer_control_create_reset_button(button_store,image_store);



    return(timer_control);
}