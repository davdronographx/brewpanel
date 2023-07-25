#pragma once

#include "brewpanel-clock.hpp"
#include "brewpanel-images.cpp"

internal BrewPanelClock 
brewpanel_clock_create() {

    BrewPanelClock clock = {0};

    clock.system_time = brewpanel_platform_system_time_get();

    return(clock);
}

internal bool
brewpanel_clock_update(
    BrewPanelClock*       clock_state,
    BrewPanelImagesState* images_state,
    mem_data              draw_buffer) {

    bool redraw = false;

    //get the system time
    clock_state->system_time = brewpanel_platform_system_time_get();

    //calculate the 12hr am/pm
    if (clock_state->system_time.hours > 12) {
        clock_state->system_time.hours -= 12;
        clock_state->am_pm_face = BREWPANEL_IMAGES_ID_CLOCK_PM;
    } 
    else {
        clock_state->am_pm_face = BREWPANEL_IMAGES_ID_CLOCK_AM;
    }

    //get the tens and ones digits of the clock
    u8 hours_tens   = (clock_state->system_time.hours / 10) % 10;
    u8 hours_ones   = clock_state->system_time.hours - (hours_tens * 10);
    u8 minutes_tens = (clock_state->system_time.minutes / 10) % 10;
    u8 minutes_ones = clock_state->system_time.minutes - (minutes_tens * 10);
    u8 seconds_tens = (clock_state->system_time.seconds / 10) % 10;
    u8 seconds_ones = clock_state->system_time.seconds - (seconds_tens * 10);

    //get the clock face images
    local BrewPanelClockFaces previous_faces = clock_state->faces;
    
    clock_state->faces.hours.tens_face   = brewpanel_clock_glyph_table[hours_tens];
    clock_state->faces.hours.ones_face   = brewpanel_clock_glyph_table[hours_ones];
    clock_state->faces.minutes.tens_face = brewpanel_clock_glyph_table[minutes_tens];
    clock_state->faces.minutes.ones_face = brewpanel_clock_glyph_table[minutes_ones];
    clock_state->faces.seconds.tens_face = brewpanel_clock_glyph_table[seconds_tens];
    clock_state->faces.seconds.ones_face = brewpanel_clock_glyph_table[seconds_ones];

    //get the offset info for the faces
    u32 face_offset = BREWPANEL_CLOCK_OFFSET_X;
    BrewPanelImagesFileIndex digit_image_info = brewpanel_images_index(images_state,clock_state->faces.seconds.ones_face);
    u32 y_offset = BREW_PANEL_HEIGHT_PIXELS - (digit_image_info.image_height_pixels);

    //draw the hours
    if (previous_faces.hours.tens_face != clock_state->faces.hours.tens_face) {
        brewpanel_images_draw_image(
            images_state,
            clock_state->faces.hours.tens_face,
            face_offset,
            y_offset,
            draw_buffer
        );
        redraw = true;
    }
    face_offset += digit_image_info.image_width_pixels;

    if (previous_faces.hours.ones_face != clock_state->faces.hours.ones_face) {
        brewpanel_images_draw_image(
            images_state,
            clock_state->faces.hours.ones_face,
            face_offset,
            y_offset,
            draw_buffer
        );
        redraw = true;
    }
    face_offset += digit_image_info.image_width_pixels;

    //semicolon
    brewpanel_images_draw_image(
        images_state,
        BREWPANEL_IMAGES_ID_CLOCK_COLON,
        face_offset,
        y_offset,
        draw_buffer
    );
    face_offset += digit_image_info.image_width_pixels;

    //draw the minutes
    if (previous_faces.minutes.tens_face != clock_state->faces.minutes.tens_face) {
        brewpanel_images_draw_image(
            images_state,
            clock_state->faces.minutes.tens_face,
            face_offset,
            y_offset,
            draw_buffer
        );
        redraw = true;
    }
    face_offset += digit_image_info.image_width_pixels;

    if (previous_faces.minutes.tens_face != clock_state->faces.minutes.ones_face) {
        brewpanel_images_draw_image(
            images_state,
            clock_state->faces.minutes.ones_face,
            face_offset,
            y_offset,
            draw_buffer
        );
        redraw = true;
    }
    face_offset += digit_image_info.image_width_pixels;

    //semicolon
    brewpanel_images_draw_image(
        images_state,
        BREWPANEL_IMAGES_ID_CLOCK_COLON,
        face_offset,
        y_offset,
        draw_buffer
    );
    face_offset += digit_image_info.image_width_pixels;

    //seconds
    if (previous_faces.seconds.tens_face != clock_state->faces.seconds.tens_face) {
        brewpanel_images_draw_image(
            images_state,
            clock_state->faces.seconds.tens_face,
            face_offset,
            y_offset,
            draw_buffer
        );
        redraw = true;
    }
    face_offset += digit_image_info.image_width_pixels;

    if (previous_faces.seconds.tens_face != clock_state->faces.seconds.ones_face) {
        brewpanel_images_draw_image(
            images_state,
            clock_state->faces.seconds.ones_face,
            face_offset,
            y_offset,
            draw_buffer
        );
        redraw = true;
    }
    face_offset += digit_image_info.image_width_pixels;

    //am/pm
    brewpanel_images_draw_image(
        images_state,
        clock_state->am_pm_face,
        face_offset,
        y_offset,
        draw_buffer
    );

    return true;
}