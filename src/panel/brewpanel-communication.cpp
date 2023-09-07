#include "brewpanel-communication.hpp"

#pragma once

internal comm_handler
brewpanel_communication_create_handler(
    BrewPanelControllerInfo controller_info
) {

    comm_handler handler = {0};
    handler.controller_info = controller_info;

    return(handler);
}

internal void
brewpanel_communication_message_heartbeat_build(
    comm_handler* comm_handler) {

    comm_handler->outgoing_message = {0};
    comm_handler->outgoing_message.header.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT;
    comm_handler->outgoing_message.payload_data[0]     = '\0';
    comm_handler->outgoing_message.payload_size_bytes  = 1;
}

internal void
brewpanel_communication_message_buffer_build(
    BrewPanelCommunicationMessage        message,
    BrewPanelCommunicationMessageBuffer* message_buffer) {

    

}

internal void
brewpanel_communication_update(
    comm_handler* comm_handler) {

    BrewPanelCommunicationMessageBuffer outgoing_message_buffer;
    BrewPanelCommunicationMessageBuffer incoming_message_buffer;

    if (comm_handler->controller_handle == NULL) {

        comm_handler->controller_handle = brewpanel_platform_controller_handle(comm_handler->controller_info);
        
        if (comm_handler->controller_handle == NULL) {
            brewpanel_nop();
            return;
        }
    }

    for (
        u32 message_index = 0;
        message_index < comm_handler->outgoing_message_queue.message_count;
        ++message_index
    ) {

        //build the outgoing message
        brewpanel_communication_message_buffer_build(
            comm_handler->outgoing_message_queue.messages[message_index],
            &outgoing_message_buffer
        );

        //send the message
        if (!brewpanel_platform_controller_write(
            comm_handler->controller_handle,
            outgoing_message_buffer.buffer,
            outgoing_message_buffer.buffer_size
        )) {
            brewpanel_platform_controller_close(comm_handler->controller_handle);
            comm_handler->controller_handle = NULL;
            break;  
        }

        //get any replies
        u64 bytes_read = 0;
        brewpanel_platform_controller_read(
            comm_handler->controller_handle,
            incoming_message_buffer.buffer,
            incoming_message_buffer.buffer_size,
            &bytes_read
        );
    }
}