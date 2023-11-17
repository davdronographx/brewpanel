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

    image_id panel_image = BREWPANEL_IMAGES_ID_NULL;

    switch (heating_element->state) {

        case BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF: {

            panel_image =
                (mode == BREWPANEL_MODE_MASH)
                ? BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL
                : BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL;

            brewpanel_buttons_set_idle(buttons,heating_element->set_button_id);
            brewpanel_buttons_set_disabled(buttons,heating_element->off_button_id);

            heating_element->keypad_input = {0};
            heating_element->temp_values.value = {0};      
        
        } break;

        case BREWPANEL_TEMP_HEATING_ELEMENT_STATE_SET: {

            panel_image =
                (mode == BREWPANEL_MODE_MASH)
                ? BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL_INPUT
                : BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL_INPUT;

            brewpanel_buttons_set_disabled(buttons,heating_element->set_button_id);
            brewpanel_buttons_set_disabled(buttons,heating_element->off_button_id);

            brewpanel_keypad_active_input(
                keypad,3,heating_element->temp_values.value,
                &heating_element->keypad_input,
                brewpanel_temp_control_heating_element_keypad_callback,
                (mem_data)heating_element);

            heating_element->temp_values.temp_hundreds_value = heating_element->keypad_input.values[2] * 100;
            heating_element->temp_values.temp_tens_value     = heating_element->keypad_input.values[1] * 10;
            heating_element->temp_values.temp_ones_value     = heating_element->keypad_input.values[0];

            heating_element->temp_values.value  = heating_element->temp_values.temp_hundreds_value;
            heating_element->temp_values.value += heating_element->temp_values.temp_tens_value;
            heating_element->temp_values.value += heating_element->temp_values.temp_ones_value;

        } break;

        case BREWPANEL_TEMP_HEATING_ELEMENT_STATE_RUNNING: {

            brewpanel_buttons_set_idle(buttons,heating_element->set_button_id);
            brewpanel_buttons_set_idle(buttons,heating_element->off_button_id);

            switch(mode) {

                case BREWPANEL_MODE_MASH: {

                    panel_image = BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL_ON;

                    if (heating_element->temp_values.value > 212) {
                        heating_element->temp_values.value = 212;
                    }
                    else if (heating_element->temp_values.value < 60) {
                        heating_element->temp_values.value = 60;
                    }

                } break;

                case BREWPANEL_MODE_BOIL: {

                    panel_image = BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL_ON;

                    if (heating_element->temp_values.value > 100) {
                        heating_element->temp_values.value = 100;
                    }
                    else if (heating_element->temp_values.value < 25) {
                        heating_element->temp_values.value = 25;
                    }

                } break;
            }  

            heating_element->keypad_input = {0};

        } break;

        default: {

            heating_element->state = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF;
        
        } break;
    }

    // brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_ones_digit,    BREWPANEL_IMAGES_ID_RED_DIGIT_0);
    // brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_tens_digit,    BREWPANEL_IMAGES_ID_RED_DIGIT_0);
    // brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_hundreds_digit,BREWPANEL_IMAGES_ID_RED_DIGIT_0);        
    

    brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_ones_digit,    brewpanel_temp_glyph_table[heating_element->temp_values.temp_ones_value]);
    brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_tens_digit,    brewpanel_temp_glyph_table[heating_element->temp_values.temp_tens_value]);
    // brewpanel_images_update_instance_image(images,heating_element->temp_values.temp_hundreds_digit,brewpanel_temp_glyph_table[heating_element->temp_values.temp_hundreds_value]);        
    
    brewpanel_images_update_instance_image(images,heating_element->panel_id,panel_image);

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

    heating_element_control* heating_element = NULL;

    switch (mode) {
        
        case BREWPANEL_MODE_OFF: {

            control->mlt_element.state  = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_DISABLED;
            control->boil_element.state = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_DISABLED;
            brewpanel_buttons_hide(buttons,control->boil_element.off_button_id,images);
            brewpanel_buttons_hide(buttons,control->boil_element.set_button_id,images);
            brewpanel_buttons_hide(buttons,control->mlt_element.off_button_id,images);
            brewpanel_buttons_hide(buttons,control->mlt_element.set_button_id,images);  
            brewpanel_images_draw_image_instance(images,control->off_panel);
            return(true);
        } break;

        case BREWPANEL_MODE_BOIL: {
            
            heating_element = &control->boil_element; 

            if (control->boil_element.state == BREWPANEL_TEMP_HEATING_ELEMENT_STATE_DISABLED) {
                control->boil_element.state = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF;
            }

            brewpanel_images_draw_image_instance(images,control->boil_element.panel_id);
            brewpanel_images_draw_image_instance(images,control->boil_element.temp_values.temp_hundreds_digit);
            brewpanel_images_draw_image_instance(images,control->boil_element.temp_values.temp_tens_digit);
            brewpanel_images_draw_image_instance(images,control->boil_element.temp_values.temp_ones_digit);
            brewpanel_images_draw_image_instance(images,control->boil_element.percent);
            brewpanel_buttons_show(buttons,heating_element->set_button_id,images);
            brewpanel_buttons_show(buttons,heating_element->off_button_id,images);

        } break;

        case BREWPANEL_MODE_MASH: {

            heating_element = &control->mlt_element;

            if (control->mlt_element.state == BREWPANEL_TEMP_HEATING_ELEMENT_STATE_DISABLED) {
                control->mlt_element.state = BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF;
            }

            brewpanel_images_draw_image_instance(images,control->mlt_element.panel_id);
            brewpanel_images_draw_image_instance(images,control->mlt_element.temp_values.temp_hundreds_digit);
            brewpanel_images_draw_image_instance(images,control->mlt_element.temp_values.temp_tens_digit);
            brewpanel_images_draw_image_instance(images,control->mlt_element.temp_values.temp_ones_digit);
            brewpanel_images_draw_image_instance(images,control->mlt_element.degree);
            brewpanel_images_draw_image_instance(images,control->mlt_element.farenheit);
            brewpanel_buttons_show(buttons,heating_element->set_button_id,images);
            brewpanel_buttons_show(buttons,heating_element->off_button_id,images);

        } break;

        default: break;
    }

    redraw |= brewpanel_temp_control_update_heating_element_control(
        heating_element,
        images,
        buttons,
        keypad,
        mode
    ); 

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