#include "brewpanel-mode-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal void
brewpanel_mode_control_create(
    mode_control* mode) {

    *mode = {0};

    mode->panel_id = BREWPANEL_IMAGES_ID_MODE_PANEL;
    mode->redraw   = true;
}

internal bool
brewpanel_mode_control_update(
    panel_mode    panel_mode,
    mode_control* mode_control,
    images_store* images,
    mem_data      draw_buffer) {

    bool redraw = false;

    if (mode_control->redraw) {

        brewpanel_images_draw_image(
            images,
            mode_control->panel_id,
            BREWPANEL_MODE_PANEL_X_OFFSET,
            BREWPANEL_MODE_PANEL_Y_OFFSET,
            draw_buffer
        );

        redraw = true;
        mode_control->redraw = false;
    }


    return(redraw);
}