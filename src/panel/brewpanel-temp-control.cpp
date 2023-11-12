#include "brewpanel-temp-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"
#include "brewpanel-keypad.cpp"

#pragma once

internal void
brewpanel_temp_control_change_element_state(
    heating_element_control* heating_element,
    heating_element_state    new_state) {

    heating_element->previous_state = heating_element->state;
    heating_element->state          = new_state;
}

internal void
brewpanel_temp_control_heating_element_keypad_callback(
    keypad_click_type button_type,
    mem_data          keypad_input_reference,
    mem_data          payload) {

    heating_element_control* heating_element = (heating_element_control*)payload;
    keypad_input* input = (keypad_input*)keypad_input_reference;

    switch (button_type) {

        case BREWPANEL_KEYPAD_BUTTON_TYPE_SET: {

            brewpanel_temp_control_change_element_state(heating_element,BREWPANEL_TEMP_HEATING_ELEMENT_STATE_RUNNING);
        } break;

        case BREWPANEL_KEYPAD_BUTTON_TYPE_CANCEL: {
            heating_element->temp_values.value = input->starting_value;
            brewpanel_temp_control_change_element_state(heating_element,heating_element->previous_state);
        } break;

        default: {  

        } break;
    }

    heating_element->redraw = true;
}

internal bool
brewpanel_temp_control_update_heating_element_control(
    heating_element_control* heating_element,
    images_store*            images,
    button_store*            buttons,
    keypad*                  keypad,
    panel_mode               mode) {

    bool redraw = false;

    image_id panel_image =
        (mode == BREWPANEL_MODE_MASH)
        ? BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL
        : BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL;
    
    brewpanel_images_update_instance_image(images,heating_element->panel_id,panel_image);


    if (heating_element->previous_state == BREWPANEL_TEMP_HEATING_ELEMENT_STATE_DISABLED) {
        brewpanel_buttons_show(buttons,heating_element->set_button_id,images);
        brewpanel_buttons_show(buttons,heating_element->off_button_id,images);
        heating_element->state = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF;
    }

    switch (heating_element->state) {

        case BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF: {
            brewpanel_buttons_set_idle(buttons,heating_element->set_button_id);
            brewpanel_buttons_set_disabled(buttons,heating_element->off_button_id);
            heating_element->keypad_input = {0};
            heating_element->temp_values.value = 0;

            brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_ones_digit,    BREWPANEL_IMAGES_ID_RED_DIGIT_0);
            brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_tens_digit,    BREWPANEL_IMAGES_ID_RED_DIGIT_0);
            brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_hundreds_digit,BREWPANEL_IMAGES_ID_RED_DIGIT_0);        
        
        } break;

        case BREWPANEL_TEMP_HEATING_ELEMENT_STATE_SET: {
            
            brewpanel_buttons_set_disabled(buttons,heating_element->set_button_id);
            brewpanel_buttons_set_disabled(buttons,heating_element->off_button_id);

            brewpanel_keypad_active_input(
                keypad,3,heating_element->temp_values.value,
                &heating_element->keypad_input,
                brewpanel_temp_control_heating_element_keypad_callback,
                (mem_data)heating_element);

            switch(mode) {

                case BREWPANEL_MODE_MASH: {

                    if (heating_element->temp_values.value > 212) {
                        heating_element->temp_values.value = 212;
                        heating_element->keypad_input.values[2] = 2;
                        heating_element->keypad_input.values[1] = 1;
                        heating_element->keypad_input.values[0] = 2;
                    }

                } break;

                case BREWPANEL_MODE_BOIL: {

                    if (heating_element->temp_values.value > 100) {
                        heating_element->temp_values.value = 100;
                        heating_element->keypad_input.values[2] = 1;
                        heating_element->keypad_input.values[1] = 0;
                        heating_element->keypad_input.values[0] = 0;
                    }
                } break;
            }

            u8 temp_hundreds = heating_element->keypad_input.values[2] * 100;
            u8 temp_tens     = heating_element->keypad_input.values[1] * 10;
            u8 temp_ones     = heating_element->keypad_input.values[0];

            heating_element->temp_values.value  = temp_ones;
            heating_element->temp_values.value += temp_tens;
            heating_element->temp_values.value += temp_hundreds;

            brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_ones_digit,    brewpanel_temp_glyph_table[heating_element->keypad_input.values[0]]);
            brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_tens_digit,    brewpanel_temp_glyph_table[heating_element->keypad_input.values[1]]);
            brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_hundreds_digit,brewpanel_temp_glyph_table[heating_element->keypad_input.values[2]]);

        } break;

        case BREWPANEL_TEMP_HEATING_ELEMENT_STATE_RUNNING: {
            brewpanel_buttons_set_idle(buttons,heating_element->set_button_id);
            brewpanel_buttons_set_idle(buttons,heating_element->off_button_id);

            panel_image = 
                (mode == BREWPANEL_MODE_MASH)
                ? BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL_ON
                : BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL_ON;

            brewpanel_images_update_instance_image(images,heating_element->panel_id,panel_image);

            heating_element->keypad_input = {0};
        } break;

        default: {
            brewpanel_buttons_set_idle(buttons,heating_element->set_button_id);
            brewpanel_buttons_set_disabled(buttons,heating_element->off_button_id);
        } break;

    }

    return(redraw);
}

