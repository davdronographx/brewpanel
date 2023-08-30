#pragma once

#include "brewpanel-timer-control.hpp"
#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"
#include "brewpanel-keypad.cpp"

internal void
brewpanel_timer_control_change_timer_state(
    timer*      timer,
    timer_state state) {

    timer->previous_state = timer->state;
    timer->state = state;
}

internal void
brewpanel_timer_control_on_start_button_click(
    mem_data payload) {

    timer* t = (timer*)payload;


    brewpanel_nop();
}

internal void
brewpanel_timer_control_on_stop_button_click(
    mem_data payload) {

    timer* t = (timer*)payload;
    brewpanel_timer_control_change_timer_state(t,BREWPANEL_TIMER_STATE_IDLE);
}

internal void
brewpanel_timer_control_on_pause_button_click(
    mem_data payload) {

    timer* t = (timer*)payload;

    brewpanel_nop();
}

internal void
brewpanel_timer_control_on_reset_button_click(
    mem_data payload) {

    timer* t = (timer*)payload;
    brewpanel_timer_control_change_timer_state(t,BREWPANEL_TIMER_STATE_SET);
}

internal void
brewpanel_timer_control_keypad_callback(
    keypad_click_type button_type,
    mem_data          keypad_input_reference,
    mem_data          payload) {

    keypad_input* input = (keypad_input*)keypad_input_reference;
    timer* t = (timer*)payload;
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
    BrewPanelTimerControl* timer_control,
    BrewPanelButtonStore*  button_store,
    BrewPanelImagesStore*  image_store) {

    *timer_control = {0};
    timer_control->off_timer_panel = 
        brewpanel_images_create_image_instance(
            image_store,
            BREWPANEL_IMAGES_ID_TIMER_PANEL_OFF,
            BREWPANEL_TIMER_PANEL_X, 
            BREWPANEL_TIMER_PANEL_Y
    );

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
    timer_control->mash_timer.buttons.reset_button_id  = brewpanel_timer_control_create_reset_button(button_store,image_store,timer_control->mash_timer);
    timer_control->mash_timer.buttons.stop_button_id   = brewpanel_timer_control_create_stop_button(button_store,image_store,timer_control->mash_timer);
    // timer_control->buttons.start_button_id = brewpanel_timer_control_create_mlt_start_button(button_store,image_store,(*timer_control));
    // timer_control->buttons.pause_button_id = brewpanel_timer_control_create_mlt_pause_button(button_store,image_store,(*timer_control));

    //boil timer
    timer_control->boil_timer.redraw                   = true;
    timer_control->boil_timer.state                    = BREWPANEL_TIMER_STATE_IDLE;
    timer_control->boil_timer.panel_image              = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL, BREWPANEL_TIMER_PANEL_X, BREWPANEL_TIMER_PANEL_Y);
    timer_control->boil_timer.digits.hours.tens_face   = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hours_tens_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.hours.ones_face   = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hours_ones_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.colon_1                  = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,colon_1_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.minutes.tens_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,minutes_tens_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.minutes.ones_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,minutes_ones_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.colon_2                  = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,colon_2_x_offset,  BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.seconds.tens_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,seconds_tens_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.digits.seconds.ones_face = brewpanel_images_create_image_instance(image_store,BREWPANEL_IMAGES_ID_RED_DIGIT_0,seconds_ones_x_offset,BREWPANEL_TIMER_DIGIT_Y);
    timer_control->boil_timer.buttons.reset_button_id  = brewpanel_timer_control_create_reset_button(button_store,image_store,timer_control->boil_timer);
    timer_control->boil_timer.buttons.stop_button_id   = brewpanel_timer_control_create_stop_button(button_store,image_store,timer_control->mash_timer);
    
    // timer_control->buttons.start_button_id = brewpanel_timer_control_create_boil_start_button(button_store,image_store,(*timer_control));
    // timer_control->buttons.pause_button_id = brewpanel_timer_control_create_boil_pause_button(button_store,image_store,(*timer_control));
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
brewpanel_timer_control_update_and_render(
    bool*           redraw,
    timer*          timer,
    timer_timestamp timestamp,
    images_store*   images_state,
    button_store*   buttons,
    keypad*         keypad,
    panel_mode      mode) {

    if (*redraw || timer->redraw) {

        switch(timer->state) {

            case BREWPANEL_TIMER_STATE_IDLE: {
                brewpanel_buttons_set_idle(buttons,timer->buttons.reset_button_id);
                brewpanel_buttons_set_disabled(buttons,timer->buttons.stop_button_id);

                image_id panel_image = 
                    (mode == BREWPANEL_MODE_BOIL) 
                    ? BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL 
                    : BREWPANEL_IMAGES_ID_TIMER_PANEL_MLT;

                images_state->image_instances[timer->panel_image].image_id = panel_image;

                timer->set_time_seconds = 0;

                timer->keypad_input = {0};

                

            } break;
            
            case BREWPANEL_TIMER_STATE_SET: {
                
                brewpanel_buttons_set_disabled(buttons,timer->buttons.reset_button_id);
                brewpanel_buttons_set_disabled(buttons,timer->buttons.stop_button_id);

                image_id panel_image = 
                    (mode == BREWPANEL_MODE_BOIL) 
                        ? BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL_INPUT 
                        : BREWPANEL_IMAGES_ID_TIMER_PANEL_MASH_INPUT;

                images_state->image_instances[timer->panel_image].image_id = panel_image; 

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

                image_id panel_image = 
                    (mode == BREWPANEL_MODE_BOIL) 
                        ? BREWPANEL_IMAGES_ID_TIMER_PANEL_BOIL 
                        : BREWPANEL_IMAGES_ID_TIMER_PANEL_MLT;

                images_state->image_instances[timer->panel_image].image_id = panel_image; 

                brewpanel_buttons_hide(buttons, timer->buttons.reset_button_id,images_state);
                
                brewpanel_buttons_set_idle(buttons, timer->buttons.stop_button_id);


            } break;

            default: {
                timer->state = BREWPANEL_TIMER_STATE_IDLE;
                brewpanel_buttons_set_idle(buttons,timer->buttons.reset_button_id);
                brewpanel_buttons_set_disabled(buttons,timer->buttons.stop_button_id);
            } break;
        }
    }

    //get the tens and ones digits of the timers
    u8 hours_tens   = (timestamp.hours / 10) % 10;
    u8 hours_ones   = timestamp.hours - (hours_tens * 10);
    u8 minutes_tens = (timestamp.minutes / 10) % 10;
    u8 minutes_ones = timestamp.minutes - (minutes_tens * 10);
    u8 seconds_tens = (timestamp.seconds / 10) % 10;
    u8 seconds_ones = timestamp.seconds - (seconds_tens * 10);

    image_instance* hours_tens_instance   = &images_state->image_instances[timer->digits.hours.tens_face];        
    image_instance* hours_ones_instance   = &images_state->image_instances[timer->digits.hours.ones_face];    
    image_instance* minutes_tens_instance = &images_state->image_instances[timer->digits.minutes.tens_face];    
    image_instance* minutes_ones_instance = &images_state->image_instances[timer->digits.minutes.ones_face];    
    image_instance* seconds_tens_instance = &images_state->image_instances[timer->digits.seconds.tens_face];    
    image_instance* seconds_ones_instance = &images_state->image_instances[timer->digits.seconds.ones_face];    

    image_id  previous_hours_tens_image   = hours_tens_instance->image_id;
    image_id  previous_hours_ones_image   = hours_ones_instance->image_id;
    image_id  previous_minutes_tens_image = minutes_tens_instance->image_id;
    image_id  previous_minutes_ones_image = minutes_ones_instance->image_id;
    image_id  previous_seconds_tens_image = seconds_tens_instance->image_id;
    image_id  previous_seconds_ones_image = seconds_ones_instance->image_id;

    hours_tens_instance->image_id   = brewpanel_timer_glyph_table[hours_tens];
    hours_ones_instance->image_id   = brewpanel_timer_glyph_table[hours_ones];
    minutes_tens_instance->image_id = brewpanel_timer_glyph_table[minutes_tens];
    minutes_ones_instance->image_id = brewpanel_timer_glyph_table[minutes_ones];
    seconds_tens_instance->image_id = brewpanel_timer_glyph_table[seconds_tens];
    seconds_ones_instance->image_id = brewpanel_timer_glyph_table[seconds_ones];

    *redraw |= (
        previous_hours_tens_image   != hours_tens_instance->image_id   ||
        previous_hours_ones_image   != hours_ones_instance->image_id   ||
        previous_minutes_tens_image != minutes_tens_instance->image_id ||
        previous_minutes_ones_image != minutes_ones_instance->image_id ||
        previous_seconds_tens_image != seconds_tens_instance->image_id ||
        previous_seconds_ones_image != seconds_ones_instance->image_id 
    );

    if (*redraw) {
        brewpanel_images_draw_image_instance(images_state,timer->panel_image);
        brewpanel_images_draw_image_instance(images_state,timer->digits.hours.tens_face);
        brewpanel_images_draw_image_instance(images_state,timer->digits.hours.ones_face);
        brewpanel_images_draw_image_instance(images_state,timer->colon_1);
        brewpanel_images_draw_image_instance(images_state,timer->digits.minutes.tens_face);
        brewpanel_images_draw_image_instance(images_state,timer->digits.minutes.ones_face);
        brewpanel_images_draw_image_instance(images_state,timer->colon_2);
        brewpanel_images_draw_image_instance(images_state,timer->digits.seconds.tens_face);
        brewpanel_images_draw_image_instance(images_state,timer->digits.seconds.ones_face);
    }

    return(redraw);
}

