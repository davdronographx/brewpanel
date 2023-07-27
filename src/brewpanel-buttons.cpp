#pragma once

#include "brewpanel-buttons.hpp"

internal BrewPanelButtonStore
brewpanel_buttons_create_store(
    BrewPanelMemory* memory) {

    BrewPanelButtonStore button_store = {0};

    button_store.button_count = -1;
    button_store.button_id_matrix = (s8*)brewpanel_memory_allocate(memory,BREW_PANEL_PIXEL_COUNT);

    memset(
        button_store.button_id_matrix,
        BREW_PANEL_BUTTONS_NULL,
        BREW_PANEL_PIXEL_COUNT
    );

    return(button_store);
}

internal s8
brewpanel_buttons_create_button(
    BrewPanelButtonStore*      button_store,
    BrewPanelImagesState*      images_state,
    func_button_click_callback click_callback,
    mem_data                   payload,
    BrewPanelImagesId          button_image_id_idle,
    BrewPanelImagesId          button_image_id_hover,
    BrewPanelImagesId          button_image_id_clicked,
    BrewPanelImagesId          button_image_id_disabled,
    u32                        x_offset,
    u32                        y_offset) {

    //make sure we have space available for the button
    brewpanel_assert(button_store->button_count != BREW_PANEL_BUTTONS_MAX);

    //get the new button id
    s8 button_id = 0;
    if (button_store->button_count <= 0) {
        button_store->button_count = 0;
    }
    ++button_store->button_count;
    button_id = button_store->button_count - 1;

    button_store->states[button_id] = BREWPANEL_BUTTON_STATE_IDLE;

    //set the image ids
    button_store->images.idle[button_id]     = button_image_id_idle; 
    button_store->images.hover[button_id]    = button_image_id_hover;
    button_store->images.clicked[button_id]  = button_image_id_clicked;
    button_store->images.disabled[button_id] = button_image_id_disabled;

    //set the offsets
    button_store->offsets[button_id].x_pixels = x_offset;
    button_store->offsets[button_id].y_pixels = y_offset;

    //callbacks
    button_store->on_click_callbacks[button_id] = click_callback;
    button_store->on_click_payloads[button_id]  = payload;

    //update the button matrix
    //NOTE: also, we're assuming here that all the images are the same dimensions
    BrewPanelImagesFileIndex image_index = brewpanel_images_index(images_state, button_image_id_idle);
    u32 button_height = image_index.image_height_pixels;
    u32 button_width  = image_index.image_width_pixels;
    
    u32 button_matrix_offset = x_offset + (y_offset * BREW_PANEL_WIDTH_PIXELS);
    mem_data button_matrix_address = (mem_data)(&button_store->button_id_matrix[button_matrix_offset]);

    for (
        u32 button_matrix_row = 0;
        button_matrix_row < button_height;
        ++button_matrix_row
    ) {
        memset(
            button_matrix_address,
            button_id,
            button_width
        );
        button_matrix_address +=  BREW_PANEL_WIDTH_PIXELS;
    }

    return(button_id);
}

internal void
brewpanel_buttons_click(
    BrewPanelButtonStore* button_store,
    u32 x_pos,
    u32 y_pos) {

    //get the button id
    u32 button_id_index = (y_pos * BREW_PANEL_WIDTH_PIXELS) + x_pos;
    s8 button_id = button_store->button_id_matrix[button_id_index];

    if (button_id != BREW_PANEL_BUTTONS_NULL) {

        func_button_click_callback callback = button_store->on_click_callbacks[button_id];
        mem_data payload = button_store->on_click_payloads[button_id];
        callback(payload); 
    }

}

