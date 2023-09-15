
#include "brewpanel-control-types.hpp"
#include "brewpanel-control-communication.hpp"
#include "brewpanel-control-temperature.hpp"
#include <stdio.h>

internal BrewPanelCommunicationHandler
brewpanel_control_communication_init() {

    BrewPanelCommunicationHandler comm_handler = {0};

    Serial.begin(BREWPANEL_CONTROL_COMMUNICATION_BAUD,SERIAL_8N1);
    Serial.setTimeout(BREWPANEL_COMMUNICATION_MESSAGE_TIMEOUT_MS);

    return(comm_handler);
}

internal void
brewpanel_control_communication_build_message_invalid(
    BrewPanelCommunicationMessageBuffer* message_buffer) {
    
    BrewPanelCommunicationMessage message = {0};
    message.header.sender       = BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC;
    message.header.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_INVALID;
    message.header.message_size = BREWPANEL_COMMUNICATION_MESSAGE_SIZE_INVALID;

    memmove(
        message_buffer->buffer,
        &message.header,
        sizeof(BrewPanelCommunicationMessageHeader)
    );

    message_buffer->buffer[BREWPANEL_COMMUNICATION_MESSAGE_SIZE_INVALID] = '\0';
}

internal void
brewpanel_control_communication_build_message_heartbeat_ack(
    BrewPanelCommunicationMessageBuffer* message_buffer,
    BrewpanelControlTemperatureState     temp,
    u64 timestamp) {

    BrewPanelCommunicationMessage message = {0};
    message.header.sender       = BREWPANEL_COMMUNICATION_MESSAGE_SENDER_PLC;
    message.header.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK;
    message.header.message_size = BREWPANEL_COMMUNICATION_MESSAGE_SIZE_HEARTBEAT_ACK;
    message.header.timestamp    = timestamp;

    BrewPanelCommunicationMessagePayloadHeartBeatAck heartbeat_ack = {0};
    heartbeat_ack.hlt_element_temp  = temp.hlt_temp;
    heartbeat_ack.mlt_element_temp  = temp.mlt_set_temp;
    heartbeat_ack.boil_element_temp = temp.boil_temp;

    memmove(
        message_buffer->buffer,
        &message.header,
        sizeof(BrewPanelCommunicationMessageHeader)
    );

    memmove(
        &message_buffer->buffer[sizeof(BrewPanelCommunicationMessageHeader)],
        &heartbeat_ack,
        sizeof(BrewPanelCommunicationMessagePayloadHeartBeatAck)
    );

    message_buffer->buffer[BREWPANEL_COMMUNICATION_MESSAGE_SIZE_HEARTBEAT_ACK] = '\0';
}


internal void
brewpanel_control_communication_update(
    BrewPanelCommunicationHandler*   comm,
    BrewpanelControlTemperatureState temp) {

    mem_byte serial_buffer[BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE] = {0};
    u32 bytes_read = 0;

    while(Serial.available()) {
        
        if (Serial.available() > 0) {
            
            char c = Serial.read();
            serial_buffer[bytes_read] = c;
            ++bytes_read;

            if (c == '\0') {
                break;
            }
        }
    }

    //otherwise, cast the data to the message type
    if (bytes_read > 0) {
        delay(1000);
        Serial.write("ASDF");

    }

    // //if there's no actual data, just empty the incoming message
    // if (bytes_read == 0) {
    //     comm->incoming_message = {0};
    //     return;
    // }

    // //process the request and construct a response
    // switch (comm->incoming_message.header.message_type) {
        
    //     case BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT: {
    //         brewpanel_control_communication_build_message_heartbeat_ack(
    //             &outgoing_message_buffer,
    //             temp,
    //             comm->incoming_message.header.timestamp
    //         );
    //     } break;

    //     default: {
    //         brewpanel_control_communication_build_message_invalid(
    //             &outgoing_message_buffer
    //         );
    //     }
    // }

    // //send the response
    // Serial.write(
    //     (const char*)outgoing_message_buffer.buffer
    // );
}