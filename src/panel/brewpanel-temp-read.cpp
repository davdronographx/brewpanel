#include "brewpanel-temp-control.hpp"
#include "brewpanel-images.cpp"

internal void
brewpanel_temp_read_update_values(
    temp_read*     temp_read,
    images_store*  images) {

    temp_reading_values* temp_values = &temp_read->values;
    
    //get the values
    u8 temp_hundreds = (temp_values->value / 100) % 100;
    u8 temp_tens     = ((temp_values->value - (temp_hundreds *100)) / 10) % 10;
    u8 temp_ones     = temp_values->value - (temp_hundreds * 100) - (temp_tens * 10);

    //calculate the digits
    images->image_instances[temp_values->temp_hundreds_digit].image_id = brewpanel_temp_glyph_table[temp_hundreds];
    images->image_instances[temp_values->temp_tens_digit].image_id     = brewpanel_temp_glyph_table[temp_tens];
    images->image_instances[temp_values->temp_ones_digit].image_id     = brewpanel_temp_glyph_table[temp_ones];

    brewpanel_images_draw_image_instance(images,temp_read->panel_id);
    brewpanel_images_draw_image_instance(images,temp_read->values.temp_hundreds_digit);
    brewpanel_images_draw_image_instance(images,temp_read->values.temp_tens_digit);
    brewpanel_images_draw_image_instance(images,temp_read->values.temp_ones_digit);
    brewpanel_images_draw_image_instance(images,temp_read->degree);
    brewpanel_images_draw_image_instance(images,temp_read->farenheit);
}

internal void
brewpanel_temp_read_update(
    temp_control* control,
    images_store* images) {

    brewpanel_temp_read_update_values(&control->boil_temp_panel,images);
    brewpanel_temp_read_update_values(&control->mlt_temp_panel,images);
    brewpanel_temp_read_update_values(&control->hlt_temp_panel,images);
}

internal void
brewpanel_temp_read_create(
    temp_control* control,
    images_store* images) {

    control->redraw                 = true;
    control->boil_temp_panel.redraw = true;
    control->mlt_temp_panel.redraw  = true;
    control->hlt_temp_panel.redraw  = true;

    //offsets
    u32 hundreds_digit_offset         = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET;
    u32 tens_digit_offset             = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + BREWPANEL_TEMP_READ_DIGIT_WIDTH;
    u32 ones_digit_offset             = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 2);
    u32 degree_offset                 = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 3);
    u32 farenhet_offset               = BREWPANEL_TEMP_READ_DIGIT_X_OFFSET + (BREWPANEL_TEMP_READ_DIGIT_WIDTH * 4);
    u32 mlt_panel_y_offset            = BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE + BREWPANEL_TEMP_READ_PANEL_Y_SPACING + BREWPANEL_TEMP_READ_PANEL_HEIGHT;
    u32 hlt_panel_y_offset            = BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE + (BREWPANEL_TEMP_READ_PANEL_Y_SPACING * 2) + (BREWPANEL_TEMP_READ_PANEL_HEIGHT * 2);
    u32 hlt_digit_y_offset            = BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET + (BREWPANEL_TEMP_READ_PANEL_Y_SPACING * 2) + (BREWPANEL_TEMP_READ_PANEL_HEIGHT * 2);
    u32 mlt_digit_y_offset            = BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET + BREWPANEL_TEMP_READ_PANEL_Y_SPACING + BREWPANEL_TEMP_READ_PANEL_HEIGHT;

    //boil panel
    control->boil_temp_panel.panel_id                   = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_BOIL_TEMP_PANEL,BREWPANEL_TEMP_READ_PANEL_X_OFFSET,BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE);
    control->boil_temp_panel.values.temp_hundreds_digit = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hundreds_digit_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    control->boil_temp_panel.values.temp_tens_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,tens_digit_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    control->boil_temp_panel.values.temp_ones_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,ones_digit_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    control->boil_temp_panel.degree                     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,degree_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);
    control->boil_temp_panel.farenheit                  = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_F,farenhet_offset,BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET);

    //mlt panel
    control->mlt_temp_panel.panel_id                   = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_MLT_TEMP_PANEL,BREWPANEL_TEMP_READ_PANEL_X_OFFSET,mlt_panel_y_offset);
    control->mlt_temp_panel.values.temp_hundreds_digit = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hundreds_digit_offset,mlt_digit_y_offset);
    control->mlt_temp_panel.values.temp_tens_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,tens_digit_offset,mlt_digit_y_offset);
    control->mlt_temp_panel.values.temp_ones_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,ones_digit_offset,mlt_digit_y_offset);
    control->mlt_temp_panel.degree                     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,degree_offset,mlt_digit_y_offset);
    control->mlt_temp_panel.farenheit                  = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_F,farenhet_offset,mlt_digit_y_offset);

    //hlt panel
    control->hlt_temp_panel.panel_id                   = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_HLT_TEMP_PANEL,BREWPANEL_TEMP_READ_PANEL_X_OFFSET,hlt_panel_y_offset);
    control->hlt_temp_panel.values.temp_hundreds_digit = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,hundreds_digit_offset,hlt_digit_y_offset);
    control->hlt_temp_panel.values.temp_tens_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,tens_digit_offset,hlt_digit_y_offset);
    control->hlt_temp_panel.values.temp_ones_digit     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_0,ones_digit_offset,hlt_digit_y_offset);
    control->hlt_temp_panel.degree                     = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_DEGREE,degree_offset,hlt_digit_y_offset);
    control->hlt_temp_panel.farenheit                  = brewpanel_images_create_image_instance(images,BREWPANEL_IMAGES_ID_RED_DIGIT_F,farenhet_offset,hlt_digit_y_offset);
}