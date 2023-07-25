#pragma once

#include "brewpanel-core.hpp"
#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"
#include "brewpanel-timer-control.cpp"
#include "brewpanel-clock.cpp"


internal void
brewpanel_core_init() {

    //allocate memory for the state
    BrewPanelMemory memory = brewpanel_memory_create();
    brewpanel_state = brewpanel_memory_allocate_struct(&memory,BrewPanelState);
    brewpanel_state->memory = memory;

    //get the images
    brewpanel_state->images = brewpanel_images_state_create(&memory);

    //get the button store
    brewpanel_state->button_store = brewpanel_buttons_create_store(&memory);

    //timer controls
    brewpanel_state->timer_control = brewpanel_timer_control_create(
        &brewpanel_state->button_store,
        &brewpanel_state->images
    );

    //initialize the clock
    brewpanel_state->clock = brewpanel_clock_create();

    //render the main background
    brewpanel_core_render_main_screen();
}

internal bool
brewpanel_core_update_and_render(
    BrewPanelInput* input) {

    bool render_screen = false;

    //update the clock
    brewpanel_clock_update(
        &brewpanel_state->clock,
        &brewpanel_state->images,
        (mem_data)brewpanel_state->back_buffer.pixels
    );


    brewpanel_buttons_update(
        input,
        &brewpanel_state->button_store
    );

    //draw the buttons
    render_screen = brewpanel_buttons_draw(
        &brewpanel_state->button_store,
        &brewpanel_state->images,
        (mem_data)brewpanel_state->back_buffer.pixels
    );

    brewpanel_timer_control_update(
        &brewpanel_state->timer_control,
        &brewpanel_state->images,
        &brewpanel_state->button_store,
        (mem_data)brewpanel_state->back_buffer.pixels
    );

    return(render_screen);
}