internal bool
brewpanel_temp_control_update(
    panel_mode    mode,
    temp_control* control,
    images_store* images,
    button_store* buttons,
    keypad*       keypad) {

    local bool redraw = false;
    local panel_mode previous_mode = BREWPANEL_MODE_OFF;

    //first, we need to determine which panel we need to draw
    switch (mode) {
        
        case BREWPANEL_MODE_OFF: {
            brewpanel_images_draw_image_instance(images,control->off_panel);
        } break;

        case BREWPANEL_MODE_BOIL: {
            
            brewpanel_images_draw_image_instance(images,control->boil_element.panel_id);
            brewpanel_images_draw_image_instance(images,control->boil_element.temp_values.temp_hundreds_digit);
            brewpanel_images_draw_image_instance(images,control->boil_element.temp_values.temp_tens_digit);
            brewpanel_images_draw_image_instance(images,control->boil_element.temp_values.temp_ones_digit);
            brewpanel_images_draw_image_instance(images,control->boil_element.percent);
        } break;

        case BREWPANEL_MODE_MASH: {
            brewpanel_images_draw_image_instance(images,control->mlt_element.panel_id);
            brewpanel_images_draw_image_instance(images,control->mlt_element.temp_values.temp_hundreds_digit);
            brewpanel_images_draw_image_instance(images,control->mlt_element.temp_values.temp_tens_digit);
            brewpanel_images_draw_image_instance(images,control->mlt_element.temp_values.temp_ones_digit);
            brewpanel_images_draw_image_instance(images,control->mlt_element.degree);
            brewpanel_images_draw_image_instance(images,control->mlt_element.farenheit);
        } break;

        default: break;
    }

    //if the panel is off, we're done
    if (mode == BREWPANEL_MODE_OFF) {
        control->mlt_element.state  = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_DISABLED;
        control->boil_element.state = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_DISABLED;
        brewpanel_buttons_hide(buttons,control->boil_element.off_button_id,images);
        brewpanel_buttons_hide(buttons,control->boil_element.set_button_id,images);
        brewpanel_buttons_hide(buttons,control->mlt_element.off_button_id,images);
        brewpanel_buttons_hide(buttons,control->mlt_element.set_button_id,images);        
        previous_mode = mode;
        return(redraw);
    }

    //now, determine which heating element we need to update
    heating_element_control* heating_element = 
        mode == BREWPANEL_MODE_MASH
        ? &control->mlt_element
        : &control->boil_element;

    redraw |= brewpanel_temp_control_update_heating_element_control(
        heating_element,
        images,
        buttons,
        keypad,
        mode
    ); 

    previous_mode = mode;

    return(true);
}

internal void
brewpanel_temp_control_mlt_element_set_click(
    mem_data payload) {

    heating_element_control* mlt_element = (heating_element_control*)payload;
    brewpanel_temp_control_change_element_state(mlt_element,BREWPANEL_TEMP_HEATING_ELEMENT_STATE_SET);
    mlt_element->redraw = true;
}

internal void
brewpanel_temp_control_mlt_element_off_click(
    mem_data payload) {

    heating_element_control* mlt_element = (heating_element_control*)payload;
    mlt_element->temp_values.value = 0;
    brewpanel_temp_control_change_element_state(mlt_element,BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF);
    mlt_element->redraw = true;
}

