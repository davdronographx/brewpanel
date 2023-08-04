#ifndef BREWPANEL_KEYPAD_HPP
#define BREWPANEL_KEYPAD_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-buttons.hpp"

#define BREWPANEL_KEYPAD_OFFSET_X         740
#define BREWPANEL_KEYPAD_OFFSET_Y         30
#define BREWPANEL_KEYPAD_OFFSET_ROW_0     85 
#define BREWPANEL_KEYPAD_OFFSET_ROW_1     135
#define BREWPANEL_KEYPAD_OFFSET_ROW_2     185
#define BREWPANEL_KEYPAD_OFFSET_ROW_3     235
#define BREWPANEL_KEYPAD_OFFSET_COLUMN_0  800
#define BREWPANEL_KEYPAD_OFFSET_COLUMN_1  850
#define BREWPANEL_KEYPAD_OFFSET_COLUMN_2  900
#define BREWPANEL_KEYPAD_OFFSET_COLUMN_3  950
#define BREWPANEL_KEYPAD_CONTROL_OFFSET_X 755
#define BREWPANEL_KEYPAD_CONTROL_OFFSET_Y 35

#define BREWPANEL_KEYPAD_INPUT_MAX_VALUES 6

enum BrewPanelKeypadInputState : u8 {
    BREWPANEL_KEYPAD_INPUT_STATE_IDLE   = 0,
    BREWPANEL_KEYPAD_INPUT_STATE_ACTIVE = 1,
    BREWPANEL_KEYPAD_INPUT_STATE_SET    = 2,
    BREWPANEL_KEYPAD_INPUT_STATE_CANCEL = 3,
};
typedef BrewPanelKeypadInputState keypad_input_state;

struct BrewPanelKeypadInput {
    u8                 values[BREWPANEL_KEYPAD_INPUT_MAX_VALUES];
    u8                 num_digits;
    u8                 current_digit_count;
    keypad_input_state input_state;
};
typedef BrewPanelKeypadInput keypad_input;

struct BrewPanelKeypad {
    bool              redraw;
    image_instance_id panel_id;
    button_id         button_del;
    button_id         button_0;
    button_id         button_1;
    button_id         button_2;
    button_id         button_3;
    button_id         button_4;
    button_id         button_5;
    button_id         button_6;
    button_id         button_7;
    button_id         button_8;
    button_id         button_9;
    button_id         button_set;
    button_id         button_cancel;
    keypad_input*     input;
};
typedef BrewPanelKeypad keypad;

#endif //BREWPANEL_KEYPAD_HPP