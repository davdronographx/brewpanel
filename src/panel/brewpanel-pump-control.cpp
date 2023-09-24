#pragma once

#include "brewpanel-pump-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

internal void
brewpanel_pump_control_create(
    pump_control* pumps,
    images_store* images,
    button_store* buttons) {

    *pumps = {0};

    pumps->panel_id = 
        brewpanel_images_create_image_instance(
            images,
            BREWPANEL_IMAGES_ID_PUMPS_PANEL,
            BREWPANEL_PUMPS_PANEL_X_OFFSET,
            BREWPANEL_PUMPS_PANEL_Y_OFFSET
    );

    pumps->redraw = true;
}

internal bool
brewpanel_pump_control_update(
    pump_control* pumps,
    images_store* images,
    button_store* buttons) {

    bool redraw = false;

    if (pumps->redraw) {
        brewpanel_images_draw_image_instance(
            images,pumps->panel_id
        );
        redraw = true;
    }


    return(redraw);
}