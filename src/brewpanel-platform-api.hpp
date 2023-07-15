#ifndef BREWPANEL_PLATFORM_API_HPP
#define BREWPANEL_PLATFORM_API_HPP

#include "brewpanel-types.hpp"

typedef mem_data
(*func_platform_memory_allocate)
(
    u64 size
);

typedef void
(*func_platform_memory_free)
(
    u64      size,
    mem_data memory
);

struct BrewpanelPlatformApi {
    func_platform_memory_allocate memory_allocate;
    func_platform_memory_free     memory_free;
};

global BrewpanelPlatformApi platform_api;

#define brewpanel_platform_memory_allocate(size)    platform_api.memory_allocate(size)
#define brewpanel_platform_memory_free(size,memory) platform_api.memory_free(size,memory)

#endif //BREWPANEL_PLATFORM_API_HPP
