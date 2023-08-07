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
    BrewPanelImagesStore* image_store) {

    *timer_control = {0};
    timer_control->state = BREWPANEL_TIMER_STATE_IDLE;

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
    BrewPanelImagesStore*  image_store) {

    *timer_control = {0};
    timer_control->state = BREWPANEL_TIMER_STATE_IDLE;

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
    BrewPanelImagesStore* image_store) {

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
    BrewPanelTimers*       timers,
    BrewPanelImagesStore*  images_state,
    BrewPanelButtonStore*  button_store,
    mem_data               draw_buffer) {

    bool redraw = false;

    if (timers->mash_lauter_timer.redraw) {
        brewpanel_images_draw_image_immediate(
            images_state,
            timers->mash_lauter_timer.panel.panel_image,
            timers->mash_lauter_timer.panel.x_offset,
            timers->mash_lauter_timer.panel.y_offset
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->mash_lauter_timer.buttons.start_button_id
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->mash_lauter_timer.buttons.stop_button_id
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->mash_lauter_timer.buttons.pause_button_id
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->mash_lauter_timer.buttons.reset_button_id
        );

        timers->mash_lauter_timer.redraw = false;
        redraw = true;
    }

    if (timers->boil_timer.redraw) {
        brewpanel_images_draw_image_immediate(
            images_state,
            timers->boil_timer.panel.panel_image,
            timers->boil_timer.panel.x_offset,
            timers->boil_timer.panel.y_offset
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->boil_timer.buttons.start_button_id
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->boil_timer.buttons.stop_button_id
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->boil_timer.buttons.pause_button_id
        );

        brewpanel_buttons_draw_button(
            button_store,
            images_state,
            timers->boil_timer.buttons.reset_button_id
        );

        timers->boil_timer.redraw = false;
        redraw = true;
    }

    return(redraw);
}

