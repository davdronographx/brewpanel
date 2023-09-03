#include <sdl2/include/SDL.h>
#include "brewpanel-sdl2-api.cpp"
#include "brewpanel-core.cpp"

#define SDL_main main

int main(int argc, char** argv) {

    //initialize the api
    platform_api = {0};
    platform_api.memory_allocate = brewpanel_sdl2_api_memory_allocate;
    platform_api.memory_free     = brewpanel_sdl2_api_free_memory;
    platform_api.file_open       = brewpanel_sdl2_api_file_open; 
    platform_api.file_get_size   = brewpanel_sdl2_api_file_size;
    platform_api.file_create     = brewpanel_sdl2_api_file_create;
    platform_api.file_close      = brewpanel_sdl2_api_file_close;
    platform_api.file_read       = brewpanel_sdl2_api_file_read;
    platform_api.file_write      = brewpanel_sdl2_api_file_write;
    platform_api.system_time_get = brewpanel_sdl2_api_system_time;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* sdl_window = SDL_CreateWindow(
        "BrewPanel",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        BREW_PANEL_WIDTH_PIXELS,
        BREW_PANEL_HEIGHT_PIXELS,
        SDL_WINDOW_RESIZABLE
    );

    SDL_SetWindowBordered(sdl_window, SDL_FALSE);

    SDL_Event sdl_event;

    bool running = true;

    brewpanel_core_init();
    brewpanel_assert(brewpanel_state);

    BrewPanelInput input = {0};

    while (running) {

        SDL_PollEvent(&sdl_event);

        if (brewpanel_core_update_and_render(&input)) {
            continue;
        }

    }

    return(0);
}