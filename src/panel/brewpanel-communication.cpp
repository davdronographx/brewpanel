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
brewpanel_communication_update(
    comm_handler* comm_handler) {

    if (comm_handler->controller_handle == NULL) {

        comm_handler->controller_handle = brewpanel_platform_controller_handle(comm_handler->controller_info);
        
        if (comm_handler->controller_handle == NULL) {
            brewpanel_nop();
            return;
        }
    }

    char test_buffer[5];
    test_buffer[0] = 'T';
    test_buffer[1] = 'E';
    test_buffer[2] = 'S';
    test_buffer[3] = 'T';
    test_buffer[4] = '\0';

    brewpanel_platform_controller_write(
        comm_handler->controller_handle,
        (mem_data)test_buffer,
        5
    );

}