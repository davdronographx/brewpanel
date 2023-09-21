#include <Arduino.h>
#include "brewpanel-control.hpp"

void
brewpanel_control_message_heartbeat_build() {

}

void setup() {
    Serial.begin(115200,SERIAL_8N1);
    Serial.setTimeout(1000);
}

void loop() {

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
    message_buffer.buffer_size = BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE + sizeof(comm_payload_heartbeat);

    memmove(
        message_buffer.buffer,
        heartbeat.header.buffer,
        BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE
    );

    memmove(
        &message_buffer.buffer[BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE],
        heartbeat.payload.data.payload_buffer,
        sizeof(comm_payload_heartbeat)
    );

    message_buffer.buffer[BREWPANEL_COMMUNICATION_MESSAGE_HEADER_SIZE + sizeof(comm_payload_heartbeat)] = '\0';

    Serial.write(message_buffer.buffer,message_buffer.buffer_size);
    delay(2000);
}

void serialEvent() {



}