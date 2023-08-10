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
brewpanel_timer_control_create(
    BrewPanelTimerControl* timer_control,
    BrewPanelButtonStore*  button_store,
    BrewPanelImagesStore*  image_store) {

    *timer_control = {0};

    u32 hours_tens_x_offset   = BREWPANEL_TIMER_DIGIT_X;
    u32 hours_ones_x_offset   = BREWPANEL_TIMER_DIGIT_X + BREWPANEL_TIMER_DIGIT_WIDTH;
    u32 colon_1_x_offset      = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 2);
    u32 minutes_tens_x_offset = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 3);
    u32 minutes_ones_x_offset = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 4);
    u32 colon_2_x_offset      = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 5);
    u32 seconds_tens_x_offset = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 6);
    u32 seconds_ones_x_offset = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 7);

    //mash timer
    timer_control->mash_timer.redraw                   = true;
    timer_control->mash_timer.state                    = BREWPANEL_TIMER_STATE_IDLE;
    timer_control->mash_timer.panel_image              = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_TIMER_PANEL_MLT, BREWPANEL_TIMER_PANEL_X, BREWPANEL_TIMER_PANEL_Y);
    timer_control->mash_timer.digits.hours.tens_face   = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_9,hours_tens_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->mash_timer.digits.hours.ones_face   = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_9,hours_ones_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->mash_timer.colon_1                  = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,colon_1_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->mash_timer.digits.minutes.tens_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_9,minutes_tens_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->mash_timer.digits.minutes.ones_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_9,minutes_ones_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->mash_timer.colon_2                  = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,colon_2_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->mash_timer.digits.seconds.tens_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_9,seconds_tens_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->mash_timer.digits.seconds.ones_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_9,seconds_ones_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    // timer_control->buttons.start_button_id = brewpanel_timer_control_create_mlt_start_button(button_store,image_store,(*timer_control));
    // timer_control->buttons.stop_button_id  = brewpanel_timer_control_create_mlt_stop_button( button_store,image_store,(*timer_control));
    // timer_control->buttons.pause_button_id = brewpanel_timer_control_create_mlt_pause_button(button_store,image_store,(*timer_control));
    // timer_control->buttons.reset_button_id = brewpanel_timer_control_create_mlt_reset_button(button_store,image_store,(*timer_control));

    //boil timer
    timer_control->boil_timer.redraw                   = true;
    timer_control->boil_timer.state                    = BREWPANEL_TIMER_STATE_IDLE;
    timer_control->boil_timer.panel_image              = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL, BREWPANEL_TIMER_PANEL_X, BREWPANEL_TIMER_PANEL_Y);
    timer_control->boil_timer.panel_image              = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_TIMER_PANEL_MLT, BREWPANEL_TIMER_PANEL_X, BREWPANEL_TIMER_PANEL_Y);
    timer_control->boil_timer.digits.hours.tens_face   = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hours_tens_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.hours.ones_face   = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hours_ones_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.colon_1                  = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,colon_1_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.minutes.tens_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,minutes_tens_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.minutes.ones_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,minutes_ones_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.colon_2                  = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,colon_2_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.seconds.tens_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,seconds_tens_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.seconds.ones_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,seconds_ones_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    // timer_control->buttons.start_button_id = brewpanel_timer_control_create_mlt_start_button(button_store,image_store,(*timer_control));
    // timer_control->buttons.start_button_id = brewpanel_timer_control_create_boil_start_button(button_store,image_store,(*timer_control));
    // timer_control->buttons.stop_button_id  = brewpanel_timer_control_create_boil_stop_button(button_store,image_store, (*timer_control));
    // timer_control->buttons.pause_button_id = brewpanel_timer_control_create_boil_pause_button(button_store,image_store,(*timer_control));
    // timer_control->buttons.reset_button_id = brewpanel_timer_control_create_boil_reset_button(button_store,image_store,(*timer_control));
}

internal BrewPanelTimerTimestamp
brewpanel_timer_control_calculate_timestamp(
    u32 set_seconds,
    u32 elapsed_seconds) {

    BrewPanelTimerTimestamp timestamp = {0};
    u32 total_seconds = set_seconds - elapsed_seconds;

    //calculate hours
    f32 total_hours = (f32)total_seconds / 3600.0f;
    timestamp.hours = (u32)total_hours;

    //calculate minutes
    f32 total_minutes = (total_hours - timestamp.hours) * 60.0f;
    timestamp.minutes = (u32)total_minutes;

    //calculate seconds
    timestamp.seconds = (u32)((total_minutes - timestamp.minutes) * 60.0f);

    return(timestamp);
}

internal bool
brewpanel_timer_control_draw_timers(
    timer*         timer,
    images_store*  images_state,
    button_store*  button_store,
    mem_data       draw_buffer) {

    bool redraw = false;

    if (timer->redraw) {
        brewpanel_images_draw_image_instance(images_state, timer->panel_image);
        // brewpanel_buttons_draw_button(button_store, images_state, timer_control->mash_timer.buttons.start_button_id);
        // brewpanel_buttons_draw_button(button_store, images_state, timer_control->mash_timer.buttons.stop_button_id);
        // brewpanel_buttons_draw_button(button_store, images_state, timer_control->mash_timer.buttons.pause_button_id);
        // brewpanel_buttons_draw_button(button_store, images_state, timer_control->mash_timer.buttons.reset_button_id);
        timer->redraw = false;
        redraw = true;
    }

    return(redraw);
}

