#include "brewpanel-mode-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"
#include "brewpanel-communication.cpp"

#pragma once

internal void
brewpanel_mode_control_mash_button_click(
    mem_data payload) {

    mode_control* control = (mode_control*)payload;

    control->mode = 
        control->mode == BREWPANEL_MODE_OFF
            ? BREWPANEL_MODE_MASH
            : BREWPANEL_MODE_OFF;

    brewpanel_communication_send_message_mode_change(
        control->comm,
        control->mode == BREWPANEL_MODE_OFF ? BREWPANEL_COMMUNICATION_MODE_OFF : BREWPANEL_COMMUNICATION_MODE_MASH
    );
}

internal void
brewpanel_mode_control_boil_button_click(
    mem_data payload) {

    mode_control* control = (mode_control*)payload;

    control->mode = 
        control->mode == BREWPANEL_MODE_OFF
            ? BREWPANEL_MODE_BOIL
            : BREWPANEL_MODE_OFF;
    
    brewpanel_communication_send_message_mode_change(
        control->comm,
        control->mode == BREWPANEL_MODE_OFF ? BREWPANEL_COMMUNICATION_MODE_OFF : BREWPANEL_COMMUNICATION_MODE_BOIL
    );
}

internal void
brewpanel_mode_control_create(
    mode_control* mode,
    images_store* images,
    button_store* buttons,
    comm_handler* comm) {

    *mode = {0};
    mode->mode = BREWPANEL_MODE_OFF;
    mode->comm = comm;

    mode->panel_id =
        brewpanel_images_create_image_instance(
            images,
            BREWPANEL_IMAGES_ID_MODE_PANEL,
            BREWPANEL_MODE_PANEL_X_OFFSET,
            BREWPANEL_MODE_PANEL_Y_OFFSET        
    );

    mode->mash_mode_button = 
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_mode_control_mash_button_click,
            (mem_data)mode,
            BREWPANEL_IMAGES_ID_MODE_MASH_OFF_IDLE,
            BREWPANEL_IMAGES_ID_MODE_MASH_OFF_HOVER,
            BREWPANEL_IMAGES_ID_MODE_MASH_OFF_CLICKED,
            BREWPANEL_IMAGES_ID_MODE_MASH_OFF_DISABLED,
            BREWPANEL_MODE_BUTTONS_X_OFFSET,
            BREWPANEL_MODE_MASH_BUTTON_Y_OFFSET
    );

    mode->boil_mode_button = 
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_mode_control_boil_button_click,
            (mem_data)mode,
            BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_IDLE,
            BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_HOVER,
            BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_CLICKED,
            BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_DISABLED,
            BREWPANEL_MODE_BUTTONS_X_OFFSET,
            BREWPANEL_MODE_BOIL_BUTTON_Y_OFFSET
    );
 
    mode->redraw   = true;
}

