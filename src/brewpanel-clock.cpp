#pragma once

#include "brewpanel-clock.hpp"
#include "brewpanel-images.cpp"

internal void 
brewpanel_clock_create(
    BrewPanelClock* clock,
    images_store*   images) {

    *clock = {0};

    clock->system_time = brewpanel_platform_system_time_get();

    BrewPanelImagesFileIndex digit_image_info = brewpanel_images_index(images,BREWPANEL_IMAGES_ID_CLOCK_0);
    u32 y_offset = BREW_PANEL_HEIGHT_PIXELS - (digit_image_info.image_height_pixels);

    u32 x_offset_hours_tens   = BREWPANEL_CLOCK_OFFSET_X;
    u32 x_offset_hours_ones   = BREWPANEL_CLOCK_OFFSET_X + digit_image_info.image_width_pixels;
    u32 x_offset_colon_1      = BREWPANEL_CLOCK_OFFSET_X + (digit_image_info.image_width_pixels * 2);
    u32 x_offset_minutes_tens = BREWPANEL_CLOCK_OFFSET_X + (digit_image_info.image_width_pixels * 3);
    u32 x_offset_minutes_ones = BREWPANEL_CLOCK_OFFSET_X + (digit_image_info.image_width_pixels * 4);
    u32 x_offset_colon_2      = BREWPANEL_CLOCK_OFFSET_X + (digit_image_info.image_width_pixels * 5);
    u32 x_offset_seconds_tens = BREWPANEL_CLOCK_OFFSET_X + (digit_image_info.image_width_pixels * 6);
    u32 x_offset_seconds_ones = BREWPANEL_CLOCK_OFFSET_X + (digit_image_info.image_width_pixels * 7);
    u32 x_offset_am_pm        = BREWPANEL_CLOCK_OFFSET_X + (digit_image_info.image_width_pixels * 8);

    clock->faces.hours.tens_face   = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_0,x_offset_hours_tens,y_offset);
    clock->faces.hours.ones_face   = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_0,x_offset_hours_ones,y_offset);
    clock->colon_1                 = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_COLON,x_offset_colon_1,y_offset);
    clock->faces.minutes.tens_face = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_0,x_offset_minutes_tens,y_offset);
    clock->faces.minutes.ones_face = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_0,x_offset_minutes_ones,y_offset);
    clock->colon_2                 = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_COLON,x_offset_colon_2,y_offset);
    clock->faces.seconds.tens_face = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_0,x_offset_seconds_tens,y_offset);
    clock->faces.seconds.ones_face = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_0,x_offset_seconds_ones,y_offset);
    clock->am_pm_face              = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_CLOCK_AM,x_offset_am_pm,y_offset);
}

internal bool
brewpanel_clock_update(
    BrewPanelClock*       clock_state,
    BrewPanelImagesState* images,
    mem_data              draw_buffer) {

    bool redraw = false;

    //get the system time
    clock_state->system_time = brewpanel_platform_system_time_get();

    //calculate the 12hr am/pm
    if (clock_state->system_time.hours > 12) {
        clock_state->system_time.hours -= 12;

        images->image_instances[clock_state->am_pm_face].image_id = BREWPANEL_IMAGES_ID_CLOCK_PM;
    } 
    else {
        images->image_instances[clock_state->am_pm_face].image_id = BREWPANEL_IMAGES_ID_CLOCK_AM;
    }

    //get the tens and ones digits of the clock
    u8 hours_tens   = (clock_state->system_time.hours / 10) % 10;
    u8 hours_ones   = clock_state->system_time.hours - (hours_tens * 10);
    u8 minutes_tens = (clock_state->system_time.minutes / 10) % 10;
    u8 minutes_ones = clock_state->system_time.minutes - (minutes_tens * 10);
    u8 seconds_tens = (clock_state->system_time.seconds / 10) % 10;
    u8 seconds_ones = clock_state->system_time.seconds - (seconds_tens * 10);

    image_instance* hours_tens_face_instance   = &images->image_instances[clock_state->faces.hours.tens_face];
    image_instance* hours_ones_face_instance   = &images->image_instances[clock_state->faces.hours.ones_face];
    image_instance* minutes_tens_face_instance = &images->image_instances[clock_state->faces.minutes.tens_face];
    image_instance* minutes_ones_face_instance = &images->image_instances[clock_state->faces.minutes.ones_face];
    image_instance* seconds_tens_face_instance = &images->image_instances[clock_state->faces.seconds.tens_face];
    image_instance* seconds_ones_face_instance = &images->image_instances[clock_state->faces.seconds.ones_face];

    image_id previous_hours_tens_face   = hours_tens_face_instance->image_id;
    image_id previous_hours_ones_face   = hours_ones_face_instance->image_id;
    image_id previous_minutes_tens_face = minutes_tens_face_instance->image_id;
    image_id previous_minutes_ones_face = minutes_ones_face_instance->image_id;
    image_id previous_seconds_tens_face = seconds_tens_face_instance->image_id;
    image_id previous_seconds_ones_face = seconds_ones_face_instance->image_id;

    hours_tens_face_instance->image_id   = brewpanel_clock_glyph_table[hours_tens];
    hours_ones_face_instance->image_id   = brewpanel_clock_glyph_table[hours_ones];
    minutes_tens_face_instance->image_id = brewpanel_clock_glyph_table[minutes_tens];
    minutes_ones_face_instance->image_id = brewpanel_clock_glyph_table[minutes_ones];
    seconds_tens_face_instance->image_id = brewpanel_clock_glyph_table[seconds_tens];
    seconds_ones_face_instance->image_id = brewpanel_clock_glyph_table[seconds_ones];

    redraw |= (
        previous_hours_tens_face != hours_tens_face_instance->image_id ||
        previous_hours_ones_face != hours_ones_face_instance->image_id ||
        previous_minutes_tens_face != minutes_tens_face_instance->image_id ||
        previous_minutes_ones_face != minutes_ones_face_instance->image_id ||
        previous_seconds_tens_face != seconds_tens_face_instance->image_id ||
        previous_seconds_ones_face != seconds_ones_face_instance->image_id
    );

    if (redraw) {
        brewpanel_images_draw_image_(images,clock_state->faces.hours.tens_face);
        brewpanel_images_draw_image_(images,clock_state->faces.hours.ones_face);
        brewpanel_images_draw_image_(images,clock_state->colon_1);
        brewpanel_images_draw_image_(images,clock_state->faces.minutes.tens_face);
        brewpanel_images_draw_image_(images,clock_state->faces.minutes.ones_face);
        brewpanel_images_draw_image_(images,clock_state->colon_2);
        brewpanel_images_draw_image_(images,clock_state->faces.seconds.tens_face);
        brewpanel_images_draw_image_(images,clock_state->faces.seconds.ones_face);
        brewpanel_images_draw_image_(images,clock_state->am_pm_face);
    }

    return(redraw);
}