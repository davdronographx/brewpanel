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

    //buttons
    timer_control.buttons.start_button_id = brewpanel_timer_control_create_start_button(button_store,image_store);
    timer_control.buttons.stop_button_id  = brewpanel_timer_control_create_stop_button(button_store,image_store);
    timer_control.buttons.pause_button_id = brewpanel_timer_control_create_pause_button(button_store,image_store);
    timer_control.buttons.reset_button_id = brewpanel_timer_control_create_reset_button(button_store,image_store);

    //panel
    timer_control.panel.panel_image = BREWPANEL_IMAGES_ID_TIMER_PANEL;
    timer_control.panel.x_offset = BREWPANEL_TIMER_PANEL_X;
    timer_control.panel.y_offset = BREWPANEL_TIMER_PANEL_Y;

    //set the redraw so we render the panel on the next update
    timer_control.redraw = true;

    return(timer_control);
}

internal void
brewpanel_timer_control_update(
    BrewPanelTimerControl* timer_control,
    BrewPanelImagesState*  images_state,
    BrewPanelButtonStore*  button_store,
    mem_data               draw_buffer) {

    if (timer_control->redraw) {
        brewpanel_images_draw_image(
            images_state,
            timer_control->panel.panel_image,
            timer_control->panel.x_offset,
            timer_control->panel.y_offset,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timer_control->buttons.start_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timer_control->buttons.stop_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timer_control->buttons.pause_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timer_control->buttons.reset_button_id,
            draw_buffer
        );

        timer_control->redraw = false;
    }
}