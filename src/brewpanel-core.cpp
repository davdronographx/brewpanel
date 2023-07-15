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

    for (
        u32 pixel_index = 0;
        pixel_index < BREW_PANEL_PIXEL_COUNT;
        ++pixel_index
    ) {
        brewpanel_state->back_buffer.pixels[pixel_index].red   = 0x28;  
        brewpanel_state->back_buffer.pixels[pixel_index].green = 0x28; 
        brewpanel_state->back_buffer.pixels[pixel_index].blue  = 0x28; 
        brewpanel_state->back_buffer.pixels[pixel_index].alpha = 0xFF; 
    }
}