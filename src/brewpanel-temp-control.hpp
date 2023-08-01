#ifndef BREWPANEL_TEMP_CONTROL_HPP
#define BREWPANEL_TEMP_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-buttons.hpp"

#define BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE 25
#define BREWPANEL_TEMP_READ_PANEL_X_OFFSET      25
#define BREWPANEL_TEMP_READ_PANEL_Y_SPACING     25

#define BREWPANEL_TEMP_READ_DIGIT_X_OFFSET 70
#define BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET 45

#define BREWPANEL_TEMP_HEATING_ELEMENT_X_OFFSET 299
#define BREWPANEL_TEMP_HEATING_ELEMENT_Y_OFFSET 190

struct BrewPanelTempRead {
    bool     redraw;
    u32      y_offset_panel;
    u32      y_offset_digit;
    u8       temp_fahrenheit;
    image_instance_id panel_id;
    image_instance_id temp_hundreds_digit;
    image_instance_id temp_tens_digit;
    image_instance_id temp_ones_digit;
};
typedef BrewPanelTempRead temp_read;

struct BrewPanelTempHeatingElementControl {
    bool      redraw;
    image_instance_id  panel_id;
    u16       set_point;
    button_id set_button_id;
    button_id off_button_id;
};
typedef BrewPanelTempHeatingElementControl heating_element_control;

struct BrewPanelTempControl {
    bool      redraw;
    temp_read hlt_temp_panel;
    temp_read mlt_temp_panel;
    temp_read boil_temp_panel;
    heating_element_control mlt_element;
    heating_element_control boil_element;
};
typedef BrewPanelTempControl temp_control;


const image_id brewpanel_temp_glyph_table[] = {
    BREWPANEL_IMAGES_ID_RED_DIGIT_0,
    BREWPANEL_IMAGES_ID_RED_DIGIT_1,
    BREWPANEL_IMAGES_ID_RED_DIGIT_2,
    BREWPANEL_IMAGES_ID_RED_DIGIT_3,
    BREWPANEL_IMAGES_ID_RED_DIGIT_4,
    BREWPANEL_IMAGES_ID_RED_DIGIT_5,
    BREWPANEL_IMAGES_ID_RED_DIGIT_6,
    BREWPANEL_IMAGES_ID_RED_DIGIT_7,
    BREWPANEL_IMAGES_ID_RED_DIGIT_8,
    BREWPANEL_IMAGES_ID_RED_DIGIT_9
};

#endif //BREWPANEL_TEMP_CONTROL_HPP