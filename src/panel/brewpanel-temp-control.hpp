#ifndef BREWPANEL_TEMP_CONTROL_HPP
#define BREWPANEL_TEMP_CONTROL_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-buttons.hpp"
#include "brewpanel-keypad.hpp"
#include "brewpanel-communication.hpp"

#define BREWPANEL_TEMP_READ_PANEL_Y_OFFSET_BASE 25
#define BREWPANEL_TEMP_READ_PANEL_X_OFFSET      25
#define BREWPANEL_TEMP_READ_PANEL_Y_SPACING     25
#define BREWPANEL_TEMP_READ_PANEL_HEIGHT        140

#define BREWPANEL_TEMP_READ_DIGIT_X_OFFSET 70
#define BREWPANEL_TEMP_READ_DIGIT_Y_OFFSET 45
#define BREWPANEL_TEMP_READ_DIGIT_WIDTH    30

#define BREWPANEL_TEMP_HEATING_ELEMENT_X_OFFSET 299
#define BREWPANEL_TEMP_HEATING_ELEMENT_Y_OFFSET 190

#define BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_X_OFFSET 330
#define BREWPANEL_TEMP_HEATING_ELEMENT_DIGIT_Y_OFFSET 210

#define BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_SET_X_OFFSET 575
#define BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_SET_Y_OFFSET 250

#define BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_OFF_X_OFFSET 575
#define BREWPANEL_TEMP_HEATING_ELEMENT_BUTTON_OFF_Y_OFFSET 200

struct BrewPanelTempReading {
    u8                value;
    u32               temp_hundreds_value;
    u32               temp_tens_value;
    u32               temp_ones_value;
    image_instance_id temp_hundreds_digit;
    image_instance_id temp_tens_digit;
    image_instance_id temp_ones_digit;
};
typedef BrewPanelTempReading temp_reading_values;

struct BrewPanelTempRead {
    bool                redraw;
    temp_reading_values values;
    image_instance_id   panel_id;
    image_instance_id   degree;
    image_instance_id   farenheit;
};
typedef BrewPanelTempRead temp_read;

enum BrewPanelTempHeatingElementState : u8 {
    BREWPANEL_TEMP_HEATING_ELEMENT_STATE_DISABLED = 0,
    BREWPANEL_TEMP_HEATING_ELEMENT_STATE_OFF      = 1,
    BREWPANEL_TEMP_HEATING_ELEMENT_STATE_SET      = 2,
    BREWPANEL_TEMP_HEATING_ELEMENT_STATE_RUNNING  = 3
};
typedef BrewPanelTempHeatingElementState heating_element_state;

struct BrewPanelTempHeatingElementControl {
    bool                  redraw;
    image_instance_id     panel_id;
    button_id             set_button_id;
    button_id             off_button_id;
    temp_reading_values   temp_values;
    image_instance_id     degree;
    image_instance_id     farenheit;
    image_instance_id     percent;
    heating_element_state state;
    heating_element_state previous_state;
    keypad_input          keypad_input;
    u8                    set_point;
};
typedef BrewPanelTempHeatingElementControl heating_element_control;

struct BrewPanelTempControl {
    bool                    redraw;
    temp_read               hlt_temp_panel;
    temp_read               mlt_temp_panel;
    temp_read               boil_temp_panel;
    heating_element_control mlt_element;
    heating_element_control boil_element;
    image_instance_id       digit_glyph_table;
    image_instance_id       off_panel;
    comm_handler*           comm;
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