internal void
brewpanel_buttons_update(
    BrewPanelInput* input,
    BrewPanelButtonStore* button_store) {

    local button_id last_button_click = BREW_PANEL_BUTTONS_NULL;

    //get the button id
    u32 button_id_index = (input->mouse_y_pos * BREW_PANEL_WIDTH_PIXELS) + input->mouse_x_pos;
    s8 button_id = button_store->button_id_matrix[button_id_index];

    for (
        s8 button_index = 0;
        button_index < button_store->button_count;
        ++button_index
    ) {

        if (button_store->states[button_index] == BREWPANEL_BUTTON_STATE_HOVER) {
            button_store->states[button_index] = BREWPANEL_BUTTON_STATE_IDLE;
        }
    }
    //if we have a button id, the mouse is over a valid button
    if (button_id != BREW_PANEL_BUTTONS_NULL) {

        //we have a click event, so change the button to its clicked state
        if (input->click) {
            button_store->states[button_id] = BREWPANEL_BUTTON_STATE_CLICKED;
        }
        else {
            //otherwise, just set the state to hover
            button_store->states[button_id] = BREWPANEL_BUTTON_STATE_HOVER;
        }
    }

}

internal bool
brewpanel_buttons_draw(
    BrewPanelButtonStore* button_store,
    BrewPanelImagesState* images_state,
    mem_data              draw_buffer) {

    bool render_screen = false;

    for (
        s8 button_index = 0;
        button_index < button_store->button_count;
        ++button_index
    ) {

        BrewPanelButtonState current_button_state  = button_store->states[button_index];
        BrewPanelButtonState state_currently_drawn = button_store->draw_state[button_index];

        //we only want to draw if the state is different
        if (current_button_state == state_currently_drawn) {
            continue;
        }

        render_screen = true;

        //set the draw state        
        button_store->draw_state[button_index] = current_button_state;

        //get the current button
        image_id button_image = 0;
        switch (button_store->states[button_index]) {
            
            case BREWPANEL_BUTTON_STATE_IDLE: {
                button_image = button_store->images.idle[button_index];
            } break;

            case BREWPANEL_BUTTON_STATE_HOVER: {
                button_image = button_store->images.hover[button_index];
            } break;
            
            case BREWPANEL_BUTTON_STATE_CLICKED: {
                button_image = button_store->images.clicked[button_index];
            } break;
            
            case BREWPANEL_BUTTON_STATE_DISABLED: {
                button_image = button_store->images.disabled[button_index];
            } break;
            
            default: {
                button_image = button_store->images.idle[button_index];
            } break;
        }

        u32 x_offset = button_store->offsets[button_index].x_pixels;
        u32 y_offset = button_store->offsets[button_index].y_pixels;

        brewpanel_images_draw_image(
            images_state,
            button_image,
            x_offset,
            y_offset,
            draw_buffer
        );
    }

    return(render_screen);
}

internal void
brewpanel_buttons_draw_button(
    BrewPanelButtonStore* button_store,
    BrewPanelImagesState* images_state,
    button_id             button_id,
    mem_data              draw_buffer) {

    BrewPanelButtonState current_button_state  = button_store->states[button_id];
    button_store->draw_state[button_id] = current_button_state;

    //get the current button
    image_id button_image;
    switch (current_button_state) {
        
        case BREWPANEL_BUTTON_STATE_IDLE: {
            button_image = button_store->images.idle[button_id];
        } break;

        case BREWPANEL_BUTTON_STATE_HOVER: {
            button_image = button_store->images.hover[button_id];
        } break;

        case BREWPANEL_BUTTON_STATE_CLICKED: {
            button_image = button_store->images.clicked[button_id];
        } break;

        case BREWPANEL_BUTTON_STATE_DISABLED: {
            button_image = button_store->images.disabled[button_id];
        } break;

        default: {
            button_image = button_store->images.idle[button_id];
        } break;
    }    

    u32 x_offset = button_store->offsets[button_id].x_pixels;
    u32 y_offset = button_store->offsets[button_id].y_pixels;

    brewpanel_images_draw_image(
        images_state,
        button_image,
        x_offset,
        y_offset,
        draw_buffer
    );
}


