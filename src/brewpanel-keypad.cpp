#include "brewpanel-keypad.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal bool
brewpanel_keypad_update(
    keypad*       keypad,
    images_store* images,
    button_store* buttons) {

    if (keypad->redraw) {

        keypad->redraw = false;

        brewpanel_images_draw_image_(
            images,
            keypad->panel_id
        );

        return(true);
    }

    return(false);
}


internal void
brewpanel_keypad_create(
    keypad*       keypad,
    images_store* images,
    button_store* buttons) {

    keypad->redraw = true;

    keypad->panel_id = brewpanel_images_create_image_instance(
        images,
        BREWPANEL_IMAGES_ID_KEYPAD_PANEL,
        BREWPANEL_KEYPAD_OFFSET_X,
        BREWPANEL_KEYPAD_OFFSET_Y
    );
}