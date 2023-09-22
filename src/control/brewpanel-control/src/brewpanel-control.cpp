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
    
    char msg_start[3] ="<<<";
    char msg_end[3] = ">>>";

    comm_message heartbeat = {0};
    heartbeat.header.sender       = BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC;
    heartbeat.header.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK;
    heartbeat.header.message_size = sizeof(BrewPanelCommunicationMessagePayloadHeartbeat);
    heartbeat.header.timestamp    = 0;
    
    heartbeat.payload.heartbeat.hlt_element_temp  = 111;
    heartbeat.payload.heartbeat.mlt_element_temp  = 222;
    heartbeat.payload.heartbeat.boil_element_temp = 121;

    message_buffer.buffer_size = comm_message_size(heartbeat.header.message_type) + 6; 

    memmove(
        message_buffer.buffer,
        msg_start,
        3
    );

    memmove(
        &message_buffer.buffer[3],
        &heartbeat,
        comm_message_size(heartbeat.header.message_type)
    );

    memmove(
        &message_buffer.buffer[3 + comm_message_size(heartbeat.header.message_type)],
        msg_end,
        3
    );


    Serial.write(message_buffer.buffer,message_buffer.buffer_size);
    delay(2000);
}

void serialEvent() {



}