internal bool
brewpanel_timer_control_calculate_and_draw_digits(
    BrewPanelTimers*        timers,
    BrewPanelTimerTimestamp mlt_timestamp,
    BrewPanelTimerTimestamp boil_timestamp,
    BrewPanelImagesStore*   images_state) {

    bool redraw = false;

    local BrewPanelTimerDigits previous_mlt_digits  = timers->mash_lauter_timer.digits;
    local BrewPanelTimerDigits previous_boil_digits = timers->boil_timer.digits;

    //get the tens and ones digits of the timers
    u8 mlt_hours_tens   = (mlt_timestamp.hours / 10) % 10;
    u8 mlt_hours_ones   = mlt_timestamp.hours - (mlt_hours_tens * 10);
    u8 mlt_minutes_tens = (mlt_timestamp.minutes / 10) % 10;
    u8 mlt_minutes_ones = mlt_timestamp.minutes - (mlt_minutes_tens * 10);
    u8 mlt_seconds_tens = (mlt_timestamp.seconds / 10) % 10;
    u8 mlt_seconds_ones = mlt_timestamp.seconds - (mlt_seconds_tens * 10);
    
    u8 boil_hours_tens   = (boil_timestamp.hours / 10) % 10;
    u8 boil_hours_ones   = boil_timestamp.hours - (boil_hours_tens * 10);
    u8 boil_minutes_tens = (boil_timestamp.minutes / 10) % 10;
    u8 boil_minutes_ones = boil_timestamp.minutes - (boil_minutes_tens * 10);
    u8 boil_seconds_tens = (boil_timestamp.seconds / 10) % 10;
    u8 boil_seconds_ones = boil_timestamp.seconds - (boil_seconds_tens * 10);
    
    //get the timer images
    timers->mash_lauter_timer.digits.hours.tens_face   = brewpanel_timer_glyph_table[mlt_hours_tens];
    timers->mash_lauter_timer.digits.hours.ones_face   = brewpanel_timer_glyph_table[mlt_hours_ones];
    timers->mash_lauter_timer.digits.minutes.tens_face = brewpanel_timer_glyph_table[mlt_minutes_tens];
    timers->mash_lauter_timer.digits.minutes.ones_face = brewpanel_timer_glyph_table[mlt_minutes_ones];
    timers->mash_lauter_timer.digits.seconds.tens_face = brewpanel_timer_glyph_table[mlt_seconds_tens];
    timers->mash_lauter_timer.digits.seconds.ones_face = brewpanel_timer_glyph_table[mlt_seconds_ones];

    timers->boil_timer.digits.hours.tens_face   = brewpanel_timer_glyph_table[boil_hours_tens];
    timers->boil_timer.digits.hours.ones_face   = brewpanel_timer_glyph_table[boil_hours_ones];
    timers->boil_timer.digits.minutes.tens_face = brewpanel_timer_glyph_table[boil_minutes_tens];
    timers->boil_timer.digits.minutes.ones_face = brewpanel_timer_glyph_table[boil_minutes_ones];
    timers->boil_timer.digits.seconds.tens_face = brewpanel_timer_glyph_table[boil_seconds_tens];
    timers->boil_timer.digits.seconds.ones_face = brewpanel_timer_glyph_table[boil_seconds_ones];

    //-----------------------------
    // MLT TIMER
    //-----------------------------

    u32 mlt_digits_offset = BREWPANEL_TIMER_CONTROL_MLT_DIGITS_OFFSET_X;
    BrewPanelImagesFileIndex digit_image_info = brewpanel_images_index(images_state,timers->mash_lauter_timer.digits.hours.tens_face);
    u32 y_offset = BREW_PANEL_HEIGHT_PIXELS - (digit_image_info.image_height_pixels + 147);

    //draw the hours
    // if (previous_mlt_digits.hours.tens_face != timers->mash_lauter_timer.digits.hours.tens_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->mash_lauter_timer.digits.hours.tens_face,
    //         mlt_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // mlt_digits_offset += digit_image_info.image_width_pixels;

    // if (previous_mlt_digits.hours.ones_face != timers->mash_lauter_timer.digits.hours.ones_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->mash_lauter_timer.digits.hours.ones_face,
    //         mlt_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // mlt_digits_offset += digit_image_info.image_width_pixels;

    // //semicolon
    // brewpanel_images_draw_image_immediate(
    //     images_state,
    //     BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,
    //     mlt_digits_offset,
    //     y_offset,
    //     draw_buffer
    // );
    // mlt_digits_offset += digit_image_info.image_width_pixels;

    // //draw the minutes
    // if (previous_mlt_digits.minutes.tens_face != timers->mash_lauter_timer.digits.minutes.tens_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->mash_lauter_timer.digits.minutes.tens_face,
    //         mlt_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // mlt_digits_offset += digit_image_info.image_width_pixels;

    // if (previous_mlt_digits.minutes.ones_face != timers->mash_lauter_timer.digits.minutes.ones_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->mash_lauter_timer.digits.minutes.ones_face,
    //         mlt_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // mlt_digits_offset += digit_image_info.image_width_pixels;

    // //semicolon
    // brewpanel_images_draw_image_immediate(
    //     images_state,
    //     BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,
    //     mlt_digits_offset,
    //     y_offset,
    //     draw_buffer
    // );
    // mlt_digits_offset += digit_image_info.image_width_pixels;

    // //draw the seconds
    // if (previous_mlt_digits.seconds.tens_face != timers->mash_lauter_timer.digits.seconds.tens_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->mash_lauter_timer.digits.seconds.tens_face,
    //         mlt_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // mlt_digits_offset += digit_image_info.image_width_pixels;

    // if (previous_mlt_digits.seconds.ones_face != timers->mash_lauter_timer.digits.seconds.ones_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->mash_lauter_timer.digits.seconds.ones_face,
    //         mlt_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    
    // //-----------------------------
    // // BOIL TIMER
    // //-----------------------------

    // u32 boil_digits_offset = BREWPANEL_TIMER_CONTROL_BOIL_DIGITS_OFFSET_X;

    // //draw the hours
    // if (previous_boil_digits.hours.tens_face != timers->boil_timer.digits.hours.tens_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->boil_timer.digits.hours.tens_face,
    //         boil_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // boil_digits_offset += digit_image_info.image_width_pixels;

    // if (previous_boil_digits.hours.ones_face != timers->boil_timer.digits.hours.ones_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->boil_timer.digits.hours.ones_face,
    //         boil_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // boil_digits_offset += digit_image_info.image_width_pixels;

    // //semicolon
    // brewpanel_images_draw_image_immediate(
    //     images_state,
    //     BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,
    //     boil_digits_offset,
    //     y_offset,
    //     draw_buffer
    // );
    // boil_digits_offset += digit_image_info.image_width_pixels;

    // //draw the minutes
    // if (previous_boil_digits.minutes.tens_face != timers->boil_timer.digits.minutes.tens_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->boil_timer.digits.minutes.tens_face,
    //         boil_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // boil_digits_offset += digit_image_info.image_width_pixels;

    // if (previous_boil_digits.minutes.ones_face != timers->boil_timer.digits.minutes.ones_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->boil_timer.digits.minutes.ones_face,
    //         boil_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // boil_digits_offset += digit_image_info.image_width_pixels;

    // //semicolon
    // brewpanel_images_draw_image_immediate(
    //     images_state,
    //     BREWPANEL_IMAGES_ID_RED_DIGIT_COLON,
    //     boil_digits_offset,
    //     y_offset,
    //     draw_buffer
    // );
    // boil_digits_offset += digit_image_info.image_width_pixels;

    // //draw the seconds
    // if (previous_boil_digits.seconds.tens_face != timers->boil_timer.digits.seconds.tens_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->boil_timer.digits.seconds.tens_face,
    //         boil_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }
    // boil_digits_offset += digit_image_info.image_width_pixels;

    // if (previous_boil_digits.seconds.ones_face != timers->boil_timer.digits.seconds.ones_face) {
    //     brewpanel_images_draw_image_immediate(
    //         images_state,
    //         timers->boil_timer.digits.seconds.ones_face,
    //         boil_digits_offset,
    //         y_offset,
    //         draw_buffer
    //     );
    //     redraw = true;
    // }

    return(redraw);
}