internal void
brewpanel_timer_control_hide_timer(
    timer*        timer,
    button_store* buttons,
    images_store* images) {

    brewpanel_buttons_hide(buttons,timer->buttons.reset_button_id,images);
    brewpanel_buttons_hide(buttons,timer->buttons.stop_button_id,images);
}

internal void
brewpanel_timer_control_show_timer(
    timer*        timer,
    button_store* buttons,
    images_store* images) {

    brewpanel_buttons_show(buttons,timer->buttons.reset_button_id,images);
    brewpanel_buttons_show(buttons,timer->buttons.stop_button_id,images);
}

internal bool
brewpanel_timer_control_update(
    timer_control* timer_control,
    images_store*  images_state,
    button_store*  button_store,
    keypad*        keypad,
    panel_mode     mode,
    mem_data       draw_buffer) {


    //if the panel is off, draw the off panel and we're done
    if (mode == BREWPANEL_MODE_OFF){
        
        brewpanel_images_draw_image_instance(
            images_state,
            timer_control->off_timer_panel
        );

        brewpanel_timer_control_hide_timer(&timer_control->mash_timer,button_store,images_state);
        brewpanel_timer_control_hide_timer(&timer_control->boil_timer,button_store,images_state);
        
        return(true);
    }

    //get the timer to update
    timer* timer = (mode == BREWPANEL_MODE_BOIL) ? &timer_control->boil_timer : &timer_control->mash_timer;

    local panel_mode previous_mode = BREWPANEL_MODE_OFF;
    local bool re_enabled = false;
    
    bool redraw        = previous_mode != mode || timer->previous_state != timer->state;
    bool keypad_in_use = keypad->input_reference != NULL && keypad->input_reference != &timer->keypad_input; 

    //first we need to determine if another control is using the keypad
    //if so, we need to disable the timer controls
    if (keypad_in_use) {
        re_enabled = false;
        brewpanel_buttons_disable(button_store,timer->buttons.start_button_id,images_state);
        brewpanel_buttons_disable(button_store,timer->buttons.stop_button_id, images_state);
        brewpanel_buttons_disable(button_store,timer->buttons.pause_button_id,images_state);
        brewpanel_buttons_disable(button_store,timer->buttons.reset_button_id,images_state);
    }
    else if (!re_enabled &&
             !keypad_in_use &&
             brewpanel_buttons_is_disabled(button_store,timer->buttons.start_button_id) && 
             brewpanel_buttons_is_disabled(button_store,timer->buttons.stop_button_id)  &&
             brewpanel_buttons_is_disabled(button_store,timer->buttons.pause_button_id) &&
             brewpanel_buttons_is_disabled(button_store,timer->buttons.reset_button_id)) {
        
        re_enabled = true;
        brewpanel_buttons_enable(button_store,timer->buttons.start_button_id,images_state);
        brewpanel_buttons_enable(button_store,timer->buttons.stop_button_id, images_state);
        brewpanel_buttons_enable(button_store,timer->buttons.pause_button_id,images_state);
        brewpanel_buttons_enable(button_store,timer->buttons.reset_button_id,images_state);
    }

    if (redraw || re_enabled || previous_mode != mode) {

        re_enabled = false;

        switch(mode) {
            
            case BREWPANEL_MODE_MASH: {
                brewpanel_timer_control_hide_timer(&timer_control->boil_timer,button_store,images_state);
                brewpanel_timer_control_show_timer(&timer_control->mash_timer,button_store,images_state);
            } break;
            
            case BREWPANEL_MODE_BOIL: {
                brewpanel_timer_control_hide_timer(&timer_control->mash_timer,button_store,images_state);
                brewpanel_timer_control_show_timer(&timer_control->boil_timer,button_store,images_state);
            } break;
        }
    }

    //calculate timestamp
    timer_timestamp timestamp = 
        brewpanel_timer_control_calculate_timestamp(
            timer->set_time_seconds,
            timer->elapsed_time_seconds
    ); 
    
    //update the timer
    redraw |= brewpanel_timer_control_update_and_render(
        &redraw,
        timer,
        timestamp,
        images_state,
        button_store,
        keypad,
        mode
    );
    
    previous_mode = mode;
    timer->previous_state = timer->state;
    timer->redraw = false;

    return(redraw);
}