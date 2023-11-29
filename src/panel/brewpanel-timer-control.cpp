#pragma once

#include "brewpanel-timer-control.hpp"
#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"
#include "brewpanel-keypad.cpp"

internal void
brewpanel_timer_control_change_timer_state(
    timer_control*      timer,
    timer_state state) {

    timer->previous_state = timer->state;
    timer->state = state;
}

internal void
brewpanel_timer_control_on_start_button_click(
    mem_data payload) {

    timer_control* t = (timer_control*)payload;
}

internal void
brewpanel_timer_control_on_stop_button_click(
    mem_data payload) {

    timer_control* t = (timer_control*)payload;
    brewpanel_timer_control_change_timer_state(t,BREWPANEL_TIMER_STATE_IDLE);
}

internal void
brewpanel_timer_control_on_pause_button_click(
    mem_data payload) {

    timer_control* t = (timer_control*)payload;
}

internal void
brewpanel_timer_control_on_reset_button_click(
    mem_data payload) {

    timer_control* t = (timer_control*)payload;
    brewpanel_timer_control_change_timer_state(t,BREWPANEL_TIMER_STATE_SET);
}

internal void
brewpanel_timer_control_keypad_callback(
    keypad_click_type button_type,
    mem_data          keypad_input_reference,
    mem_data          payload) {

    keypad_input* input = (keypad_input*)keypad_input_reference;
    timer_control* t = (timer_control*)payload;
    t->redraw = true;

    switch (button_type) {

        case BREWPANEL_KEYPAD_BUTTON_TYPE_SET: {
            brewpanel_timer_control_change_timer_state(t,BREWPANEL_TIMER_STATE_PAUSED);
        } break;

        case BREWPANEL_KEYPAD_BUTTON_TYPE_CANCEL: {
            t->set_time_seconds = input->starting_value;
            brewpanel_timer_control_change_timer_state(t,BREWPANEL_TIMER_STATE_IDLE);
        } break;

        default: {

        } break;

    }
}

