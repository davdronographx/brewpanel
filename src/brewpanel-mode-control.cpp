#include "brewpanel-mode-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal void
brewpanel_mode_control_mash_button_click(
    mem_data payload) {

}

internal void
brewpanel_mode_control_boil_button_click(
    mem_data payload) {

}

internal void
brewpanel_mode_control_create(
    mode_control* mode,
    images_store* images,
    button_store* buttons) {

    *mode = {0};

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
        brewpanel_mode_control_mash_button_click,
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
    panel_mode    panel_mode,
    mode_control* mode_control,
    images_store* images) {

    bool redraw = false;

    if (mode_control->redraw) {

        brewpanel_images_draw_image_(images,mode_control->panel_id);

        redraw = true;
        mode_control->redraw = false;
    }


    return(redraw);
}