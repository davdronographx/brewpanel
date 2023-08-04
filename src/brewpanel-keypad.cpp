#include "brewpanel-keypad.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal void
brewpanel_keypad_active_input(
    keypad*       keypad,
    u8            num_digits,
    func_keypad_set_callback    set_callback, 
    func_keypad_cancel_callback cancel_callback, 
    mem_data      payload) {

    brewpanel_assert(set_callback && cancel_callback);

    keypad->input.num_digits       = num_digits;
    keypad->input.input_state      = BREWPANEL_KEYPAD_INPUT_STATE_ACTIVE;
    keypad->input.callback_payload = payload;
    keypad->input.set_callback     = set_callback;
    keypad->input.cancel_callback  = cancel_callback;
}

internal void
brewpanel_keypad_enable(
    keypad*       keypad,
    button_store* buttons,
    images_store* images) {

    brewpanel_buttons_enable(buttons,keypad->button_0,images);
    brewpanel_buttons_enable(buttons,keypad->button_1,images);
    brewpanel_buttons_enable(buttons,keypad->button_2,images);
    brewpanel_buttons_enable(buttons,keypad->button_3,images);
    brewpanel_buttons_enable(buttons,keypad->button_4,images);
    brewpanel_buttons_enable(buttons,keypad->button_5,images);
    brewpanel_buttons_enable(buttons,keypad->button_6,images);
    brewpanel_buttons_enable(buttons,keypad->button_7,images);
    brewpanel_buttons_enable(buttons,keypad->button_8,images);
    brewpanel_buttons_enable(buttons,keypad->button_9,images);
    brewpanel_buttons_enable(buttons,keypad->button_del,images);
    brewpanel_buttons_enable(buttons,keypad->button_set,images);
    brewpanel_buttons_enable(buttons,keypad->button_cancel,images);
}

internal void
brewpanel_keypad_disable(
    keypad*       keypad,
    button_store* buttons,
    images_store* images) {

    brewpanel_buttons_disable(buttons,keypad->button_0,images);
    brewpanel_buttons_disable(buttons,keypad->button_1,images);
    brewpanel_buttons_disable(buttons,keypad->button_2,images);
    brewpanel_buttons_disable(buttons,keypad->button_3,images);
    brewpanel_buttons_disable(buttons,keypad->button_4,images);
    brewpanel_buttons_disable(buttons,keypad->button_5,images);
    brewpanel_buttons_disable(buttons,keypad->button_6,images);
    brewpanel_buttons_disable(buttons,keypad->button_7,images);
    brewpanel_buttons_disable(buttons,keypad->button_8,images);
    brewpanel_buttons_disable(buttons,keypad->button_9,images);
    brewpanel_buttons_disable(buttons,keypad->button_del,images);
    brewpanel_buttons_disable(buttons,keypad->button_set,images);
    brewpanel_buttons_disable(buttons,keypad->button_cancel,images);
}

internal bool
brewpanel_keypad_update(
    keypad*       keypad,
    images_store* images,
    button_store* buttons) {

    local keypad_input_state previous_state = BREWPANEL_KEYPAD_INPUT_STATE_ACTIVE;

    if (previous_state != keypad->input.input_state) {
        if (keypad->input.input_state == BREWPANEL_KEYPAD_INPUT_STATE_IDLE) {
           brewpanel_keypad_disable(keypad,buttons,images);
        }
        else {
            brewpanel_keypad_enable(keypad,buttons,images);
        }
    }

    previous_state = keypad->input.input_state;

    if (keypad->redraw) {

        keypad->redraw = false;

        brewpanel_images_draw_image_(
            images,
            keypad->panel_id
        );

        return(true);
    }

    return(false);
}

internal void
brewpanel_keypad_button_click_del(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

}