internal void
brewpanel_timer_control_create(
    BrewPanelTimerControl* timer,
    BrewPanelButtonStore*  buttons,
    BrewPanelImagesStore*  images) {

    *timer = {0};

    u32 hours_tens_x_offset   = BREWPANEL_TIMER_DIGIT_X;
    u32 hours_ones_x_offset   = BREWPANEL_TIMER_DIGIT_X + BREWPANEL_TIMER_DIGIT_WIDTH;
    u32 colon_1_x_offset      = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 2);
    u32 minutes_tens_x_offset = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 3);
    u32 minutes_ones_x_offset = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 4);
    u32 colon_2_x_offset      = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 5);
    u32 seconds_tens_x_offset = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 6);
    u32 seconds_ones_x_offset = BREWPANEL_TIMER_DIGIT_X + (BREWPANEL_TIMER_DIGIT_WIDTH * 7);

    //mash timer
    timer->redraw                   = true;
    timer->state                    = BREWPANEL_TIMER_STATE_IDLE;
    timer->panel_image              = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_TIMER_PANEL_MLT, BREWPANEL_TIMER_PANEL_X, BREWPANEL_TIMER_PANEL_Y);
    timer->digits.hours.tens_face   = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_9,     hours_tens_x_offset,     BREWPANEL_TIMER_DIGIT_Y);
    timer->digits.hours.ones_face   = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_9,     hours_ones_x_offset,     BREWPANEL_TIMER_DIGIT_Y);
    timer->colon_1                  = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_COLON, colon_1_x_offset,        BREWPANEL_TIMER_DIGIT_Y);
    timer->digits.minutes.tens_face = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_9,     minutes_tens_x_offset,   BREWPANEL_TIMER_DIGIT_Y);
    timer->digits.minutes.ones_face = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_9,     minutes_ones_x_offset,   BREWPANEL_TIMER_DIGIT_Y);
    timer->colon_2                  = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_COLON, colon_2_x_offset,        BREWPANEL_TIMER_DIGIT_Y);
    timer->digits.seconds.tens_face = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_9,     seconds_tens_x_offset,   BREWPANEL_TIMER_DIGIT_Y);
    timer->digits.seconds.ones_face = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_9,     seconds_ones_x_offset,   BREWPANEL_TIMER_DIGIT_Y);

    timer->buttons.reset_button_id = 
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_timer_control_on_reset_button_click,
            (mem_data)(timer),
            BREWPANEL_IMAGES_ID_TIMER_RESET_IDLE,
            BREWPANEL_IMAGES_ID_TIMER_RESET_HOVER,
            BREWPANEL_IMAGES_ID_TIMER_RESET_CLICKED,
            BREWPANEL_IMAGES_ID_TIMER_RESET_DISABLED,
            BREWPANEL_TIMER_BUTTON_X,
            BREWPANEL_TIMER_BUTTON_TOP_Y
    );
    
    timer->buttons.stop_button_id  = 
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_timer_control_on_stop_button_click,
            (mem_data)(timer),
            BREWPANEL_IMAGES_ID_TIMER_STOP_IDLE,
            BREWPANEL_IMAGES_ID_TIMER_STOP_HOVER,
            BREWPANEL_IMAGES_ID_TIMER_STOP_CLICKED,
            BREWPANEL_IMAGES_ID_TIMER_STOP_DISABLED, 
            BREWPANEL_TIMER_BUTTON_X,
            BREWPANEL_TIMER_BUTTON_BOTTOM_Y
    );

    // timer->buttons.start_button_id = 
    //     brewpanel_buttons_create_button(
    //         buttons,
    //         images,
    //         brewpanel_timer_control_on_start_button_click,
    //         (mem_data)(&timer),
    //         BREWPANEL_IMAGES_ID_TIMER_START_IDLE,
    //         BREWPANEL_IMAGES_ID_TIMER_START_HOVER,
    //         BREWPANEL_IMAGES_ID_TIMER_START_CLICKED,
    //         BREWPANEL_IMAGES_ID_TIMER_START_DISABLED,
    //         BREWPANEL_TIMER_BUTTON_X,
    //         BREWPANEL_TIMER_BUTTON_TOP_Y
    // );
    
    // timer->buttons.pause_button_id = 
    //     brewpanel_buttons_create_button(
    //         buttons,
    //         images,
    //         brewpanel_timer_control_on_pause_button_click,
    //         (mem_data)(&timer),
    //         BREWPANEL_IMAGES_ID_TIMER_PAUSE_IDLE,
    //         BREWPANEL_IMAGES_ID_TIMER_PAUSE_HOVER,
    //         BREWPANEL_IMAGES_ID_TIMER_PAUSE_CLICKED,
    //         BREWPANEL_IMAGES_ID_TIMER_PAUSE_DISABLED,
    //         BREWPANEL_TIMER_BUTTON_X,
    //         BREWPANEL_TIMER_BUTTON_TOP_Y
    // );

    // brewpanel_buttons_hide(buttons,timer->buttons.start_button_id,images);
    // brewpanel_buttons_hide(buttons,timer->buttons.stop_button_id, images);
    // brewpanel_buttons_hide(buttons,timer->buttons.pause_button_id,images);
    // brewpanel_buttons_hide(buttons,timer->buttons.reset_button_id,images);
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
    total_seconds    -= timestamp.hours * 3600;
    f32 total_minutes = (f32)total_seconds / 60.0f;
    timestamp.minutes = (u32)total_minutes;

    //calculate seconds
    timestamp.seconds = total_seconds - (timestamp.minutes * 60);

    return(timestamp);
}

