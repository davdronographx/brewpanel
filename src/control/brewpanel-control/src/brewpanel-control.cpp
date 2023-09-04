#include <Arduino.h>
#include "brewpanel-control.hpp"
#include "brewpanel-control-communication.hpp"
#include "brewpanel-control-communication.cpp"

int main(void) {
    
    BrewpanelControlState control_state = {0};
    control_state.comm_handler = brewpanel_control_communication_init();

    while(true) {

        brewpanel_control_communication_read(&control_state.comm_handler);

    }

    return(0);
}