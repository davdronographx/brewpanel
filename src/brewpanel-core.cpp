#pragma once

#include "brewpanel-types.hpp"
#include "brewpanel-core.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-memory.hpp"

#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

internal void
brewpanel_core_test_button_click(
    void* data) {

    brewpanel_nop();
}

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

    //create the buttons
    brewpanel_state->ui_buttons.test_button_id = brewpanel_buttons_create_button(
        &brewpanel_state->button_store,
        &brewpanel_state->images,
        brewpanel_core_test_button_click,
        BREWPANEL_IMAGES_ID_GREEN_BUTTON_IDLE,
        BREWPANEL_IMAGES_ID_GREEN_BUTTON_HOVER,
        BREWPANEL_IMAGES_ID_GREEN_BUTTON_IDLE,
        BREWPANEL_IMAGES_ID_GREEN_BUTTON_IDLE,
        50,
        50
    );

    //render the main background
    brewpanel_core_render_main_screen();
}

internal void
brewpanel_core_update_and_render(
    BrewPanelInput* input) {

    brewpanel_buttons_update(
        input,
        &brewpanel_state->button_store
    );

    //draw the buttons
    brewpanel_buttons_draw(
        &brewpanel_state->button_store,
        &brewpanel_state->images,
        (mem_data)brewpanel_state->back_buffer.pixels
    );

    //reset user input
    // *input = {0};
}