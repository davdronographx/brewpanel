#pragma once

#include "brewpanel-buttons.hpp"

internal BrewPanelButtonStore
brewpanel_buttons_create_store(
    BrewPanelMemory* memory) {

    BrewPanelButtonStore button_store = {0};

    button_store.button_count = BREW_PANEL_BUTTONS_NULL;
    button_store.button_id_matrix = (s8*)brewpanel_memory_allocate(memory,BREW_PANEL_PIXEL_COUNT);

    memset(
        button_store.button_id_matrix,
        BREW_PANEL_BUTTONS_NULL,
        BREW_PANEL_PIXEL_COUNT
    );

    memset(
        (void*)(&button_store.images),
        BREW_PANEL_BUTTONS_NULL,
        sizeof(BrewPanelButtonImages)
    );

    memset(
        (void*)(button_store.offsets),
        BREW_PANEL_BUTTONS_NULL,
        sizeof(BrewPanelButtonOffsets) * BREW_PANEL_BUTTONS_MAX
    );

    memset(
        (void*)(button_store.on_click_callbacks),
        NULL,
        sizeof(func_button_click_callback) * BREW_PANEL_BUTTONS_MAX
    );

    return(button_store);
}

internal s8
brewpanel_buttons_create_button(
    BrewPanelButtonStore* button_store,
    BrewPanelImagesId     button_image_id_idle,
    BrewPanelImagesId     button_image_id_hover,
    BrewPanelImagesId     button_image_id_clicked,
    BrewPanelImagesId     button_image_id_disabled,
    u32                   x_offset,
    u32                   y_offset) {

    //make sure we have space available for the button
    brewpanel_assert(button_store->button_count != BREW_PANEL_BUTTONS_MAX);

    //get the new button id
    s8 button_id = 0;
    ++button_store->button_count;
    button_id = button_store->button_count;

    //set the image ids
    button_store->images.idle[button_id]     = button_image_id_idle; 
    button_store->images.hover[button_id]    = button_image_id_hover;
    button_store->images.clicked[button_id]  = button_image_id_clicked;
    button_store->images.disabled[button_id] = button_image_id_disabled;

    //TODO: we'll do the offsets after we make sure  the button draws   


    return(button_id);
}