internal bool
brewpanel_timer_control_calculate_and_draw_digits(
    timer*          timer,
    timer_timestamp timestamp,
    images_store*   images_state) {

    bool redraw = false;

    local BrewPanelTimerDigits previous_digits  = timer->digits;

    //get the tens and ones digits of the timers
    u8 hours_tens   = (timestamp.hours / 10) % 10;
    u8 hours_ones   = timestamp.hours - (hours_tens * 10);
    u8 minutes_tens = (timestamp.minutes / 10) % 10;
    u8 minutes_ones = timestamp.minutes - (minutes_tens * 10);
    u8 seconds_tens = (timestamp.seconds / 10) % 10;
    u8 seconds_ones = timestamp.seconds - (seconds_tens * 10);
    
    //get the timer images
    images_state->image_instances[timer->digits.hours.tens_face].image_id   = brewpanel_timer_glyph_table[hours_tens];
    images_state->image_instances[timer->digits.hours.ones_face].image_id   = brewpanel_timer_glyph_table[hours_ones];
    images_state->image_instances[timer->digits.minutes.tens_face].image_id = brewpanel_timer_glyph_table[minutes_tens];
    images_state->image_instances[timer->digits.minutes.ones_face].image_id = brewpanel_timer_glyph_table[minutes_ones];
    images_state->image_instances[timer->digits.seconds.tens_face].image_id = brewpanel_timer_glyph_table[seconds_tens];
    images_state->image_instances[timer->digits.seconds.ones_face].image_id = brewpanel_timer_glyph_table[seconds_ones];

    //draw the hours
    if (previous_digits.hours.tens_face != timer->digits.hours.tens_face) {
        brewpanel_images_draw_image_instance(images_state,timer->digits.hours.tens_face);
        redraw = true;
    }
    if (previous_digits.hours.ones_face != timer->digits.hours.ones_face) {
        brewpanel_images_draw_image_instance(images_state,timer->digits.hours.ones_face);
        redraw = true;
    }

    //semicolon
    brewpanel_images_draw_image_instance(images_state,timer->colon_1);

    //draw the minutes
    if (previous_digits.minutes.tens_face != timer->digits.minutes.tens_face) {
        brewpanel_images_draw_image_instance(images_state,timer->digits.minutes.tens_face);
        redraw = true;
    }
    if (previous_digits.minutes.ones_face != timer->digits.minutes.ones_face) {
        brewpanel_images_draw_image_instance(images_state,timer->digits.minutes.ones_face);
        redraw = true;
    }

    //semicolon
    brewpanel_images_draw_image_instance(images_state,timer->colon_2);

    //draw the seconds
    if (previous_digits.seconds.tens_face != timer->digits.seconds.tens_face) {
        brewpanel_images_draw_image_instance(images_state,timer->digits.seconds.tens_face);
        redraw = true;
    }
    if (previous_digits.seconds.ones_face != timer->digits.seconds.ones_face) {
        brewpanel_images_draw_image_instance(images_state,timer->digits.seconds.ones_face);
        redraw = true;
    }

    return(redraw);
}


internal void
brewpanel_timer_control_update_buttons(
    BrewPanelTimer*        timer,
    BrewPanelButtonStore*  button_store,
    images_store*          images) {

    switch (timer->state) {

        case BREWPANEL_TIMER_STATE_IDLE: {

            // brewpanel_buttons_disable(button_store,timer->buttons.start_button_id,images);
            // brewpanel_buttons_disable(button_store,timer->buttons.stop_button_id,images);
            // brewpanel_buttons_disable(button_store,timer->buttons.pause_button_id,images);
            // brewpanel_buttons_enable(button_store, timer->buttons.reset_button_id,images);

        } break;

        case BREWPANEL_TIMER_STATE_SET: {

        } break;

        case BREWPANEL_TIMER_STATE_RUNNING: {

        } break;

        case BREWPANEL_TIMER_STATE_PAUSED: {

        } break;

        case BREWPANEL_TIMER_STATE_EXPIRED: {

        } break;

        default: {

        } break;
    }

}

internal bool
brewpanel_timer_control_update(
    timer_control* timer_control,
    images_store*  images_state,
    button_store*  button_store,
    panel_mode     mode,
    mem_data       draw_buffer) {

    local panel_mode previous_mode = BREWPANEL_MODE_NULL;
    bool redraw = previous_mode != mode;

    //get the timer to update
    timer* timer = (mode == BREWPANEL_MODE_BOIL) ? &timer_control->boil_timer : &timer_control->mash_timer;
    timer->redraw = redraw;

    //update the timer buttons
    // brewpanel_timer_control_update_buttons(&timer_control->mash_timer,button_store,images_state);
    // brewpanel_timer_control_update_buttons(&timer_control->boil_timer,button_store,images_state);

    //calculate timestamp
    timer_timestamp timestamp = 
        brewpanel_timer_control_calculate_timestamp(
            timer->set_time_seconds,
            timer->elapsed_time_seconds
    ); 

    //draw the timers
    redraw |= brewpanel_timer_control_draw_timers(
        timer,
        images_state,
        button_store,
        draw_buffer
    );
    
    //get the timer images
    redraw |= brewpanel_timer_control_calculate_and_draw_digits(
        timer,
        timestamp,
        images_state
    );
    
    return(redraw);
}