internal bool
brewpanel_timer_control_update(
    timer_control* timer,
    images_store*  images,
    button_store*  buttons,
    keypad*        keypad,
    mem_data       draw_buffer,
    f64            delta_time) {

    local bool re_enabled = true;

    bool keypad_in_use = keypad->input_reference != NULL && keypad->input_reference != &timer->keypad_input; 

    //first we need to determine if another control is using the keypad
    //if so, we need to disable the timer controls
    if (keypad_in_use) {
        re_enabled = false;
        brewpanel_buttons_disable(buttons,timer->buttons.start_button_id,images);
        brewpanel_buttons_disable(buttons,timer->buttons.stop_button_id, images);
        brewpanel_buttons_disable(buttons,timer->buttons.pause_button_id,images);
        brewpanel_buttons_disable(buttons,timer->buttons.reset_button_id,images);
    }
    else if (!re_enabled &&
             !keypad_in_use &&
            //  brewpanel_buttons_is_disabled(button_store,timer->buttons.start_button_id) && 
             brewpanel_buttons_is_disabled(buttons,timer->buttons.stop_button_id)  &&
             brewpanel_buttons_is_disabled(buttons,timer->buttons.pause_button_id) &&
             brewpanel_buttons_is_disabled(buttons,timer->buttons.reset_button_id)) {
        
        re_enabled = true;
        brewpanel_buttons_enable(buttons,timer->buttons.start_button_id,images);
        brewpanel_buttons_enable(buttons,timer->buttons.stop_button_id, images);
        brewpanel_buttons_enable(buttons,timer->buttons.pause_button_id,images);
        brewpanel_buttons_enable(buttons,timer->buttons.reset_button_id,images);
    }

    //calculate timestamp
    timer_timestamp timestamp = 
        brewpanel_timer_control_calculate_timestamp(
            timer->set_time_seconds,
            timer->elapsed_time_seconds
    ); 
    
    //update the timer
    switch(timer->state) {

        case BREWPANEL_TIMER_STATE_IDLE: {

            brewpanel_buttons_show(buttons,timer->buttons.reset_button_id,images);
            brewpanel_buttons_set_idle(buttons,timer->buttons.reset_button_id);

            brewpanel_buttons_show(buttons,timer->buttons.stop_button_id,images);
            brewpanel_buttons_set_disabled(buttons,timer->buttons.stop_button_id);

            brewpanel_buttons_hide(buttons,timer->buttons.start_button_id,images);
            brewpanel_buttons_hide(buttons,timer->buttons.pause_button_id,images);

            auto state      = buttons->states[timer->buttons.reset_button_id];
            auto visibility = buttons->visibility[timer->buttons.reset_button_id];

            timer->set_time_seconds = 0;
            timer->keypad_input     = {0};

        } break;
        
        case BREWPANEL_TIMER_STATE_SET: {
            
            brewpanel_buttons_set_disabled(buttons,timer->buttons.reset_button_id);
            brewpanel_buttons_set_disabled(buttons,timer->buttons.stop_button_id);

            brewpanel_buttons_hide(buttons,timer->buttons.start_button_id,images);
            brewpanel_buttons_hide(buttons,timer->buttons.pause_button_id,images);

            brewpanel_keypad_active_input(
                keypad,6,timer->set_time_seconds,
                &timer->keypad_input,
                brewpanel_timer_control_keypad_callback,
                (mem_data)timer);

            timer->set_time_seconds  = timer->keypad_input.values[5] * 36000;
            timer->set_time_seconds += timer->keypad_input.values[4] * 3600;
            timer->set_time_seconds += timer->keypad_input.values[3] * 600;
            timer->set_time_seconds += timer->keypad_input.values[2] * 60;
            timer->set_time_seconds += timer->keypad_input.values[1] * 10; 
            timer->set_time_seconds += timer->keypad_input.values[0]; 

        } break;

        case BREWPANEL_TIMER_STATE_PAUSED: {

            brewpanel_buttons_hide(buttons, timer->buttons.reset_button_id,images);
            brewpanel_buttons_hide(buttons, timer->buttons.pause_button_id,images);
            brewpanel_buttons_show(buttons,timer->buttons.start_button_id, images);
            brewpanel_buttons_show(buttons,timer->buttons.stop_button_id,  images);
            brewpanel_buttons_set_idle(buttons, timer->buttons.stop_button_id);

        } break;

        default: {
            timer->state = BREWPANEL_TIMER_STATE_IDLE;
            return(true);

        } break;
    }

    //get the tens and ones digits of the timers
    u8 hours_tens   = (timestamp.hours / 10) % 10;
    u8 hours_ones   = timestamp.hours - (hours_tens * 10);
    u8 minutes_tens = (timestamp.minutes / 10) % 10;
    u8 minutes_ones = timestamp.minutes - (minutes_tens * 10);
    u8 seconds_tens = (timestamp.seconds / 10) % 10;
    u8 seconds_ones = timestamp.seconds - (seconds_tens * 10);

    brewpanel_images_update_instance_image(images,timer->digits.hours.tens_face,brewpanel_timer_glyph_table[hours_tens]);
    brewpanel_images_update_instance_image(images,timer->digits.hours.ones_face,brewpanel_timer_glyph_table[hours_ones]);
    brewpanel_images_update_instance_image(images,timer->digits.minutes.tens_face,brewpanel_timer_glyph_table[minutes_tens]);
    brewpanel_images_update_instance_image(images,timer->digits.minutes.ones_face,brewpanel_timer_glyph_table[minutes_ones]);
    brewpanel_images_update_instance_image(images,timer->digits.seconds.tens_face,brewpanel_timer_glyph_table[seconds_tens]);
    brewpanel_images_update_instance_image(images,timer->digits.seconds.ones_face,brewpanel_timer_glyph_table[seconds_ones]);

    brewpanel_images_draw_image_instance(images,timer->panel_image);
    brewpanel_images_draw_image_instance(images,timer->digits.hours.tens_face);
    brewpanel_images_draw_image_instance(images,timer->digits.hours.ones_face);
    brewpanel_images_draw_image_instance(images,timer->colon_1);
    brewpanel_images_draw_image_instance(images,timer->digits.minutes.tens_face);
    brewpanel_images_draw_image_instance(images,timer->digits.minutes.ones_face);
    brewpanel_images_draw_image_instance(images,timer->colon_2);
    brewpanel_images_draw_image_instance(images,timer->digits.seconds.tens_face);
    brewpanel_images_draw_image_instance(images,timer->digits.seconds.ones_face);
    
    timer->previous_state = timer->state;

    return(true);
}