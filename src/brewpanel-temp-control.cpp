#include "brewpanel-temp-control.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal bool
brewpanel_temp_control_update_heating_element_control(
        heating_element_control* heating_element,
        images_store*  images) {

        bool redraw = false;

        if (heating_element->redraw) {

                brewpanel_images_draw_image_(images,heating_element->panel_id);

                heating_element->redraw = false;
                redraw = true;
        }

        return(redraw);
}

internal bool
brewpanel_temp_control_update_temp_read(
    temp_read*     temp_read,
    images_store*  images) {

    bool redraw = false;

    //calculate the new temp digits
    image_id previous_temp_hundreds_digit = images->image_instances[temp_read->temp_hundreds_digit].image_id;
    image_id previous_temp_tens_digit     = images->image_instances[temp_read->temp_tens_digit].image_id;
    image_id previous_temp_ones_digit     = images->image_instances[temp_read->temp_ones_digit].image_id;

    u8 temp_hundreds = (temp_read->temp_fahrenheit / 100) % 100;
    u8 temp_tens     = ((temp_read->temp_fahrenheit - (temp_hundreds *100)) / 10) % 10;
    u8 temp_ones     = temp_read->temp_fahrenheit - (temp_hundreds * 100) - (temp_tens * 10);

    images->image_instances[temp_read->temp_hundreds_digit].image_id = brewpanel_temp_glyph_table[temp_hundreds]; 
    images->image_instances[temp_read->temp_tens_digit].image_id     = brewpanel_temp_glyph_table[temp_tens]; 
    images->image_instances[temp_read->temp_ones_digit].image_id     = brewpanel_temp_glyph_table[temp_ones]; 

    temp_read->redraw |= 
        images->image_instances[temp_read->temp_hundreds_digit].image_id != previous_temp_hundreds_digit ||
        images->image_instances[temp_read->temp_tens_digit].image_id != previous_temp_tens_digit ||
        images->image_instances[temp_read->temp_ones_digit].image_id != previous_temp_ones_digit;

    if (temp_read->redraw) {
        
        brewpanel_images_draw_image_(images,temp_read->panel_id);
        brewpanel_images_draw_image_(images,temp_read->temp_hundreds_digit);
        brewpanel_images_draw_image_(images,temp_read->temp_tens_digit);
        brewpanel_images_draw_image_(images,temp_read->temp_ones_digit);
        brewpanel_images_draw_image_(images,temp_read->degree);
        brewpanel_images_draw_image_(images,temp_read->farenheit);

        redraw = true;
        temp_read->redraw = false;
    }

    return(redraw);
}

internal bool
brewpanel_temp_control_update(
    BrewPanelMode mode,
    temp_control* control,
    images_store* images) {

    bool redraw = false;

    //draw the heating element control
    heating_element_control* heating_element = 
        mode == BREWPANEL_MODE_MASH
        ? &control->mlt_element
        : &control->boil_element;

    //draw controls
    redraw |= brewpanel_temp_control_update_heating_element_control(heating_element,images);
    redraw |= brewpanel_temp_control_update_temp_read(&control->boil_temp_panel,images);
    redraw |= brewpanel_temp_control_update_temp_read(&control->mlt_temp_panel,images);
    redraw |= brewpanel_temp_control_update_temp_read(&control->hlt_temp_panel,images);

    return(redraw);
}

internal void
brewpanel_temp_control_create(
    temp_control* control,
    button_store* buttons,
    images_store* images) {

    *control = {0};

    control->redraw = true;
    control->boil_temp_panel.redraw = true;
    control->mlt_temp_panel.redraw  = true;
    control->hlt_temp_panel.redraw  = true;
    control->mlt_element.redraw     = true;
    control->boil_element.redraw    = true;

    //offsets
    u32 hundreds_digit_offset = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET;
    u32 tents_digit_offset    = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + BREWPANEL_TEMP_READ_DIGIT_WIDTH;
    u32 ones_digit_offset     = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 2);
    u32 degree_offset         = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 3);
    u32 farenhet_offset       = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 4);
    u32 mlt_panel_y_offset    = BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE + BREWPANEL_TEMP_READ_PANEL_Y_SPACING + BREWPANEL_TEMP_READ_PANEL_HEIGHT; 
    u32 hlt_panel_y_offset    = BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE + (BREWPANEL_TEMP_READ_PANEL_Y_SPACING * 2) + (BREWPANEL_TEMP_READ_PANEL_HEIGHT * 2); 
    u32 hlt_digit_y_offset    = BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET + (BREWPANEL_TEMP_READ_PANEL_Y_SPACING * 2) + (BREWPANEL_TEMP_READ_PANEL_HEIGHT * 2); 
    u32 mlt_digit_y_offset    = BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET + BREWPANEL_TEMP_READ_PANEL_Y_SPACING + BREWPANEL_TEMP_READ_PANEL_HEIGHT; 

    //boil panel
    control->boil_temp_panel.panel_id            = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_BOIL_TEMP_PANEL,BREWPANEL_TEMP_READ_PANEL_X_OFFSET,BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE);
    control->boil_temp_panel.temp_hundreds_digit = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hundreds_digit_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    control->boil_temp_panel.temp_tens_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,tents_digit_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    control->boil_temp_panel.temp_ones_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,ones_digit_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    control->boil_temp_panel.degree              = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,degree_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    control->boil_temp_panel.farenheit           = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_F,farenhet_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    
    //mlt panel
    control->mlt_temp_panel.panel_id            = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_BOIL_TEMP_PANEL,BREWPANEL_TEMP_READ_PANEL_X_OFFSET,mlt_panel_y_offset);
    control->mlt_temp_panel.temp_hundreds_digit = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hundreds_digit_offset,mlt_digit_y_offset);
    control->mlt_temp_panel.temp_tens_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,tents_digit_offset,mlt_digit_y_offset);
    control->mlt_temp_panel.temp_ones_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,ones_digit_offset,mlt_digit_y_offset);
    control->mlt_temp_panel.degree              = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,degree_offset,mlt_digit_y_offset);
    control->mlt_temp_panel.farenheit           = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_F,farenhet_offset,mlt_digit_y_offset);

    //hlt panel
    control->hlt_temp_panel.panel_id            = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_HLT_TEMP_PANEL,BREWPANEL_TEMP_READ_PANEL_X_OFFSET,hlt_panel_y_offset);
    control->hlt_temp_panel.temp_hundreds_digit = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hundreds_digit_offset,hlt_digit_y_offset);
    control->hlt_temp_panel.temp_tens_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,tents_digit_offset,hlt_digit_y_offset);
    control->hlt_temp_panel.temp_ones_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,ones_digit_offset,hlt_digit_y_offset);
    control->hlt_temp_panel.degree              = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,degree_offset,hlt_digit_y_offset);
    control->hlt_temp_panel.farenheit           = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_F,farenhet_offset,hlt_digit_y_offset);

    //heating element panels
    control->mlt_element.panel_id  = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_MLT_ELEMENT_PANEL,BREWPANEL_TEMP_HEATING_ELEMENT_X_OFFSET,BREWPANEL_TEMP_HEATING_ELEMENT_Y_OFFSET);
    control->boil_element.panel_id = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_BOIL_ELEMENT_PANEL,BREWPANEL_TEMP_HEATING_ELEMENT_X_OFFSET,BREWPANEL_TEMP_HEATING_ELEMENT_Y_OFFSET);
}