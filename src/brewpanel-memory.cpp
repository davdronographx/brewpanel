#pragma once

#include "brewpanel-memory.hpp"
#include "brewpanel-platform-api.hpp"

//This is a very simple 'churn and burn' allocator
//we have a set size that we know we should never exceed,
//but if we do, we crash and readjust

internal BrewPanelMemory
brewpanel_memory_create() {

    BrewPanelMemory memory = {0};
    memory.total = BREWPANEL_MEMORY_TOTAL;
    memory.used  = 0; 
    memory.core_memory = brewpanel_platform_memory_allocate(BREWPANEL_MEMORY_TOTAL);

    brewpanel_assert(memory.core_memory != NULL);

    return(memory);
}

internal mem_data
brewpanel_memory_allocate(
    BrewPanelMemory* memory,
    u64 size
) {

    //make sure we have space for the allocation
    brewpanel_assert((memory->used + size) <= memory->total);

    mem_data new_memory = (mem_data)((u8*)memory->core_memory + memory->used);

    memory->used += size;

    return(new_memory);
}