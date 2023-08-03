#ifndef BREWPANEL_KEYPAD_HPP
#define BREWPANEL_KEYPAD_HPP

#include "brewpanel-types.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-buttons.hpp"

#define BREWPANEL_KEYPAD_OFFSET_X 740
#define BREWPANEL_KEYPAD_OFFSET_Y 30

#define BREWPANEL_KEYPAD_OFFSET_ROW_0 85 
#define BREWPANEL_KEYPAD_OFFSET_ROW_1 135
#define BREWPANEL_KEYPAD_OFFSET_ROW_2 185
#define BREWPANEL_KEYPAD_OFFSET_ROW_3 235
#define BREWPANEL_KEYPAD_OFFSET_COLUMN_0 800
#define BREWPANEL_KEYPAD_OFFSET_COLUMN_1 850
#define BREWPANEL_KEYPAD_OFFSET_COLUMN_2 900
#define BREWPANEL_KEYPAD_OFFSET_COLUMN_3 950

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
};
typedef BrewPanelKeypad keypad;

#endif //BREWPANEL_KEYPAD_HPP