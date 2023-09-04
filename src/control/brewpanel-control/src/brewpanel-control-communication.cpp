#include "brewpanel-control-types.hpp"
#include "brewpanel-control-communication.hpp"

internal BrewPanelCommunicationHandler
brewpanel_communication_init() {

    BrewPanelCommunicationHandler comm_handler = {0};

    Serial.begin(BREWPANEL_CONTROL_COMMUNICATION_BAUD);

    return(comm_handler);
}