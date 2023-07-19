#pragma once

#include "brewpanel-buttons.hpp"

internal BrewPanelButtonStore
brewpanel_buttons_create_store(
    BrewPanelMemory* memory) {

    BrewPanelButtonStore button_store = {0};

    button_store.button_count = -1;
    button_store.button_id_matrix = (s8*)brewpanel_memory_allocate(memory,BREW_PANEL_PIXEL_COUNT);

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
    if (button_store->button_count <= 0) {
        button_store->button_count = 0;
    }
    ++button_store->button_count;
    button_id = button_store->button_count - 1;

    //set the image ids
    button_store->images.idle[button_id]     = button_image_id_idle; 
    button_store->images.hover[button_id]    = button_image_id_hover;
    button_store->images.clicked[button_id]  = button_image_id_clicked;
    button_store->images.disabled[button_id] = button_image_id_disabled;

    button_store->offsets[button_id].x_pixels = x_offset;
    button_store->offsets[button_id].y_pixels = y_offset;

    return(button_id);
}

internal void
brewpanel_buttons_draw(
    BrewPanelButtonStore* button_store,
    BrewPanelImagesState* images_state,
    mem_data              draw_buffer) {

    for (
        s8 button_index = 0;
        button_index < button_store->button_count;
        ++button_index
    ) {

        //get the current button
        u16 button_image_id_idle     = button_store->images.idle[button_index];
        u16 button_image_id_hover    = button_store->images.hover[button_index];
        u16 button_image_id_clicked  = button_store->images.clicked[button_index];
        u16 button_image_id_disabled = button_store->images.disabled[button_index];
    
        u32 x_offset = button_store->offsets[button_index].x_pixels;
        u32 y_offset = button_store->offsets[button_index].y_pixels;

        brewpanel_images_draw_image(
            images_state,
            button_image_id_idle,
            x_offset,
            y_offset,
            draw_buffer
        );
    }

}