internal bool
brewpanel_mode_control_update(
    mode_control* mode_control,
    images_store* images,
    button_store* buttons,
    bool          keypad_in_use) {

    bool redraw = false;

    local panel_mode previous_mode = BREWPANEL_MODE_OFF;
    local bool re_enabled = false;

    //we don't want the ability to change mode in the middle of capturing input
    if (keypad_in_use) {
        re_enabled = false;
        brewpanel_buttons_disable(buttons,mode_control->mash_mode_button,images);
        brewpanel_buttons_disable(buttons,mode_control->boil_mode_button,images);
    } 
    else if (!keypad_in_use && 
             !re_enabled &&
             brewpanel_buttons_is_disabled(buttons,mode_control->mash_mode_button) &&
             brewpanel_buttons_is_disabled(buttons,mode_control->boil_mode_button)) {
        brewpanel_buttons_enable(buttons,mode_control->mash_mode_button,images);
        brewpanel_buttons_enable(buttons,mode_control->boil_mode_button,images);
        re_enabled = true;
    } 

    if (re_enabled || previous_mode != mode_control->mode) {
        
        re_enabled = false;

        switch (mode_control->mode) {
        
            case BREWPANEL_MODE_OFF: {

                brewpanel_buttons_enable(buttons,mode_control->mash_mode_button,images);
                brewpanel_buttons_enable(buttons,mode_control->boil_mode_button,images);

                buttons->images.idle[mode_control->mash_mode_button]     = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_IDLE; 
                buttons->images.hover[mode_control->mash_mode_button]    = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_HOVER; 
                buttons->images.clicked[mode_control->mash_mode_button]  = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_CLICKED; 
                buttons->images.disabled[mode_control->mash_mode_button] = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_DISABLED; 

                buttons->images.idle[mode_control->boil_mode_button]     = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_IDLE; 
                buttons->images.hover[mode_control->boil_mode_button]    = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_HOVER; 
                buttons->images.clicked[mode_control->boil_mode_button]  = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_CLICKED; 
                buttons->images.disabled[mode_control->boil_mode_button] = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_DISABLED; 

            } break;

            case BREWPANEL_MODE_MASH: {
                brewpanel_buttons_enable(buttons,mode_control->mash_mode_button,images);
                brewpanel_buttons_disable(buttons,mode_control->boil_mode_button,images);

                buttons->images.idle[mode_control->mash_mode_button]     = BREWPANEL_IMAGES_ID_MODE_MASH_ON_IDLE; 
                buttons->images.hover[mode_control->mash_mode_button]    = BREWPANEL_IMAGES_ID_MODE_MASH_ON_HOVER; 
                buttons->images.clicked[mode_control->mash_mode_button]  = BREWPANEL_IMAGES_ID_MODE_MASH_ON_CLICKED; 
                buttons->images.disabled[mode_control->mash_mode_button] = BREWPANEL_IMAGES_ID_MODE_MASH_ON_DISABLED; 

                buttons->images.idle[mode_control->boil_mode_button]     = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_IDLE; 
                buttons->images.hover[mode_control->boil_mode_button]    = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_HOVER; 
                buttons->images.clicked[mode_control->boil_mode_button]  = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_CLICKED; 
                buttons->images.disabled[mode_control->boil_mode_button] = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_DISABLED; 

            } break;

            case BREWPANEL_MODE_BOIL: {
                brewpanel_buttons_enable(buttons,mode_control->boil_mode_button,images);
                brewpanel_buttons_disable(buttons,mode_control->mash_mode_button,images);

                buttons->images.idle[mode_control->mash_mode_button]     = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_IDLE; 
                buttons->images.hover[mode_control->mash_mode_button]    = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_HOVER; 
                buttons->images.clicked[mode_control->mash_mode_button]  = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_CLICKED; 
                buttons->images.disabled[mode_control->mash_mode_button] = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_DISABLED; 

                buttons->images.idle[mode_control->boil_mode_button]     = BREWPANEL_IMAGES_ID_MODE_BOIL_ON_IDLE; 
                buttons->images.hover[mode_control->boil_mode_button]    = BREWPANEL_IMAGES_ID_MODE_BOIL_ON_HOVER; 
                buttons->images.clicked[mode_control->boil_mode_button]  = BREWPANEL_IMAGES_ID_MODE_BOIL_ON_CLICKED; 
                buttons->images.disabled[mode_control->boil_mode_button] = BREWPANEL_IMAGES_ID_MODE_BOIL_ON_DISABLED; 

            } break;

            default: {
                //by default the panel is off
                mode_control->mode = BREWPANEL_MODE_OFF;
                
                brewpanel_buttons_enable(buttons,mode_control->mash_mode_button,images);
                brewpanel_buttons_enable(buttons,mode_control->boil_mode_button,images);

                buttons->images.idle[mode_control->mash_mode_button]     = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_IDLE; 
                buttons->images.hover[mode_control->mash_mode_button]    = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_HOVER; 
                buttons->images.clicked[mode_control->mash_mode_button]  = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_CLICKED; 
                buttons->images.disabled[mode_control->mash_mode_button] = BREWPANEL_IMAGES_ID_MODE_MASH_OFF_DISABLED; 

                buttons->images.idle[mode_control->boil_mode_button]     = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_IDLE; 
                buttons->images.hover[mode_control->boil_mode_button]    = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_HOVER; 
                buttons->images.clicked[mode_control->boil_mode_button]  = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_CLICKED; 
                buttons->images.disabled[mode_control->boil_mode_button] = BREWPANEL_IMAGES_ID_MODE_BOIL_OFF_DISABLED; 
            } break;
        }
    }

    if (mode_control->redraw) {

        brewpanel_images_draw_image_instance(images,mode_control->panel_id);

        redraw = true;
        mode_control->redraw = false;
    }

    previous_mode = mode_control->mode;

    return(redraw);
}