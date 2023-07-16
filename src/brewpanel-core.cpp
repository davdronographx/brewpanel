#pragma once

#include "brewpanel-types.hpp"
#include "brewpanel-core.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-memory.hpp"

#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"

internal void
brewpanel_core_init() {

    BrewPanelMemory memory = brewpanel_memory_create();
    
    brewpanel_state = brewpanel_memory_allocate_struct(&memory,BrewPanelState);
    brewpanel_state->memory = memory;

    brewpanel_state->images = brewpanel_images_state_create(&memory);
}

internal void
brewpanel_core_update_and_render() {

    for (
        u32 pixel_index = 0;
        pixel_index < BREW_PANEL_PIXEL_COUNT;
        ++pixel_index
    ) {
        if (brewpanel_state->back_buffer.pixels[pixel_index].red == 0xFF) {
            brewpanel_state->back_buffer.pixels[pixel_index].red = 0x00;
        } else {
            brewpanel_state->back_buffer.pixels[pixel_index].red += 1;
        }

        brewpanel_state->back_buffer.pixels[pixel_index].green = 0x00; 
        brewpanel_state->back_buffer.pixels[pixel_index].blue  = 0xFF; 
        brewpanel_state->back_buffer.pixels[pixel_index].alpha = 0xFF; 
    }
}