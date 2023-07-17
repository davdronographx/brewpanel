#pragma once

#include "brewpanel-types.hpp"
#include "brewpanel-core.hpp"
#include "brewpanel-images.hpp"
#include "brewpanel-memory.hpp"

#include "brewpanel-memory.cpp"
#include "brewpanel-images.cpp"

internal void
brewpanel_core_init() {

    //allocate memory for the state
    BrewPanelMemory memory = brewpanel_memory_create();
    brewpanel_state = brewpanel_memory_allocate_struct(&memory,BrewPanelState);
    brewpanel_state->memory = memory;

    //get the images
    brewpanel_state->images = brewpanel_images_state_create(&memory);
}

internal void
brewpanel_core_update_and_render() {
    
    brewpanel_core_render_main_screen()
}