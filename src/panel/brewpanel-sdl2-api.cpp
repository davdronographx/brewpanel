#include <chrono>

#include "brewpanel-platform-api.hpp"
#include "brewpanel-types.hpp"


internal mem_data
brewpanel_sdl2_api_memory_allocate(u64 size) {
    
    //TODO: we're using malloc here, but we may be able to do something for linux
    mem_data memory = (mem_data)malloc(size);
    return(memory);
}

internal void
brewpanel_sdl2_api_free_memory(
    u64      size,
    mem_data memory) {
    
    free(memory);
}

internal file_handle
brewpanel_sdl2_api_file_open(
    str file_path) {

    SDL_RWops* file_handle = SDL_RWFromFile(file_path,"r+");
    return(file_handle);
}

internal u64
brewpanel_sdl2_api_file_size(
    file_handle file) {

    u64 file_size = file ? SDL_RWsize((SDL_RWops*)file) : 0;
    return(file_size);
}

internal file_handle
brewpanel_sdl2_api_file_create(
    str file_path) {

    SDL_RWops* file_handle = SDL_RWFromFile(file_path,"w+");
    return(file_handle);
}

internal void
brewpanel_sdl2_api_file_close(
    file_handle file) {

    SDL_RWclose((SDL_RWops*)file);
}

internal void
brewpanel_sdl2_api_file_read(
    file_handle file_handle,
    mem_data    file_buffer,
    u64         file_buffer_size,
    u64         file_offset) {


    SDL_RWops* sdl_file = (SDL_RWops*)file_handle;
    if (sdl_file) {
        
        //offset the next operation
        SDL_RWseek(
            sdl_file,
            file_offset,
            RW_SEEK_SET
        );

        //read the data
        SDL_RWread(
            sdl_file,
            file_buffer,
            file_buffer_size,
            1
        );
    }
}

internal void
brewpanel_sdl2_api_file_write(
    file_handle file_handle,
    mem_data    write_buffer,
    u64         write_buffer_size,
    u64         file_offset) {
    
    SDL_RWops* sdl_file = (SDL_RWops*)file_handle;
    if (sdl_file) {

        //offset the next operation
        SDL_RWseek(
            sdl_file,
            file_offset,
            RW_SEEK_SET
        );

        //write the datas
        SDL_RWwrite(
            sdl_file,
            write_buffer,
            write_buffer_size,
            1
        );
    }
}

internal BrewPanelSystemTime
brewpanel_sdl2_api_system_time() {

    BrewPanelSystemTime bp_system_time = {0};

    auto system_clock_now = std::chrono::system_clock::now();
    auto system_time_now = std::chrono::system_clock::to_time_t(system_clock_now);
    auto local_time = *localtime(&system_time_now);
    
    bp_system_time.hours   = local_time.tm_hour;
    bp_system_time.minutes = local_time.tm_min;
    bp_system_time.seconds = local_time.tm_sec;

    return(bp_system_time);
}