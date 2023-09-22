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
    
    char msg_start[3] ="<<<";
    char msg_end[3] = ">>>";

    comm_message heartbeat = {0};
    heartbeat.header.data.sender       = BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC;
    heartbeat.header.data.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK;
    heartbeat.header.data.message_size = sizeof(BrewPanelCommunicationMessagePayloadHeartbeat);
    heartbeat.header.data.timestamp    = 0;
    
    heartbeat.payload.data.heartbeat.hlt_element_temp  = 111;
    heartbeat.payload.data.heartbeat.mlt_element_temp  = 222;
    heartbeat.payload.data.heartbeat.boil_element_temp = 121;
    heartbeat.payload.type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK;

    comm_message_buffer message_buffer = {0};
    strcat(message_buffer.buffer,msg_start);
    message_buffer.buffer_size = BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE + sizeof(comm_payload_heartbeat) + 6;

    memmove(
        &message_buffer.buffer[3],
        heartbeat.header.buffer,
        BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE
    );

    memmove(
        &message_buffer.buffer[BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE],
        heartbeat.payload.data.payload_buffer,
        sizeof(comm_payload_heartbeat)
    );
    strcat(message_buffer.buffer,msg_end);

    Serial.write(message_buffer.buffer,message_buffer.buffer_size);
    delay(2000);
}

void serialEvent() {



}