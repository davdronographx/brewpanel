#include <Arduino.h>
#include "brewpanel-control.hpp"
#include "brewpanel-control-communication.hpp"
#include "brewpanel-control-communication.cpp"

BrewpanelControlState control_state = {0};

void setup() {

    control_state.comm_handler = brewpanel_control_communication_init();

}

void loop() {

    brewpanel_control_communication_read(&control_state.comm_handler);
}