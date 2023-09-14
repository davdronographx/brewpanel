#include "brewpanel-communication.hpp"
#include <time.h>
#include <string.h>

#pragma once

internal void
brewpanel_communication_controller_read_callback(
    mem_data comm_handler_payload) {

    brewpanel_assert(comm_handler_payload);
    comm_handler* comm = (comm_handler*)comm_handler_payload;

    memset(
        (mem_data)comm->comm_data.write_buffer,
        0,
        BREWPANEL_CONTROL_COMM_DATA_BUFFER_SIZE
    );

    brewpanel_nop();
}

internal void
brewpanel_communication_create_handler(
    BrewPanelCommunicationHandler* comm_handler,
    BrewPanelControllerInfo        controller_info) {

    *comm_handler = {0};
    comm_handler->controller_info              = controller_info;
    comm_handler->read_thread                  = brewpanel_platform_controller_thread_start_read(&comm_handler->comm_data);
    comm_handler->write_thread                 = brewpanel_platform_controller_thread_start_write(&comm_handler->comm_data);
    comm_handler->comm_data.panel_comm_handler = (mem_data)&comm_handler;
    comm_handler->comm_data.read_callback      = brewpanel_communication_controller_read_callback;
}

internal bool
brewpanel_communication_message_queue_add(
    comm_handler*                 comm_handler,
    BrewPanelCommunicationMessage message) {

    if (comm_handler->outgoing_message_queue.message_count < BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES) {

        comm_handler->outgoing_message_queue.messages[comm_handler->outgoing_message_queue.message_count] = message;
        ++comm_handler->outgoing_message_queue.message_count;

        return(true);
    }

    return(false);
}

internal void
brewpanel_communication_message_heartbeat_build(
    BrewPanelCommunicationMessage* message) {

    *message = {0};

    message->header.sender       = BREWPANEL_COMMUNICATION_MESSAGE_SENDER_HMI;
    message->header.message_type = BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT;
    message->header.message_size = sizeof(BrewPanelCommunicationMessageHeader) + 1;
    message->header.timestamp    = (u64)time(NULL);
    message->payload_data[0]     = '\0';
    message->payload_size_bytes  = 1;
}

internal void
brewpanel_communication_message_buffer_build(
    BrewPanelCommunicationMessage        message,
    BrewPanelCommunicationMessageBuffer* message_buffer) {

    *message_buffer = {0};
    
    memmove(
        message_buffer->buffer,
        &message.header,
        sizeof(BrewPanelCommunicationMessageHeader)
    );

    memmove(
        &message_buffer->buffer[sizeof(BrewPanelCommunicationMessageHeader)],
        message.payload_data,
        message.payload_size_bytes
    );

    message_buffer->buffer_size = sizeof(BrewPanelCommunicationMessageHeader) + message.payload_size_bytes;
}

internal void
brewpanel_communication_update(
    comm_handler* comm_handler) {


    //establish communication with the controller
    if (comm_handler->comm_data.controller == NULL) {

        comm_handler->comm_data.controller = brewpanel_platform_controller_handle(comm_handler->controller_info);
        
        if (comm_handler->comm_data.controller == NULL) {
            return;
        }
    }

    //start by building the heartbeat message
    //it will be the last message we request to get the latest
    //data from the controller after all user requests
    BrewPanelCommunicationMessageBuffer outgoing_message_buffer = {0};
    BrewPanelCommunicationMessageBuffer incoming_message_buffer = {0};
    BrewPanelCommunicationMessage       heartbeat_message       = {0};

    brewpanel_communication_message_heartbeat_build(&heartbeat_message);
    brewpanel_communication_message_queue_add(comm_handler,heartbeat_message);

    for (
        u32 message_index = 0;
        message_index < comm_handler->outgoing_message_queue.message_count;
        ++message_index
    ) {

        // build the outgoing message
        brewpanel_communication_message_buffer_build(
            comm_handler->outgoing_message_queue.messages[message_index],
            &outgoing_message_buffer
        );

        comm_handler->comm_data.bytes_to_write = outgoing_message_buffer.buffer_size;
        memmove(
            comm_handler->comm_data.write_buffer,
            outgoing_message_buffer.buffer,
            outgoing_message_buffer.buffer_size
        );
    }
}