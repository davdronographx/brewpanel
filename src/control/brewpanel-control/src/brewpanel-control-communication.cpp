#include "brewpanel-control-types.hpp"
#include "brewpanel-control-communication.hpp"

internal BrewPanelCommunicationHandler
brewpanel_control_communication_init() {

    BrewPanelCommunicationHandler comm_handler = {0};

    Serial.begin(BREWPANEL_CONTROL_COMMUNICATION_BAUD);
    Serial.setTimeout(BREWPANEL_COMMUNICATION_MESSAGE_TIMEOUT_MS);

    return(comm_handler);
}

internal void
brewpanel_control_communication_read(
    BrewPanelCommunicationHandler* comm_handler) {

    mem_byte serial_buffer[BREWPANEL_COMMUNICATION_MESSAGE_TOTAL_SIZE];
    u32 bytes_read = 0;

    if (Serial.available()) {

        //if we have data available, read until we hit a terminator or max message size
        bytes_read = Serial.readBytesUntil(
            BREWPANEL_COMMUNICATION_MESSAGE_TERMINATOR,
            serial_buffer,
            BREWPANEL_COMMUNICATION_MESSAGE_TOTAL_SIZE
        );

        //if there's no actual data, just empty the incoming message
        if (bytes_read == 0) {
            comm_handler->incoming_message = {0};
            return;
        }

        //otherwise, cast the data to the message
        comm_handler->incoming_message = *((BrewPanelCommunicationMessage*)serial_buffer);
    }
}