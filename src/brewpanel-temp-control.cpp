#include "brewpanel-temp-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal bool
brewpanel_temp_control_update(
    temp_control* control,
    images_store* images,
    mem_data      draw_buffer) {

    bool redraw = false;

    if (control->boil_temp_panel.redraw) {
        brewpanel_images_draw_image(
            images,control->boil_temp_panel.panel_id,
            BREWPANEL_TEMP_READ_PANEL_X_OFFSET,
            control->boil_temp_panel.y_offset,
            draw_buffer
        );
        redraw = true;
        control->boil_temp_panel.redraw = false;
    }

    if (control->mlt_temp_panel.redraw) {
        brewpanel_images_draw_image(
            images,control->mlt_temp_panel.panel_id,
            BREWPANEL_TEMP_READ_PANEL_X_OFFSET,
            control->mlt_temp_panel.y_offset,
            draw_buffer
        );
        redraw = true;
        control->mlt_temp_panel.redraw = false;
    }

    if (control->hlt_temp_panel.redraw) {
        brewpanel_images_draw_image(
            images,control->hlt_temp_panel.panel_id,
            BREWPANEL_TEMP_READ_PANEL_X_OFFSET,
            control->hlt_temp_panel.y_offset,
            draw_buffer
        );
        redraw = true;
        control->hlt_temp_panel.redraw = false;
    }

    return(redraw);
}

internal void
brewpanel_temp_control_create(
    temp_control* control,
    button_store* buttons,
    images_store* images) {

    *control = {0};
    control->redraw = true;

    //temperature panels
    image_index hlt_panel_index =  brewpanel_images_index(images,BREWPANEL_IMAGES_ID_HLT_TEMP_PANEL);
    u32 temp_panel_height = hlt_panel_index.image_height_pixels;

    control->boil_temp_panel.redraw = true;
    control->mlt_temp_panel.redraw  = true;
    control->hlt_temp_panel.redraw  = true;

    control->boil_temp_panel.panel_id = BREWPANEL_IMAGES_ID_BOIL_TEMP_PANEL;
    control->mlt_temp_panel.panel_id  = BREWPANEL_IMAGES_ID_MLT_TEMP_PANEL;
    control->hlt_temp_panel.panel_id  = BREWPANEL_IMAGES_ID_HLT_TEMP_PANEL;

    control->boil_temp_panel.y_offset = BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE;
    control->mlt_temp_panel.y_offset  = BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE + temp_panel_height + BREWPANEL_TEMP_READ_PANEL_Y_SPACING;
    control->hlt_temp_panel.y_offset  = BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE + (temp_panel_height * 2) + (BREWPANEL_TEMP_READ_PANEL_Y_SPACING * 2);
}