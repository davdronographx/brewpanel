#include "brewpanel-communication.hpp"
#include "brewpanel-temp-control.hpp"
#include <time.h>
#include <string.h>

#pragma once

internal bool
brewpanel_communication_message_queue_push(
    comm_handler*                 comm_handler,
    BrewPanelCommunicationMessage message) {

    if (comm_handler->outgoing_message_queue.message_count < BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES) {

        comm_handler->outgoing_message_queue.messages[comm_handler->outgoing_message_queue.message_count] = message;
        ++comm_handler->outgoing_message_queue.message_count;

        return(true);
    }

    return(false);
}

internal bool
brewpanel_communication_message_queue_pop(
    comm_handler*                  comm,
    BrewPanelCommunicationMessage* dequeued_message) {

    BrewPanelCommunicationMessageQueue* message_queue = &comm->outgoing_message_queue;

    if (message_queue->message_count == 0) {
        dequeued_message = NULL;
        return(false);
    }

    *dequeued_message = message_queue->messages[0];
    --message_queue->message_count;

    for (
        u32 message_index = 0;
        message_index < message_queue->message_count;
        ++message_index
    ) {
        message_queue->messages[message_index] = message_queue->messages[message_index+1];
        message_queue[message_index+1] = {0};
    }

    return(true);
}

internal void
brewpanel_communication_controller_read_callback(
    mem_data comm_handler_payload) {

    brewpanel_assert(comm_handler_payload);
    comm_handler* comm = (comm_handler*)comm_handler_payload;

    //if we have an appropriately sized message
    //we will push it onto the incoming message queue
    if (comm->comm_data.bytes_read > sizeof(BrewPanelCommunicationMessageHeader) &&
        comm->comm_data.bytes_read <= BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE) {
        
        BrewPanelCommunicationMessage incoming_message = {0};
        
        //copy the header
        memmove(
            (mem_data)&incoming_message.header,
            comm->comm_data.read_buffer,
            sizeof(BrewPanelCommunicationMessageHeader)
        );

        //copy the payload
        memmove(
            incoming_message.payload_data,
            &comm->comm_data.read_buffer[sizeof(BrewPanelCommunicationMessageHeader)],
            comm->comm_data.bytes_read - sizeof(BrewPanelCommunicationMessageHeader)
        );

        //push the message onto the queue
        brewpanel_communication_message_queue_push(
            comm,
            incoming_message
        );
    }

    //reset the buffers
    memset(
        (mem_data)comm->comm_data.read_buffer,
        0,
        BREWPANEL_CONTROL_COMM_DATA_BUFFER_SIZE
    );

    memset(
        (mem_data)comm->comm_data.write_buffer,
        0,
        BREWPANEL_CONTROL_COMM_DATA_BUFFER_SIZE
    );
    comm->comm_data.bytes_read     = 0;
    comm->comm_data.bytes_written  = 0;
}

internal void
brewpanel_communication_create_handler(
    BrewPanelCommunicationHandler* comm_handler,
    BrewPanelControllerInfo        controller_info) {

    *comm_handler = {0};
    comm_handler->controller_info              = controller_info;
    comm_handler->read_thread                  = brewpanel_platform_controller_thread_start_read(&comm_handler->comm_data);
    comm_handler->write_thread                 = brewpanel_platform_controller_thread_start_write(&comm_handler->comm_data);
    comm_handler->comm_data.panel_comm_handler = (mem_data)comm_handler;
    comm_handler->comm_data.read_callback      = brewpanel_communication_controller_read_callback;
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
brewpanel_communication_handle_messages_incoming(
    comm_handler* comm,
    temp_control* temp) {

    //first check our incoming messages
    BrewPanelCommunicationMessage incoming_message = {0};
    while (brewpanel_communication_message_queue_pop(comm,&incoming_message)) {

        switch (incoming_message.header.message_type) {
            case BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK: {

                BrewPanelCommunicationMessagePayloadHeartBeatAck heartbeat;
                heartbeat = *((BrewPanelCommunicationMessagePayloadHeartBeatAck*)incoming_message.payload_data);

                temp->hlt_temp_panel.values.value  = heartbeat.hlt_element_temp;
                temp->mlt_temp_panel.values.value  = heartbeat.mlt_element_temp;
                temp->boil_temp_panel.values.value = heartbeat.boil_element_temp;

                brewpanel_nop();
            } break;
        }

        //TODO: handle message ACKs
        brewpanel_nop();
    }
}

internal void
brewpanel_communication_handle_messages_outgoing(
    comm_handler* comm) {

    BrewPanelCommunicationMessageBuffer outgoing_message_buffer = {0};
    BrewPanelCommunicationMessageBuffer incoming_message_buffer = {0};

    BrewPanelCommunicationMessage outgoing_message = {0};
    while (brewpanel_communication_message_queue_pop(comm,&outgoing_message)) {
        
        //wait till the buffer is free
        // while (comm->comm_data.bytes_to_write > 0) {
        //     brewpanel_nop();
        // }

        // build the outgoing message
        brewpanel_communication_message_buffer_build(
            outgoing_message,
            &outgoing_message_buffer
        );

        //write the message to the outgoing message buffer
        //the write thread will pick it up and clear it once its sent to the controller
        comm->comm_data.bytes_to_write = outgoing_message_buffer.buffer_size;
        memmove(
            comm->comm_data.write_buffer,
            outgoing_message_buffer.buffer,
            outgoing_message_buffer.buffer_size
        );
    }
}

internal void
brewpanel_communication_update(
    comm_handler* comm_handler,
    temp_control* temp
    ) {

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
    BrewPanelCommunicationMessage heartbeat_message = {0};
    // brewpanel_communication_message_heartbeat_build(&heartbeat_message);
    // brewpanel_communication_message_queue_push(comm_handler,heartbeat_message);

    // //handle outgoing messages
    // brewpanel_communication_handle_messages_outgoing(comm_handler);

    //handle incoming messages
    brewpanel_communication_handle_messages_incoming(comm_handler,temp);

    

}