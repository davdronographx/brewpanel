#include "brewpanel-communication.hpp"
#include <time.h>
#include <string.h>

#pragma once

internal void
brewpanel_communication_send_message_pump_control(
    BrewPanelCommunicationHandler*   comm,
    BrewPanelCommunicationPumpId     pump_id,
    BrewPanelCommunicationPumpStatus pump_status) {

    brewpanel_nop();
}

internal bool
brewpanel_communication_message_queue_push(
    BrewPanelCommunicationMessageQueue* queue,
    BrewPanelCommunicationMessage       message) {

    if (queue->message_count < BREWPANEL_COMMUNICATION_MESSAGE_QUEUE_MAX_MESSAGES) {

        queue->messages[queue->message_count] = message;
        ++queue->message_count;

        return(true);
    }

    return(false);
}


internal void
brewpanel_communication_controller_read_callback(
    mem_data comm_handler_payload) {

    brewpanel_assert(comm_handler_payload);
    comm_handler* comm = (comm_handler*)comm_handler_payload;

    //wait until we can add to the read buffer
    while (comm->comm_data.read_buffer_lock) {};

    //lock the read buffer
    comm->comm_data.read_buffer_lock = true;

    //first, if the buffer size is greater than the allocation space, we need to fix that
    u32 bytes_to_copy = 
        (comm->comm_data.bytes_read > BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE)
        ? BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE 
        : comm->comm_data.bytes_read; 

    //calculate how much of the original buffer we are preserving
    u32 buffer_copy_length = 
        (bytes_to_copy + comm->incoming_data_buffer.buffer_size > BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE) 
        ? BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE - bytes_to_copy
        : comm->incoming_data_buffer.buffer_size;

    //ensure our new buffer data will fit
    brewpanel_assert(buffer_copy_length + bytes_to_copy <= BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE);

    //if the new message data is overwriting the entire buffer, we don't need to preseve anything
    if (bytes_to_copy < BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE) {
        memmove(
            &comm->incoming_data_buffer.buffer[bytes_to_copy],  //the size of the bytes to copy is also the index of where we shift
            comm->incoming_data_buffer.buffer,                  //we shift starting at the beginning
            buffer_copy_length                                  //how much we are preserving
        );
    }
    //copy the new message to the read buffer
    memmove(
        comm->incoming_data_buffer.buffer,
        comm->comm_data.read_buffer,
        bytes_to_copy
    );

    comm->incoming_data_buffer.buffer_size = buffer_copy_length + bytes_to_copy; 

    //unlock the read buffer
    comm->comm_data.read_buffer_lock = false;
    memset(
        comm->comm_data.read_buffer,
        0,
        BREWPANEL_COMMUNICATION_MESSAGE_BUFFER_SIZE
    );
    comm->comm_data.bytes_read = 0;
}

internal void
brewpanel_communication_create_handler(
    BrewPanelCommunicationHandler* comm_handler,
    BrewPanelControllerInfo        controller_info,
    images_store*                  images
    ) {

    *comm_handler = {0};
    comm_handler->controller_info              = controller_info;
    comm_handler->read_thread                  = brewpanel_platform_controller_thread_start_read(&comm_handler->comm_data);
    comm_handler->write_thread                 = brewpanel_platform_controller_thread_start_write(&comm_handler->comm_data);
    comm_handler->comm_data.panel_comm_handler = (mem_data)comm_handler;
    comm_handler->comm_data.read_callback      = brewpanel_communication_controller_read_callback;
    comm_handler->redraw                       = true;  
    comm_handler->controller_status_panel      = brewpanel_images_create_image_instance(
        images,
        BREWPANEL_IMAGES_ID_CONTROLLER_PANEL,
        BREWPANEL_COMMUNICATION_PANEL_X_OFFSET,
        BREWPANEL_COMMUNICATION_PANEL_Y_OFFSET
    );
    comm_handler->controller_disconnected_label = brewpanel_images_create_image_instance(
        images,
        BREWPANEL_IMAGES_ID_CONTROLLER_DISCONNECTED,
        BREWPANEL_COMMUNICATION_LABEL_X_OFFSET,
        BREWPANEL_COMMUNICATION_LABEL_Y_OFFSET
    );
    comm_handler->controller_connected_label = brewpanel_images_create_image_instance(
        images,
        BREWPANEL_IMAGES_ID_CONTROLLER_CONNECTED,
        BREWPANEL_COMMUNICATION_LABEL_X_OFFSET,
        BREWPANEL_COMMUNICATION_LABEL_Y_OFFSET
    );

    comm_handler->controller_status = BREWPANEL_COMMUNICATION_CONTROLLER_STATUS_NOT_CONNECTED;
}

