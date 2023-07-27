#pragma once

#include "brewpanel-timer-control.hpp"
#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"


internal void
brewpanel_timer_control_mlt_on_start_button_click(
    mem_data payload) {

    BrewPanelTimerControl* mlt_timer = (BrewPanelTimerControl*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_mlt_on_stop_button_click(
    mem_data payload) {

    BrewPanelTimerControl* mlt_timer = (BrewPanelTimerControl*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_mlt_on_pause_button_click(
    mem_data payload) {

    BrewPanelTimerControl* mlt_timer = (BrewPanelTimerControl*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_mlt_on_reset_button_click(
    mem_data payload) {

    BrewPanelTimerControl* mlt_timer = (BrewPanelTimerControl*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_boil_on_start_button_click(
    mem_data payload) {

    BrewPanelTimerControl* boil_timer = (BrewPanelTimerControl*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_boil_on_stop_button_click(
    mem_data payload) {

    BrewPanelTimerControl* boil_timer = (BrewPanelTimerControl*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_boil_on_pause_button_click(
    mem_data payload) {

    BrewPanelTimerControl* boil_timer = (BrewPanelTimerControl*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_boil_on_reset_button_click(
    mem_data payload) {

    BrewPanelTimerControl* boil_timer = (BrewPanelTimerControl*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_create_boil_timer(
    BrewPanelTimerControl* timer_control,
    BrewPanelButtonStore* button_store,
    BrewPanelImagesState* image_store) {

    *timer_control = {0};

    //buttons
    timer_control->buttons.start_button_id = brewpanel_timer_control_create_boil_start_button(button_store,image_store,(*timer_control));
    timer_control->buttons.stop_button_id  = brewpanel_timer_control_create_boil_stop_button(button_store,image_store, (*timer_control));
    timer_control->buttons.pause_button_id = brewpanel_timer_control_create_boil_pause_button(button_store,image_store,(*timer_control));
    timer_control->buttons.reset_button_id = brewpanel_timer_control_create_boil_reset_button(button_store,image_store,(*timer_control));

    //panel
    timer_control->panel.panel_image = BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL;
    timer_control->panel.x_offset    = BREWPANEL_TIMER_PANEL_BOIL_X;
    timer_control->panel.y_offset    = BREWPANEL_TIMER_PANEL_BOIL_Y;

    //set the redraw so we render the panel on the next update
    timer_control->redraw = true;
}

internal void
brewpanel_timer_control_create_mash_lauter_timer(
    BrewPanelTimerControl* timer_control,
    BrewPanelButtonStore*  button_store,
    BrewPanelImagesState*  image_store) {

    *timer_control = {0};

    //buttons
    timer_control->buttons.start_button_id = brewpanel_timer_control_create_mlt_start_button(button_store,image_store,(*timer_control));
    timer_control->buttons.stop_button_id  = brewpanel_timer_control_create_mlt_stop_button( button_store,image_store,(*timer_control));
    timer_control->buttons.pause_button_id = brewpanel_timer_control_create_mlt_pause_button(button_store,image_store,(*timer_control));
    timer_control->buttons.reset_button_id = brewpanel_timer_control_create_mlt_reset_button(button_store,image_store,(*timer_control));

    //panel
    timer_control->panel.panel_image = BREWPANEL_IMAGES_ID_TIMER_PANEL_MLT;
    timer_control->panel.x_offset    = BREWPANEL_TIMER_PANEL_X;
    timer_control->panel.y_offset    = BREWPANEL_TIMER_PANEL_Y;

    //set the redraw so we render the panel on the next update
    timer_control->redraw = true;
}

internal void
brewpanel_timer_control_create_timers(
    BrewPanelTimers*      timers,
    BrewPanelButtonStore* button_store,
    BrewPanelImagesState* image_store) {

    *timers = {0};

    brewpanel_timer_control_create_mash_lauter_timer(
        &timers->mash_lauter_timer,
        button_store,
        image_store
    );

    brewpanel_timer_control_create_boil_timer(
        &timers->boil_timer,
        button_store,
        image_store
    );
}

internal void
brewpanel_timer_control_update(
    BrewPanelTimers*       timers,
    BrewPanelImagesState*  images_state,
    BrewPanelButtonStore*  button_store,
    mem_data               draw_buffer) {

    if (timers->mash_lauter_timer.redraw) {
        brewpanel_images_draw_image(
            images_state,
            timers->mash_lauter_timer.panel.panel_image,
            timers->mash_lauter_timer.panel.x_offset,
            timers->mash_lauter_timer.panel.y_offset,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->mash_lauter_timer.buttons.start_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->mash_lauter_timer.buttons.stop_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->mash_lauter_timer.buttons.pause_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->mash_lauter_timer.buttons.reset_button_id,
            draw_buffer
        );

        timers->mash_lauter_timer.redraw = false;
    }

    if (timers->boil_timer.redraw) {
        brewpanel_images_draw_image(
            images_state,
            timers->boil_timer.panel.panel_image,
            timers->boil_timer.panel.x_offset,
            timers->boil_timer.panel.y_offset,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->boil_timer.buttons.start_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->boil_timer.buttons.stop_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->boil_timer.buttons.pause_button_id,
            draw_buffer
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->boil_timer.buttons.reset_button_id,
            draw_buffer
        );

        timers->boil_timer.redraw = false;
    }
}