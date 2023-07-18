#ifndef BREWPANEL_MEMORY_HPP
#define BREWPANEL_MEMORY_HPP

#include "brewpanel-types.hpp"

#define BREWPANEL_MEMORY_TOTAL brewpanel_megabytes(128)

struct BrewPanelMemory {
    s32 total;
    s32 used;
    mem_data core_memory;
};

#define brewpanel_memory_allocate_struct(mem,new_struct) (new_struct*)brewpanel_memory_allocate(mem,sizeof(new_struct)) 

#endif //BREWPANEL_MEMORY_HPP