#include <sdl2/include/SDL.h>
#include <iostream>
#include <string>

#include "brewpanel-sdl2-api.cpp"
#include "brewpanel-core.cpp"

#define SDL_main main

SDL_Window*     sdl_window;
SDL_Renderer*   sdl_renderer;
SDL_Texture*    sdl_texture;
BrewPanelInput  input;

internal void
brewpanel_sdl2_draw_bitmap() {

    mem_data bitmap_data = brewpanel_back_buffer_data();

    SDL_UpdateTexture(
        sdl_texture,
        0,
        (void*)&brewpanel_state->back_buffer.pixels,
        BREW_PANEL_WIDTH_PIXELS * 4
    );

    u32 flip = SDL_FLIP_VERTICAL;

    SDL_RenderCopyEx(
        sdl_renderer,
        sdl_texture,
        0,
        0,
        0,
        NULL,
        (SDL_RendererFlip)flip
    );

    SDL_RenderPresent(sdl_renderer);

}

internal void
brewpanel_sdl2_resize_bitmap() {
    
    if (sdl_texture) {
        SDL_DestroyTexture(sdl_texture);
    }
    sdl_texture = SDL_CreateTexture(
        sdl_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        BREW_PANEL_WIDTH_PIXELS,
        BREW_PANEL_HEIGHT_PIXELS
    );
}

internal void
brewpanel_sd2_poll_events(
    SDL_Event* sdl_event) {

    if (SDL_PollEvent(sdl_event)) {

        switch (sdl_event->type) {

            case SDL_MOUSEMOTION: {
                
                SDL_GetMouseState(
                    (s32*)&input.mouse_x_pos,
                    (s32*)&input.mouse_y_pos
                );

                input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - input.mouse_y_pos;

            } break;

            case SDL_MOUSEBUTTONDOWN: {

                input.click = true;

                SDL_GetMouseState(
                    (s32*)&input.mouse_x_pos,
                    (s32*)&input.mouse_y_pos
                );

                input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - input.mouse_y_pos;

            } break;

            case SDL_MOUSEBUTTONUP: {

                input.click = false;

                SDL_GetMouseState(
                    (s32*)&input.mouse_x_pos,
                    (s32*)&input.mouse_y_pos
                );

                input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - input.mouse_y_pos;

            } break;

            case SDL_WINDOWEVENT: {

                switch (sdl_event->window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        brewpanel_sdl2_resize_bitmap();
                    } break;
                }

            } break;

        }
    }
}

int main(int argc, char** argv) {


    brewpanel_assert(argc == 4);

    BrewPanelControllerInfo controller_info = {0};
    controller_info.vendor_id     = argv[1];
    controller_info.product_id    = argv[2];
    controller_info.serial_number = argv[3];

    //initialize the api
    platform_api = {0};
    platform_api.memory_allocate   = brewpanel_sdl2_api_memory_allocate;
    platform_api.memory_free       = brewpanel_sdl2_api_free_memory;
    platform_api.file_open         = brewpanel_sdl2_api_file_open; 
    platform_api.file_get_size     = brewpanel_sdl2_api_file_size;
    platform_api.file_create       = brewpanel_sdl2_api_file_create;
    platform_api.file_close        = brewpanel_sdl2_api_file_close;
    platform_api.file_read         = brewpanel_sdl2_api_file_read;
    platform_api.file_write        = brewpanel_sdl2_api_file_write;
    platform_api.system_time_get   = brewpanel_sdl2_api_system_time;
    platform_api.controller_handle = brewpanel_sdl2_api_controller_handle;

    SDL_Init(SDL_INIT_EVERYTHING);

    sdl_window = SDL_CreateWindow(
        "BrewPanel",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        BREW_PANEL_WIDTH_PIXELS,
        BREW_PANEL_HEIGHT_PIXELS,
        SDL_WINDOW_RESIZABLE
    );

    sdl_renderer = SDL_CreateRenderer(
        sdl_window,-1,0
    );

    SDL_SetWindowBordered(sdl_window, SDL_FALSE);

    SDL_Event sdl_event;

    bool running = true;

    brewpanel_core_init(controller_info);
    brewpanel_assert(brewpanel_state);

    input = {0};

    brewpanel_sdl2_resize_bitmap();

    while (running) {

        brewpanel_sd2_poll_events(&sdl_event);
        
        if (brewpanel_core_update_and_render(&input)) {
            brewpanel_sdl2_draw_bitmap();
        }

    }

    return(0);
}