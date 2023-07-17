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

    memmove(
        brewpanel_state->back_buffer.pixels,
        brewpanel_state->images.images_file.image_data,
        BREW_PANEL_PIXEL_COUNT
    );
}