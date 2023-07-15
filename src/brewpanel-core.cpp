#pragma once

#include "brewpanel-types.hpp"
#include "brewpanel-core.hpp"
#include "brewpanel-memory.hpp"
#include "brewpanel-memory.cpp"

internal BrewPanelState*
brewpanel_core_create_state() {

    BrewPanelMemory memory = brewpanel_memory_create();
    
    BrewPanelState* brewpanel_state = brewpanel_memory_allocate_struct(&memory,BrewPanelState);
    brewpanel_state->memory = memory;

    return(brewpanel_state);
}

internal void
brewpanel_core_update_and_render(
    BrewPanelState* brewpanel_state) {
    
}