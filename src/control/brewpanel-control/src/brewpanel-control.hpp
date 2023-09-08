#ifndef BREWPANEL_CONTROL_HPP
#define BREWPANEL_CONTROL_HPP

#include "brewpanel-control-types.hpp"
#include "brewpanel-control-communication.hpp"
#include "brewpanel-control-temperature.hpp"

struct BrewpanelControlState {
    comm_handler comm;
    temp_state   temp;
};

typedef BrewpanelControlState control_state;

#endif //BREWPANEL_CONTROL_HPP