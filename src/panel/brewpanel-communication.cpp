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
brewpanel_communication_update(
    comm_handler* comm_handler) {

    if (comm_handler->controller_handle == NULL) {

        comm_handler->controller_handle = brewpanel_platform_controller_handle(comm_handler->controller_info);
        
        if (comm_handler->controller_handle == NULL) {
            brewpanel_nop();
            return;
        }
    }

    

}