internal void
brewpanel_temp_control_boil_element_set_click(
    mem_data payload) {

    heating_element_control* boil_element = (heating_element_control*)payload;
    brewpanel_temp_control_change_element_state(boil_element,BREWPANEL_TEMP_HEATING_ELEMENT_STATE_SET);
    boil_element->redraw = true;

}

internal void
brewpanel_temp_control_boil_element_off_click(
    mem_data payload) {

    heating_element_control* boil_element = (heating_element_control*)payload;
    boil_element->temp_values.value = 0;
    brewpanel_temp_control_change_element_state(boil_element,BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF);
    boil_element->redraw = true;
}

internal void
brewpanel_temp_control_create(
    temp_control* control,
    button_store* buttons,
    images_store* images) {

    control->redraw                 = true;
    control->mlt_element.redraw     = true;
    control->boil_element.redraw    = true;
    control->off_panel              = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_OFF_TEMP_PANEL,BREWPANEL_TEMP_HEATING_ELEMENT_X_OFFSET,BREWPANEL_TEMP_HEATING_ELEMENT_Y_OFFSET);
    
    //offsets
    u32 element_hundreds_digit_offset = BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_X_OFFSET + BREWPANEL_TEMP_READ_DIGIT_WIDTH;
    u32 element_tens_digit_offset     = BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 2);
    u32 element_ones_offset           = BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 3);
    u32 element_degree_offset         = BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 4);
    u32 element_percent_offset        = BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 4);
    u32 element_farenheit_offset      = BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 5);

    //mlt heating element panels
    control->mlt_element.state                           = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF;
    control->mlt_element.panel_id                        = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL,BREWPANEL_TEMP_HEATING_ELEMENT_X_OFFSET,BREWPANEL_TEMP_HEATING_ELEMENT_Y_OFFSET);
    control->mlt_element.temp_values.temp_hundreds_digit = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,element_hundreds_digit_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);
    control->mlt_element.temp_values.temp_tens_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,element_tens_digit_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);
    control->mlt_element.temp_values.temp_ones_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,element_ones_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);
    control->mlt_element.degree                          = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,element_degree_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);
    control->mlt_element.farenheit                       = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_F,element_farenheit_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);

    //boil heating element panels
    control->boil_element.state                           = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF;
    control->boil_element.panel_id                        = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL,BREWPANEL_TEMP_HEATING_ELEMENT_X_OFFSET,BREWPANEL_TEMP_HEATING_ELEMENT_Y_OFFSET);
    control->boil_element.temp_values.temp_hundreds_digit = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,element_hundreds_digit_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);
    control->boil_element.temp_values.temp_tens_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,element_tens_digit_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);
    control->boil_element.temp_values.temp_ones_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,element_ones_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);
    control->boil_element.percent                         = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_PERCENT,element_percent_offset,BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET);
    
    //mlt controls
    control->mlt_element.set_button_id =
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_temp_control_mlt_element_set_click,
            (mem_data)&control->mlt_element,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_IDLE,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_HOVER,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_CLICKED,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_DISABLED,
            BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_SET_X_OFFSET,
            BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_SET_Y_OFFSET
    );
    control->mlt_element.off_button_id =
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_temp_control_mlt_element_off_click,
            (mem_data)&control->mlt_element,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_IDLE,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_HOVER,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_CLICKED,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_DISABLED,
            BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_OFF_X_OFFSET,
            BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_OFF_Y_OFFSET
    );

    //boil controls
    control->boil_element.set_button_id =
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_temp_control_boil_element_set_click,
            (mem_data)&control->boil_element,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_IDLE,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_HOVER,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_CLICKED,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_SET_DISABLED,
            BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_SET_X_OFFSET,
            BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_SET_Y_OFFSET
    );
    control->boil_element.off_button_id =
        brewpanel_buttons_create_button(
            buttons,
            images,
            brewpanel_temp_control_boil_element_off_click,
            (mem_data)&control->boil_element,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_IDLE,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_HOVER,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_CLICKED,
            BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_ELEMENT_OFF_DISABLED,
            BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_OFF_X_OFFSET,
            BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_OFF_Y_OFFSET
    );

    //on create, the buttons are disabled
    brewpanel_buttons_hide(buttons,control->boil_element.off_button_id,images);
    brewpanel_buttons_hide(buttons,control->boil_element.set_button_id,images);
    brewpanel_buttons_hide(buttons,control->mlt_element.off_button_id,images);
    brewpanel_buttons_hide(buttons,control->mlt_element.set_button_id,images);
}