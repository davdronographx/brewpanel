#include "brewpanel-mode-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal void
brewpanel_mode_control_mash_button_click(
    mem_data payload) {

    mode_control* control = (mode_control*)payload;
    control->mode = BREWPANEL_MODE_MASH;
}

internal void
brewpanel_mode_control_boil_button_click(
    mem_data payload) {

    mode_control* control = (mode_control*)payload;
    control->mode = BREWPANEL_MODE_BOIL;
}

internal void
brewpanel_mode_control_create(
    mode_control* mode,
    images_store* images,
    button_store* buttons) {

    *mode = {0};
    mode->mode = BREWPANEL_MODE_MASH;

    mode->panel_id =
        brewpanel_images_create_image_instance(
            images,
            BREWPANEL_IMAGES_ID_MODE_PANEL,
            BREWPANEL_MODE_PANEL_X_OFFSET,
            BREWPANEL_MODE_PANEL_Y_OFFSET        
    );

    mode->mash_mode_button = brewpanel_buttons_create_button(
        buttons,
        images,
        brewpanel_mode_control_mash_button_click,
        (mem_data)mode,
        BREWPANEL_IMAGES_ID_MODE_BUTTON_MASH_IDLE,
        BREWPANEL_IMAGES_ID_MODE_BUTTON_MASH_HOVER,
        BREWPANEL_IMAGES_ID_MODE_BUTTON_MASH_CLICKED,
        BREWPANEL_IMAGES_ID_MODE_BUTTON_MASH_DISABLED,
        BREWPANEL_MODE_BUTTONS_X_OFFSET,
        BREWPANEL_MODE_MASH_BUTTON_Y_OFFSET
    );

    mode->boil_mode_button = brewpanel_buttons_create_button(
        buttons,
        images,
        brewpanel_mode_control_boil_button_click,
        (mem_data)mode,
        BREWPANEL_IMAGES_ID_MODE_BUTTON_BOIL_IDLE,
        BREWPANEL_IMAGES_ID_MODE_BUTTON_BOIL_HOVER,
        BREWPANEL_IMAGES_ID_MODE_BUTTON_BOIL_CLICKED,
        BREWPANEL_IMAGES_ID_MODE_BUTTON_BOIL_DISABLED,
        BREWPANEL_MODE_BUTTONS_X_OFFSET,
        BREWPANEL_MODE_BOIL_BUTTON_Y_OFFSET
    );
 
    mode->redraw   = true;
}

internal bool
brewpanel_mode_control_update(
    mode_control* mode_control,
    images_store* images,
    button_store* buttons) {

    bool redraw = false;

    switch (mode_control->mode) {
        
        case BREWPANEL_MODE_MASH: {
            brewpanel_buttons_disable(buttons,mode_control->mash_mode_button);
            brewpanel_buttons_enable(buttons,mode_control->boil_mode_button,images);
        } break;
        
        case BREWPANEL_MODE_BOIL: {
            brewpanel_buttons_disable(buttons,mode_control->boil_mode_button);
            brewpanel_buttons_enable(buttons,mode_control->mash_mode_button,images);
        } break;

        default: {
            //by default we are in mash mode
            brewpanel_buttons_disable(buttons,mode_control->mash_mode_button);
            brewpanel_buttons_enable(buttons,mode_control->boil_mode_button,images);
        } break;
    }

    if (mode_control->redraw) {

        brewpanel_images_draw_image_(images,mode_control->panel_id);

        redraw = true;
        mode_control->redraw = false;
    }

    return(redraw);
}