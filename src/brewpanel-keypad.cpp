#include "brewpanel-keypad.hpp"
#include "brewpanel-images.cpp"
#include "brewpanel-buttons.cpp"

#pragma once

internal void
brewpanel_keypad_active_input(
    keypad*                     keypad,
    u8                          num_digits,
    u32                         starting_value,
    func_keypad_button_callback button_callback, 
    mem_data                    payload) {

    brewpanel_assert(button_callback && payload);

    if (keypad->input.input_state != BREWPANEL_KEYPAD_INPUT_STATE_ACTIVE) {
        keypad->input.num_digits       = num_digits;
        keypad->input.input_state      = BREWPANEL_KEYPAD_INPUT_STATE_ACTIVE;
        keypad->input.callback_payload = payload;
        keypad->input.button_callback  = button_callback;
        keypad->input.starting_value   = starting_value;
    }

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
    images->image_instances[keypad->panel_id].image_id = BREWPANEL_IMAGES_ID_KEYPAD_PANEL_ACTIVE;
    keypad->redraw = true;
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
    images->image_instances[keypad->panel_id].image_id = BREWPANEL_IMAGES_ID_KEYPAD_PANEL;
    keypad->redraw = true;
}

internal bool
brewpanel_keypad_update(
    keypad*       keypad,
    images_store* images,
    button_store* buttons) {

    local keypad_input_state previous_state = BREWPANEL_KEYPAD_INPUT_STATE_ACTIVE;

    local u32 last_value = 0;

    u32 new_value = 
        keypad->input.values[0] + 
        (keypad->input.values[1] * 10) +
        (keypad->input.values[2] * 100) +
        (keypad->input.values[3] * 1000) +
        (keypad->input.values[4] * 10000) +
        (keypad->input.values[5] * 100000);

    if (previous_state != keypad->input.input_state || last_value != new_value) {
        if (keypad->input.input_state == BREWPANEL_KEYPAD_INPUT_STATE_IDLE) {
           brewpanel_keypad_disable(keypad,buttons,images);
        }
        else {
            brewpanel_keypad_enable(keypad,buttons,images);
        }

        if (keypad->input.input_state == BREWPANEL_KEYPAD_INPUT_STATE_ACTIVE) {
            if (keypad->input.num_digits > 0 && keypad->input.current_digit_count == keypad->input.num_digits) {
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
            } 
            else {
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
            }
        }
    }

    last_value = new_value;
    previous_state = keypad->input.input_state;

    if (keypad->redraw) {

        keypad->redraw = false;

        brewpanel_images_draw_image_instance(
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

    input->values[0] = 0;

    for (
        u8 index = 0;
        index < input->current_digit_count;
        ++index
    ) {
        input->values[index] = input->values[index+1];
    }

    --input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);

}

internal void
brewpanel_keypad_button_click_0(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 0;

    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_1(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 1;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_2(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 2;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_3(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 3;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_4(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 4;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_5(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 5;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_6(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 6;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_7(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 7;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_8(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 8;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_9(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;

    for (
        u8 index = input->current_digit_count;
        index > 0;
        --index) {

        input->values[index] = input->values[index-1];
    }

    input->values[0] = 9;
    ++input->current_digit_count;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_NUMBER, (mem_data)input, input->callback_payload);
}

internal void
brewpanel_keypad_button_click_set(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_SET, (mem_data)input, input->callback_payload);
    input->input_state = BREWPANEL_KEYPAD_INPUT_STATE_IDLE;
}

internal void
brewpanel_keypad_button_click_cancel(
    mem_data payload) {

    keypad_input* input = (keypad_input*)payload;
    input->current_digit_count = 0;
    input->input_state = BREWPANEL_KEYPAD_INPUT_STATE_IDLE;
    input->button_callback(BREWPANEL_KEYPAD_BUTTON_TYPE_CANCEL, (mem_data)input, input->callback_payload);
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