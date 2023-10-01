#include <Arduino.h>
#include "brewpanel-control.hpp"

BrewPanelControlState control_state;

bool io_test;

void setup() {

    Serial.begin(115200,SERIAL_8N1);
    Serial.setTimeout(1000);

    control_state = {0};

    pinMode(BREWPANEL_CONTROL_PIN_WATER_PUMP,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_WORT_PUMP,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_HLT_CONTACTOR,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR,OUTPUT);

    io_test = false;
}

void
brewpanel_control_message_heartbeat_build_and_send() {

    comm_message_buffer message_buffer = {0};
    
    char msg_start[9] ="<<<<<<<<";
    char msg_end[9] = ">>>>>>>>";

    control_state.hlt_temp++;
    control_state.mlt_temp++;
    control_state.boil_temp++;

    if (control_state.boil_temp > 212) {
        control_state.hlt_temp = 0;
        control_state.mlt_temp = 0;
        control_state.boil_temp = 0;
    }

    comm_message heartbeat = {0};
    heartbeat.header.sender       = BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC;
    heartbeat.header.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK;
    heartbeat.header.message_size = sizeof(BrewPanelCommunicationMessagePayloadHeartbeat);
    
    heartbeat.payload.heartbeat.hlt_element_temp  = control_state.hlt_temp;
    heartbeat.payload.heartbeat.mlt_element_temp  = control_state.mlt_temp;
    heartbeat.payload.heartbeat.boil_element_temp = control_state.boil_temp;
    heartbeat.payload.heartbeat.timer_set_point_ms = 0xAAAA;
    heartbeat.payload.heartbeat.timer_elapsed_ms = 0xBBBB;
    message_buffer.buffer_size = comm_message_size(heartbeat.header.message_type) + 16; 

    memmove(
        message_buffer.buffer,
        msg_start,
        8
    );

    memmove(
        &message_buffer.buffer[8],
        &heartbeat,
        comm_message_size(heartbeat.header.message_type)
    );

    memmove(
        &message_buffer.buffer[8 + comm_message_size(heartbeat.header.message_type)],
        msg_end,
        8
    );

    Serial.write(message_buffer.buffer,message_buffer.buffer_size);
}


void brewpanel_control_handle_incoming_message() {

    if (!control_state.incoming_message.message_ready) {
        return;
    }

    BrewPanelCommunicationMessage incoming_message = control_state.incoming_message.parsed_message;
    

    switch (incoming_message.header.message_type) {

        case BREWPANEL_COMMUNICATION_MESSAGE_TYPE_PUMP_CONTROL: {

            auto pump_payload = &incoming_message.payload.pump_control;

            switch(pump_payload->pump_id) {
                
                case BREWPANEL_COMMUNICATION_PUMP_ID_WATER: {
                    control_state.water_pump_state = pump_payload->pump_status == BREWPANEL_COMMUNICATION_PUMP_STATUS_ON;
                } break;
                
                case BREWPANEL_COMMUNICATION_PUMP_ID_WORT: {
                    control_state.wort_pump_state = pump_payload->pump_status == BREWPANEL_COMMUNICATION_PUMP_STATUS_ON;
                } break;

                default: break;
            }
        } break;
    }

    control_state.incoming_message.message_ready = false;
}

void brewpanel_control_update_outputs() {

    //pumps
    brewpanel_control_water_pump(control_state.water_pump_state ? ON : OFF);
    brewpanel_control_wort_pump(control_state.wort_pump_state ? ON : OFF);
    delay(10);
}

void brewpanel_control_read_and_parse_incoming_data() {

    if (Serial.available() > 0) {

        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size] = Serial.read();
        ++control_state.incoming_message.message_size;
    }    

    control_state.incoming_message.message_start = 
        control_state.incoming_message.message_size >= 8 &&
        control_state.incoming_message.message_buffer[0] == '<' &&
        control_state.incoming_message.message_buffer[1] == '<' &&
        control_state.incoming_message.message_buffer[2] == '<' &&
        control_state.incoming_message.message_buffer[3] == '<' &&
        control_state.incoming_message.message_buffer[4] == '<' &&
        control_state.incoming_message.message_buffer[5] == '<' &&
        control_state.incoming_message.message_buffer[6] == '<' &&
        control_state.incoming_message.message_buffer[7] == '<';

    control_state.incoming_message.message_end = 
        control_state.incoming_message.message_size > 8 &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 8] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 7] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 6] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 5] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 4] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 3] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 2] == '>' &&
        control_state.incoming_message.message_buffer[control_state.incoming_message.message_size - 1] == '>';

    if (control_state.incoming_message.message_end   && 
        control_state.incoming_message.message_start &&
        control_state.incoming_message.message_size  >= 16) {
            
        //we have a complete message
        control_state.incoming_message.message_ready = true;

        memmove(
            &control_state.incoming_message.parsed_message,
            &control_state.incoming_message.message_buffer[8],
            control_state.incoming_message.message_size - 16
        );

        //clear the message buffer
        memset(
            control_state.incoming_message.message_buffer,
            0,
            control_state.incoming_message.message_size
        );
        control_state.incoming_message.message_size  = 0;
        control_state.incoming_message.message_start = false;
        control_state.incoming_message.message_end   = false;
    }
}

void loop() {

    brewpanel_control_read_and_parse_incoming_data();

    brewpanel_control_handle_incoming_message();

    brewpanel_control_update_outputs();

    brewpanel_control_message_heartbeat_build_and_send();

}