internal void
brewpanel_communication_message_buffer_build(
    BrewPanelCommunicationMessage        message,
    BrewPanelCommunicationMessageBuffer* message_buffer) {

    *message_buffer = {0};
    
    message_buffer->buffer_size = comm_message_size(message.header.message_type);

    memmove(
        message_buffer->buffer,
        &message,
        message_buffer->buffer_size
    );
}

internal void
brewpanel_communication_handle_messages_incoming(
    comm_handler* comm) {

    //first check our incoming messages
    BrewPanelCommunicationMessage incoming_message = {0};
    
    for (
        u32 message_index = 0;
        message_index < comm->incoming_message_queue.message_count;
        ++message_index
    ) {
        incoming_message = comm->incoming_message_queue.messages[message_index];

        switch (incoming_message.header.message_type) {

            case BREWPANEL_COMMUNICATION_MESSAGE_TYPE_HEARTBEAT_ACK: {

                comm->latest_heartbeat.hlt_element_temp  = incoming_message.payload.heartbeat_ack.hlt_element_temp;
                comm->latest_heartbeat.mlt_element_temp  = incoming_message.payload.heartbeat_ack.mlt_element_temp;
                comm->latest_heartbeat.boil_element_temp = incoming_message.payload.heartbeat_ack.boil_element_temp;

            } break;
        }


        //TODO: handle message ACKs
        brewpanel_nop();
    }

    comm->incoming_message_queue = {0};
}

internal void
brewpanel_communication_parse_incoming_message_buffer(
    comm_handler* comm_handler) {

    //wait until we can remove from the read buffer
    while (comm_handler->comm_data.read_buffer_lock) {
        brewpanel_nop();
    }

    comm_handler->comm_data.read_buffer_lock = true;

    bool message_start = false;
    bool message_end = false;

    mem_byte message_start_buffer[8] = {0};
    mem_byte message_end_buffer[8]   = {0};

    BrewPanelCommunicationMessageBuffer message_buffer = {0};


    for (
        u32 buffer_index = 0;
        buffer_index < comm_handler->incoming_data_buffer.buffer_size;
        ++buffer_index
    ) {
        //get the next byte
        mem_byte byte = comm_handler->incoming_data_buffer.buffer[buffer_index];

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

            brewpanel_nop();

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
    
            BrewPanelCommunicationMessageHeader* header = (BrewPanelCommunicationMessageHeader*)message_buffer.buffer; 
            BrewPanelCommunicationMessage message = *(BrewPanelCommunicationMessage*)message_buffer.buffer; 

            brewpanel_communication_message_queue_push(&comm_handler->incoming_message_queue,message);

            message_start = false;
            message_end = false;
            message_buffer = {0};
        }

    }

    //clear the incoming data buffer
    memset(
        comm_handler->incoming_data_buffer.buffer,
        0,
        comm_handler->incoming_data_buffer.buffer_size
    );
    comm_handler->incoming_data_buffer.buffer_size = 0;

    comm_handler->comm_data.read_buffer_lock = false;
}

internal bool
brewpanel_communication_update(
    comm_handler* comm_handler,
    images_store* images) {

    BrewPanelCommunicationControllerStatus previous_controller_status = BREWPANEL_COMMUNICATION_CONTROLLER_STATUS_NOT_CONNECTED;

    if (comm_handler->redraw) {
        brewpanel_images_draw_image_instance(
            images,
            comm_handler->controller_status_panel
        );

        if (comm_handler->controller_status == BREWPANEL_COMMUNICATION_CONTROLLER_STATUS_CONNECTED) {

            brewpanel_images_draw_image_instance(
                images,
                comm_handler->controller_connected_label
            );
        } 
        else {

            brewpanel_images_draw_image_instance(
                images,
                comm_handler->controller_disconnected_label
            );
        }
    }

    if (comm_handler->controller_status != previous_controller_status) {
        
        comm_handler->redraw = true;
    
        previous_controller_status = comm_handler->controller_status;
    } 

    //establish communication with the controller
    if (comm_handler->comm_data.controller == NULL) {

        comm_handler->comm_data.controller = brewpanel_platform_controller_handle(comm_handler->controller_info);
        
        if (comm_handler->comm_data.controller == NULL) {
            comm_handler->controller_status = BREWPANEL_COMMUNICATION_CONTROLLER_STATUS_NOT_CONNECTED;
            return(comm_handler->redraw);
        }
    }
    comm_handler->controller_status = BREWPANEL_COMMUNICATION_CONTROLLER_STATUS_CONNECTED;


    //parse the incoming message buffer
    brewpanel_communication_parse_incoming_message_buffer(comm_handler);

    //handle incoming messages
    brewpanel_communication_handle_messages_incoming(comm_handler);

    return(comm_handler->redraw);
}