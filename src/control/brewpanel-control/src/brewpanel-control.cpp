#include <Arduino.h>
#include "brewpanel-control.hpp"

void
brewpanel_control_message_heartbeat_build() {

}

void setup() {
    Serial.begin(9600,SERIAL_8N1);
    Serial.setTimeout(1000);
}

void loop() {

    comm_message_buffer message_buffer = {0};
    
    char msg_start[9] ="<<<<<<<<";
    char msg_end[9] = ">>>>>>>>";

    comm_message heartbeat = {0};
    heartbeat.header.sender       = BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC;
    heartbeat.header.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK;
    heartbeat.header.message_size = sizeof(BrewPanelCommunicationMessagePayloadHeartbeat);
    
    heartbeat.payload.heartbeat.hlt_element_temp  = 111;
    heartbeat.payload.heartbeat.mlt_element_temp  = 222;
    heartbeat.payload.heartbeat.boil_element_temp = 121;
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
    delay(250);
}

void serialEvent() {



}