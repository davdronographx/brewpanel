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
#include "brewpanel-pump-control.cpp"


internal void
brewpanel_core_init(BrewPanelControllerInfo controller_info) {

    //assert the api is valid
    brewpanel_assert(platform_api.memory_allocate);
    brewpanel_assert(platform_api.memory_free);
    brewpanel_assert(platform_api.file_open);
    brewpanel_assert(platform_api.file_get_size);
    brewpanel_assert(platform_api.file_create);
    brewpanel_assert(platform_api.file_close);
    brewpanel_assert(platform_api.file_read);
    brewpanel_assert(platform_api.file_write);
    brewpanel_assert(platform_api.system_time_get);
    brewpanel_assert(platform_api.controller_handle);
    brewpanel_assert(platform_api.controller_close);
    brewpanel_assert(platform_api.controller_thread_start_read);
    brewpanel_assert(platform_api.controller_write);

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
    brewpanel_communication_create_handler(
        &brewpanel_state->comm_handler,
        controller_info,
        &brewpanel_state->images
    );
    
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
        &brewpanel_state->button_store,
        &brewpanel_state->comm_handler
    );

    //keypad
    brewpanel_keypad_create(
        &brewpanel_state->keypad,
        &brewpanel_state->images,
        &brewpanel_state->button_store
    );

    brewpanel_pump_control_create(
        &brewpanel_state->pump_control,
        &brewpanel_state->images,
        &brewpanel_state->button_store,
        &brewpanel_state->comm_handler
    );

    //render the main background
    brewpanel_images_draw_image_instance(&brewpanel_state->images,brewpanel_state->main_screen);
}

internal bool
brewpanel_core_update_and_render(
    BrewPanelInput* input) {

    bool redraw = false;

    brewpanel_state->temp_control.hlt_temp_panel.values.value  = brewpanel_state->comm_handler.latest_heartbeat.hlt_element_temp;
    brewpanel_state->temp_control.mlt_temp_panel.values.value  = brewpanel_state->comm_handler.latest_heartbeat.mlt_element_temp;
    brewpanel_state->temp_control.boil_temp_panel.values.value = brewpanel_state->comm_handler.latest_heartbeat.boil_element_temp;
    brewpanel_state->temp_control.redraw = true;

    brewpanel_buttons_update(
        input,
        &brewpanel_state->button_store
    );

    // redraw |= brewpanel_communication_update(
    //     &brewpanel_state->comm_handler,
    //     &brewpanel_state->images
    // );
    
    redraw |= brewpanel_clock_update(
        &brewpanel_state->clock,
        &brewpanel_state->images,
        (mem_data)brewpanel_state->back_buffer.pixels
    );

    redraw |= _brewpanel_temp_control_update(
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

    redraw |= brewpanel_pump_control_update(
        &brewpanel_state->pump_control,
        &brewpanel_state->images,
        &brewpanel_state->button_store
    );

    redraw |= brewpanel_buttons_draw(
        &brewpanel_state->button_store,
        &brewpanel_state->images,
        (mem_data)brewpanel_state->back_buffer.pixels
    );

    return(redraw);
}