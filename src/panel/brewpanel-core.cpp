#pragma once

#include "brewpanel-core.hpp"
#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"
#include "brewpanel-timer-control.cpp"
#include "brewpanel-clock.cpp"
#include "brewpanel-communication.cpp"
#include "brewpanel-temp-control.cpp"
#include "brewpanel-mode-control.cpp"
#include "brewpanel-keypad.cpp"

internal void
brewpanel_core_init() {

    //allocate memory for the state
    BrewPanelMemory memory = brewpanel_memory_create();
    brewpanel_state = brewpanel_memory_allocate_struct(&memory,BrewPanelState);
    brewpanel_state->memory = memory;

    //get the images
    brewpanel_images_state_create(
        &brewpanel_state->images,
        &memory,
        (mem_data)brewpanel_state->back_buffer.pixels);

    brewpanel_state->main_screen = brewpanel_images_create_image_instance(
        &brewpanel_state->images,
        BREWPANEL_IMAGES_ID_MAIN_SCREEN,
        0,0
    );

    //create the message handler
    brewpanel_state->comm_handler = brewpanel_communication_create_handler();
    
    //initialize the clock
    brewpanel_clock_create(
        &brewpanel_state->clock,
        &brewpanel_state->images);


    //get the button store
    brewpanel_state->button_store = brewpanel_buttons_create_store(&memory);

    //timer controls
    brewpanel_timer_control_create(
        &brewpanel_state->timer_control,
        &brewpanel_state->button_store,
        &brewpanel_state->images
    );

    //temperature controls
    brewpanel_temp_control_create(
        &brewpanel_state->temp_control,
        &brewpanel_state->button_store,
        &brewpanel_state->images
    );

    //mode control
    brewpanel_mode_control_create(
        &brewpanel_state->mode_control,
        &brewpanel_state->images,
        &brewpanel_state->button_store
    );

    //keypad
    brewpanel_keypad_create(
        &brewpanel_state->keypad,
        &brewpanel_state->images,
        &brewpanel_state->button_store
    );

    //render the main background
    brewpanel_images_draw_image_instance(&brewpanel_state->images,brewpanel_state->main_screen);
}

internal bool
brewpanel_core_update_and_render(
    BrewPanelInput* input) {

    bool redraw = false;

    brewpanel_buttons_update(
        input,
        &brewpanel_state->button_store
    );

    //update the clock
    redraw |= brewpanel_clock_update(
        &brewpanel_state->clock,
        &brewpanel_state->images,
        (mem_data)brewpanel_state->back_buffer.pixels
    );

    redraw |= brewpanel_temp_control_update(
        brewpanel_state->mode_control.mode,
        &brewpanel_state->temp_control,
        &brewpanel_state->images,
        &brewpanel_state->button_store,
        &brewpanel_state->keypad
    );

    redraw |= brewpanel_mode_control_update(
        &brewpanel_state->mode_control,
        &brewpanel_state->images,
        &brewpanel_state->button_store,
        brewpanel_state->keypad.input_reference != NULL
    );

    //draw the keypad
    redraw |= brewpanel_keypad_update(
        &brewpanel_state->keypad,
        &brewpanel_state->images,
        &brewpanel_state->button_store
    );

    redraw |= brewpanel_timer_control_update(
        &brewpanel_state->timer_control,
        &brewpanel_state->images,
        &brewpanel_state->button_store,
        &brewpanel_state->keypad,
        brewpanel_state->mode_control.mode,
        (mem_data)brewpanel_state->back_buffer.pixels
    );

    //draw the buttons
    redraw |= brewpanel_buttons_draw(
        &brewpanel_state->button_store,
        &brewpanel_state->images,
        (mem_data)brewpanel_state->back_buffer.pixels
    );

    return(redraw);
}