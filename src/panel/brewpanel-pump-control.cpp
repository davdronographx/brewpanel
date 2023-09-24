#pragma once

#include "brewpanel-pump-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

//NOTE: these functions seem backwards, because the pump buttons work like toggles
//you are seeing the off button, but when you click it, the button switches to ON

internal void
brewpanel_pump_control_water_off_button_click(
    mem_data payload) {

    pump_control* pumps = (pump_control*)payload;
    pumps->water_pump_status = BREWPANEL_PUMP_CONTROL_STATUS_ON;
}

internal void
brewpanel_pump_control_water_on_button_click(
    mem_data payload) {

    pump_control* pumps = (pump_control*)payload;
    pumps->water_pump_status = BREWPANEL_PUMP_CONTROL_STATUS_OFF;
}

internal void
brewpanel_pump_control_wort_off_button_click(
    mem_data payload) {

    pump_control* pumps = (pump_control*)payload;
    pumps->wort_pump_status = BREWPANEL_PUMP_CONTROL_STATUS_ON;
}

internal void
brewpanel_pump_control_wort_on_button_click(
    mem_data payload) {

    pump_control* pumps = (pump_control*)payload;
    pumps->wort_pump_status = BREWPANEL_PUMP_CONTROL_STATUS_OFF;
}

internal void
brewpanel_pump_control_create(
    pump_control* pumps,
    images_store* images,
    button_store* buttons) {

    *pumps = {0};

    pumps->water_pump_status = BREWPANEL_PUMP_CONTROL_STATUS_OFF;
    pumps->wort_on_button    = BREWPANEL_PUMP_CONTROL_STATUS_OFF;

    pumps->panel_id = 
        brewpanel_images_create_image_instance(
            images,
            BREWPANEL_IMAGES_ID_PUMPS_PANEL,
            BREWPANEL_PUMPS_PANEL_X_OFFSET,
            BREWPANEL_PUMPS_PANEL_Y_OFFSET
    );

    pumps->water_on_button = 
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_pump_control_water_on_button_click,
            (mem_data)pumps,
            BREWPANEL_IMAGES_ID_WATER_ON_IDLE,
            BREWPANEL_IMAGES_ID_WATER_ON_HOVER,
            BREWPANEL_IMAGES_ID_WATER_ON_CLICKED,
            BREWPANEL_IMAGES_ID_WATER_ON_DISABLED,
            BREWPANEL_PUMPS_BUTTONS_X_OFFSET,
            BREWPANEL_PUMPS_WATER_BUTTON_Y_OFFSET
    );

    pumps->wort_on_button = 
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_pump_control_wort_on_button_click,
            (mem_data)pumps,
            BREWPANEL_IMAGES_ID_WORT_ON_IDLE,
            BREWPANEL_IMAGES_ID_WORT_ON_HOVER,
            BREWPANEL_IMAGES_ID_WORT_ON_CLICKED,
            BREWPANEL_IMAGES_ID_WORT_ON_DISABLED,
            BREWPANEL_PUMPS_BUTTONS_X_OFFSET,
            BREWPANEL_PUMPS_WORT_BUTTON_Y_OFFSET
    );

    brewpanel_buttons_hide(buttons,pumps->water_on_button,images);
    brewpanel_buttons_hide(buttons,pumps->wort_on_button,images);

    pumps->water_off_button = 
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_pump_control_water_off_button_click,
            (mem_data)pumps,
            BREWPANEL_IMAGES_ID_WATER_OFF_IDLE,
            BREWPANEL_IMAGES_ID_WATER_OFF_HOVER,
            BREWPANEL_IMAGES_ID_WATER_OFF_CLICKED,
            BREWPANEL_IMAGES_ID_WATER_OFF_DISABLED,
            BREWPANEL_PUMPS_BUTTONS_X_OFFSET,
            BREWPANEL_PUMPS_WATER_BUTTON_Y_OFFSET
    );
    
    pumps->wort_off_button = 
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_pump_control_wort_off_button_click,
            (mem_data)pumps,
            BREWPANEL_IMAGES_ID_WORT_OFF_IDLE,
            BREWPANEL_IMAGES_ID_WORT_OFF_HOVER,
            BREWPANEL_IMAGES_ID_WORT_OFF_CLICKED,
            BREWPANEL_IMAGES_ID_WORT_OFF_DISABLED,
            BREWPANEL_PUMPS_BUTTONS_X_OFFSET,
            BREWPANEL_PUMPS_WORT_BUTTON_Y_OFFSET
    );

    pumps->redraw = true;
}

internal bool
brewpanel_pump_control_update(
    pump_control* pumps,
    images_store* images,
    button_store* buttons) {

    bool redraw = false;

    local BrewPanelPumpControlStatus previous_water_pump_status = BREWPANEL_PUMP_CONTROL_STATUS_OFF;
    local BrewPanelPumpControlStatus previous_wort_pump_status  = BREWPANEL_PUMP_CONTROL_STATUS_OFF;

    if (pumps->redraw) {
        brewpanel_images_draw_image_instance(
            images,pumps->panel_id
        );
        redraw = true;
    }

    if (pumps->water_pump_status != previous_water_pump_status) {

        previous_water_pump_status = pumps->water_pump_status;

        if (pumps->water_pump_status == BREWPANEL_PUMP_CONTROL_STATUS_ON) {
            
            brewpanel_buttons_hide(buttons,pumps->water_off_button,images);
            brewpanel_buttons_show(buttons,pumps->water_on_button,images);
        }
        else {

            brewpanel_buttons_hide(buttons,pumps->water_on_button,images);
            brewpanel_buttons_show(buttons,pumps->water_off_button,images);
        }

        pumps->redraw = true;
    }

    if (pumps->wort_pump_status != previous_wort_pump_status) {

        previous_wort_pump_status = pumps->wort_pump_status;

        if (pumps->wort_pump_status == BREWPANEL_PUMP_CONTROL_STATUS_ON) {
            
            brewpanel_buttons_hide(buttons,pumps->wort_off_button,images);
            brewpanel_buttons_show(buttons,pumps->wort_on_button,images);
        }
        else {

            brewpanel_buttons_hide(buttons,pumps->wort_on_button,images);
            brewpanel_buttons_show(buttons,pumps->wort_off_button,images);
        }

        pumps->redraw = true;
    }

    return(redraw);
}