internal void
brewpanel_timer_control_update_buttons(
    BrewPanelTimerControl* timer_control,
    BrewPanelButtonStore*  button_store,
    images_store*          images) {

    switch (timer_control->state) {

        case BREWPANEL_TIMER_STATE_IDLE: {

            brewpanel_buttons_disable(button_store,timer_control->buttons.start_button_id,images);
            brewpanel_buttons_disable(button_store,timer_control->buttons.stop_button_id,images);
            brewpanel_buttons_disable(button_store,timer_control->buttons.pause_button_id,images);
            brewpanel_buttons_enable(button_store,timer_control->buttons.reset_button_id,images);

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
    BrewPanelTimers*       timers,
    BrewPanelImagesStore*  images_state,
    BrewPanelButtonStore*  button_store,
    mem_data               draw_buffer) {

    //update the timer buttons
    brewpanel_timer_control_update_buttons(&timers->mash_lauter_timer,button_store,images_state);
    brewpanel_timer_control_update_buttons(&timers->boil_timer,button_store,images_state);

    //calculate the timestamps
    BrewPanelTimerTimestamp mlt_timestamp = 
        brewpanel_timer_control_calculate_timestamp(
            timers->mash_lauter_timer.set_time_seconds,
            timers->mash_lauter_timer.elapsed_time_seconds
    );
    BrewPanelTimerTimestamp boil_timestamp = 
        brewpanel_timer_control_calculate_timestamp(
            timers->boil_timer.set_time_seconds,
            timers->boil_timer.elapsed_time_seconds
    );

    //draw the timers
    bool redraw = brewpanel_timer_control_draw_timers(
        timers,
        images_state,
        button_store,
        draw_buffer
    );
    
    //get the timer images
    redraw |= brewpanel_timer_control_calculate_and_draw_digits(
        timers,
        mlt_timestamp,
        boil_timestamp,
        images_state
    );

    
    return(redraw);
}