internal void
brewpanel_keypad_button_click_0(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 0;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_1(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 1;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_2(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 2;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_3(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 3;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_4(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 4;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_5(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 5;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_6(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 6;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_7(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 7;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_8(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 8;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_9(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    input->values[input->current_digit_count] = 9;
    ++input->current_digit_count;
}

internal void
brewpanel_keypad_button_click_set(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;
    input->cancel_callback(input->callback_payload);
    input->input_state = BREWPANEL_KEYPAD_INPUT_STATE_IDLE;
}

internal void
brewpanel_keypad_button_click_cancel(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;
    input->current_digit_count = 0;
    input->input_state = BREWPANEL_KEYPAD_INPUT_STATE_IDLE;
    input->cancel_callback(input->callback_payload);
}

internal void
brewpanel_keypad_create(
    keypad*       keypad,
    images_store* images,
    button_store* buttons) {

    keypad->redraw = true;
    keypad->input.input_state = BREWPANEL_KEYPAD_INPUT_STATE_IDLE;

    keypad->panel_id = brewpanel_images_create_image_instance(
        images,
        BREWPANEL_IMAGES_ID_KEYPAD_PANEL,
        BREWPANEL_KEYPAD_OFFSET_X,
        BREWPANEL_KEYPAD_OFFSET_Y
    );

    keypad->button_0 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_0,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_0_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_0_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_0_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_0_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_0,
        BREWPANEL_KEYPAD_OFFSET_ROW_0
    );

    keypad->button_del = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_del,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_DELETE_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_DELETE_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_DELETE_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_DELETE_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_2,
        BREWPANEL_KEYPAD_OFFSET_ROW_0
    );

    keypad->button_1 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_1,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_1_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_1_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_1_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_1_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_0,
        BREWPANEL_KEYPAD_OFFSET_ROW_1
    );

    keypad->button_2 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_2,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_2_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_2_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_2_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_2_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_1,
        BREWPANEL_KEYPAD_OFFSET_ROW_1
    );

    keypad->button_3 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_3,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_3_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_3_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_3_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_3_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_2,
        BREWPANEL_KEYPAD_OFFSET_ROW_1
    );

    keypad->button_4 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_4,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_4_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_4_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_4_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_4_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_0,
        BREWPANEL_KEYPAD_OFFSET_ROW_2
    );

    keypad->button_5 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_5,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_5_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_5_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_5_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_5_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_1,
        BREWPANEL_KEYPAD_OFFSET_ROW_2
    );

    keypad->button_6 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_6,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_6_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_6_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_6_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_6_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_2,
        BREWPANEL_KEYPAD_OFFSET_ROW_2
    );

    keypad->button_7 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_7,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_7_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_7_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_7_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_7_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_0,
        BREWPANEL_KEYPAD_OFFSET_ROW_3
    );

    keypad->button_8 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_8,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_8_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_8_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_8_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_8_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_1,
        BREWPANEL_KEYPAD_OFFSET_ROW_3
    );

    keypad->button_9 = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_9,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_9_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_9_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_9_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_9_DISABLED,
        BREWPANEL_KEYPAD_OFFSET_COLUMN_2,
        BREWPANEL_KEYPAD_OFFSET_ROW_3
    );

    keypad->button_set = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_set,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_SET_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_SET_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_SET_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_SET_DISABLED,
        BREWPANEL_KEYPAD_CONTROL_OFFSET_X,
        BREWPANEL_KEYPAD_CONTROL_OFFSET_Y
    );
    
    keypad->button_cancel = brewpanel_buttons_create_button(
        buttons,images,
        brewpanel_keypad_button_click_cancel,
        (mem_data)&keypad->input,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_CANCEL_IDLE,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_CANCEL_HOVER,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_CANCEL_CLICKED,
        BREWPANEL_IMAGES_ID_KEYPAD_BUTTON_CANCEL_DISABLED,
        BREWPANEL_KEYPAD_CONTROL_OFFSET_X + 125,
        BREWPANEL_KEYPAD_CONTROL_OFFSET_Y
    );
}