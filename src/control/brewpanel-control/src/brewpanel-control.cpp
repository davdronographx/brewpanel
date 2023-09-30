#include <Arduino.h>
#include "brewpanel-control.hpp"

BrewPanelControlState control_state;

void setup() {

    Serial.begin(115200,SERIAL_8N1);
    Serial.setTimeout(1000);

    control_state = {0};

    pinMode(BREWPANEL_CONTROL_PIN_WATER_PUMP,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_WORT_PUMP,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_HLT_CONTACTOR,OUTPUT);
    pinMode(BREWPANEL_CONTROL_PIN_BOIL_CONTACTOR,OUTPUT);

    // brewpanel_control_water_pump_off();
    // brewpanel_control_wort_pump_off();
    // brewpanel_control_hlt_contactor_off();
    // brewpanel_control_boil_contactor_off();
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

void brewpanel_control_incoming_message_queue_push(
    BrewPanelCommunicationMessage message) {

    

}

void brewpanel_control_parse_incoming_message_buffer() {

    //wait until we can remove from the read buffer
    // while (control_state.read_buffer_lock) { }

    control_state.read_buffer_lock = true;

    bool message_start = false;
    bool message_end = false;

    mem_byte message_start_buffer[8] = {0};
    mem_byte message_end_buffer[8]   = {0};

    BrewPanelMessageBuffer message_buffer = {0};

    for (
        u32 buffer_index = 0;
        buffer_index < control_state.incoming_data_buffer.buffer_size;
        ++buffer_index
    ) {
        //get the next byte
        mem_byte byte = control_state.incoming_data_buffer.buffer[buffer_index];

        //look for the message start characters
        if (!message_start) {

            message_start_buffer[7] = message_start_buffer[6]; 
            message_start_buffer[6] = message_start_buffer[5]; 
            message_start_buffer[5] = message_start_buffer[4]; 
            message_start_buffer[4] = message_start_buffer[3]; 
            message_start_buffer[3] = message_start_buffer[2]; 
            message_start_buffer[2] = message_start_buffer[1]; 
            message_start_buffer[1] = message_start_buffer[0]; 
            message_start_buffer[0] = byte;

            message_start = 
                message_start_buffer[7] == '<' &&
                message_start_buffer[6] == '<' &&
                message_start_buffer[5] == '<' &&
                message_start_buffer[4] == '<' &&
                message_start_buffer[3] == '<' &&
                message_start_buffer[2] == '<' &&
                message_start_buffer[1] == '<' &&
                message_start_buffer[0] == '<';

            if (message_start) {
                memset(
                    message_start_buffer,
                    0,
                    8
                );
            }

            continue;
        }

        //we have started a new message
        if (message_start && !message_end) {

            //check if we have started message end
            if (byte == '>') {
                message_end_buffer[7] = message_end_buffer[6]; 
                message_end_buffer[6] = message_end_buffer[5]; 
                message_end_buffer[5] = message_end_buffer[4]; 
                message_end_buffer[4] = message_end_buffer[3]; 
                message_end_buffer[3] = message_end_buffer[2]; 
                message_end_buffer[2] = message_end_buffer[1]; 
                message_end_buffer[1] = message_end_buffer[0]; 
                message_end_buffer[0] = byte;

                message_end = 
                    message_end_buffer[7] == '>' &&
                    message_end_buffer[6] == '>' &&
                    message_end_buffer[5] == '>' &&
                    message_end_buffer[4] == '>' &&
                    message_end_buffer[3] == '>' &&
                    message_end_buffer[2] == '>' &&
                    message_end_buffer[1] == '>' &&
                    message_end_buffer[0] == '>';

                if (message_end) {
                    memset(
                        message_end_buffer,
                        0,
                        8
                    );
                }
            }

            if (!message_end) {
                message_buffer.buffer[message_buffer.buffer_size] = byte;
                ++message_buffer.buffer_size;
            }
        }

        //we have a complete message
        if (message_start && message_end) {
    
            BrewPanelCommunicationMessage message = *(BrewPanelCommunicationMessage*)message_buffer.buffer; 

            brewpanel_control_incoming_message_queue_push(message);

            message_start = false;
            message_end = false;
            message_buffer = {0};
        }
    }

    //clear the incoming data buffer
    memset(
        control_state.incoming_data_buffer.buffer,
        0,
        control_state.incoming_data_buffer.buffer_size
    );

    control_state.incoming_data_buffer.buffer_size = 0;
    control_state.read_buffer_lock = true;
}

void brewpanel_control_handle_incoming_messages() {



}

void loop() {

    brewpanel_control_parse_incoming_message_buffer();

    brewpanel_control_handle_incoming_messages();



    brewpanel_control_message_heartbeat_build_and_send();
}

void serialEvent() {

    //read from the port
    mem_byte read_buffer[128];
    u8       read_buffer_size = 0;

    while (Serial.available() || read_buffer_size == 128) {

        read_buffer[read_buffer_size] = Serial.read();
        ++read_buffer_size;
    }

    //wait until we can move the data to the incoming message buffer
    while (control_state.read_buffer_lock) { };
    control_state.read_buffer_lock = true;

    //calculate how much of the incoming data buffer we are preserving
    u16 bytes_to_preserve = 
        (read_buffer_size + control_state.incoming_data_buffer.buffer_size > BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE )    
        ? BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE - read_buffer_size
        : control_state.incoming_data_buffer.buffer_size;

    //if we can't fit the data into the buffer, we're done - try again next time we get a read
    if (bytes_to_preserve + read_buffer_size > BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE) {
        return;
    }

    //if the new message data is overwriting the entire buffer, we don't need to preserve anything
    if (read_buffer_size < BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE) {
        memmove(
            &control_state.incoming_data_buffer.buffer[read_buffer_size],
            (void*)control_state.incoming_data_buffer.buffer,
            bytes_to_preserve
        );
    }

    //copy the read buffer into the incoming data buffer
    memmove(
        control_state.incoming_data_buffer.buffer,
        (void*)read_buffer,
        read_buffer_size
    );
    control_state.incoming_data_buffer.buffer_size = bytes_to_preserve + read_buffer_size;

    //unlock the buffer
    memset(
        read_buffer,
        0,
        read_buffer_size
    );
    control_state.read_buffer_lock = false;
}