#include "brewpanel-temp-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal bool
brewpanel_temp_control_update_heating_element_control(
        heating_element_control* heating_element,
        images_store*  images,
        mem_data       draw_buffer) {

        bool redraw = false;

        if (heating_element->redraw) {

                heating_element->redraw = false;
                brewpanel_images_draw_image(
                        images,
                        heating_element->panel_id,
                        BREWPANEL_TEMP_HEATING_ELEMENT_X_OFFSET,
                        BREWPANEL_TEMP_HEATING_ELEMENT_Y_OFFSET,
                        draw_buffer
                );

                redraw = true;
        }

        return(redraw);
}

internal bool
brewpanel_temp_control_update_temp_read(
    temp_read*     temp_read,
    images_store*  images,
    mem_data       draw_buffer) {

    bool redraw = false;

    //calculate the new temp digits
    image_id previous_temp_hundreds_digit = temp_read->temp_hundreds_digit;
    image_id previous_temp_tens_digit     = temp_read->temp_tens_digit;
    image_id previous_temp_ones_digit     = temp_read->temp_ones_digit;

    u8 temp_hundreds = (temp_read->temp_fahrenheit / 100) % 100;
    u8 temp_tens     = ((temp_read->temp_fahrenheit - (temp_hundreds *100)) / 10) % 10;
    u8 temp_ones     = temp_read->temp_fahrenheit - (temp_hundreds * 100) - (temp_tens * 10);

    temp_read->temp_hundreds_digit = brewpanel_temp_glyph_table[temp_hundreds]; 
    temp_read->temp_tens_digit     = brewpanel_temp_glyph_table[temp_tens]; 
    temp_read->temp_ones_digit     = brewpanel_temp_glyph_table[temp_ones]; 

    temp_read->redraw |= 
        temp_read->temp_hundreds_digit != previous_temp_hundreds_digit ||
        temp_read->temp_tens_digit != previous_temp_tens_digit ||
        temp_read->temp_ones_digit != previous_temp_ones_digit;
    

    if (temp_read->redraw) {
        
        brewpanel_images_draw_image(
                images,
                temp_read->panel_id,
                BREWPANEL_TEMP_READ_PANEL_X_OFFSET,
                temp_read->y_offset_panel,
                draw_buffer
        );

        image_index digit_index = brewpanel_images_index(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0);
        u32 digit_width = digit_index.image_width_pixels;
        u32 digit_x_offset = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET;
        u32 digit_y_offset = temp_read->y_offset_digit;

        brewpanel_images_draw_image(
                images,
                temp_read->temp_hundreds_digit,
                digit_x_offset,
                digit_y_offset,
                draw_buffer
        );

        brewpanel_images_draw_image(
                images,
                temp_read->temp_tens_digit,
                digit_x_offset + digit_width,
                digit_y_offset,
                draw_buffer
        );

        brewpanel_images_draw_image(
                images,
                temp_read->temp_ones_digit,
                digit_x_offset + (digit_width * 2),
                digit_y_offset,
                draw_buffer
        );

        brewpanel_images_draw_image(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,
                digit_x_offset + (digit_width * 3),
                digit_y_offset,
                draw_buffer
        );

        brewpanel_images_draw_image(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_F,
                digit_x_offset + (digit_width * 4),
                digit_y_offset,
                draw_buffer
        );


        redraw = true;
        temp_read->redraw = false;
    }

    return(redraw);
}

internal bool
brewpanel_temp_control_update(
    BrewPanelMode mode,
    temp_control* control,
    images_store* images,
    mem_data      draw_buffer) {

    bool redraw = false;

    //draw the heating element control
    heating_element_control* heating_element = 
        mode == BREWPANEL_MODE_MASH
        ? &control->mlt_element
        : &control->boil_element;

    redraw |= brewpanel_temp_control_update_heating_element_control(heating_element,images,draw_buffer);

    //temp read controls
    redraw |= brewpanel_temp_control_update_temp_read(&control->hlt_temp_panel,images,draw_buffer);
    redraw |= brewpanel_temp_control_update_temp_read(&control->mlt_temp_panel,images,draw_buffer);
    redraw |= brewpanel_temp_control_update_temp_read(&control->boil_temp_panel,images,draw_buffer);

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
    
    //boil panel
    control->boil_temp_panel.panel_id = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_BOIL_TEMP_PANEL,
                BREWPANEL_TEMP_READ_PANEL_X_OFFSET,
                BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE;
        );
    control->boil_temp_panel.temp_hundreds_digit = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_0,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET,
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    control->boil_temp_panel.temp_tens_digit = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_0,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + BREWPANEL_TEMP_READ_DIGIT_WIDTH,
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    control->boil_temp_panel.temp_ones_digit = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_0,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 2),
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    control->boil_temp_panel.degree = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 3),
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    control->boil_temp_panel.farenheit = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_F,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 4),
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    
    //mlt panel
    //
   control->mlt_temp_panel.panel_id = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_BOIL_TEMP_PANEL,
                BREWPANEL_TEMP_READ_PANEL_X_OFFSET,
                BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE;
        );
    control->mlt_temp_panel.temp_hundreds_digit = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_0,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET,
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    control->mlt_temp_panel.temp_tens_digit = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_0,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + BREWPANEL_TEMP_READ_DIGIT_WIDTH,
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    control->mlt_temp_panel.temp_ones_digit = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_0,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 2),
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    control->mlt_temp_panel.degree = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 3),
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );
    control->mlt_temp_panel.farenheit = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_RED_DIGIT_F,
                BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 4),
                BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET;
        );

    //hlt panel
    //BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET + ((BREWPANEL_TEMP_READ_PANEL_Y_SPACING + panel_height) * 2);
    control->hlt_temp_panel.panel_id  = 
        brewpanel_images_create_image_instance(
                images,
                BREWPANEL_IMAGES_ID_HLT_TEMP_PANEL,
                BREWPANEL_TEMP_READ_PANEL_X_OFFSET,
                BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE + (temp_panel_height * 2) + (BREWPANEL_TEMP_READ_PANEL_Y_SPACING * 2);
        );



    image_index panel_index = brewpanel_images_index(images,BREWPANEL_IMAGES_ID_HLT_TEMP_PANEL);
    u32 panel_height = panel_index.image_height_pixels; 

    control->mlt_element.panel_id = BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL; 
    control->mlt_element.redraw   = true;

    control->boil_element.panel_id = BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL; 
    control->boil_element.redraw   = true;
}