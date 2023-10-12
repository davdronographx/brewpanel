#include <chrono>
#include <rs-232/rs232.h>
#include <rs-232/rs232.c>
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

internal brewpanel_file_handle
brewpanel_sdl2_api_file_open(
    str file_path) {

    SDL_RWops* file = SDL_RWFromFile(file_path,"r+");
    return(file);
}

internal u64
brewpanel_sdl2_api_file_size(
    brewpanel_file_handle file) {

    u64 file_size = file ? SDL_RWsize((SDL_RWops*)file) : 0;
    return(file_size);
}

internal brewpanel_file_handle
brewpanel_sdl2_api_file_create(
    str file_path) {

    SDL_RWops* file = SDL_RWFromFile(file_path,"w+");
    return(file);
}

internal void
brewpanel_sdl2_api_file_close(
    brewpanel_file_handle file) {

    SDL_RWclose((SDL_RWops*)file);
}

internal void
brewpanel_sdl2_api_file_read(
    brewpanel_file_handle file,
    mem_data              file_buffer,
    u64                   file_buffer_size,
    u64                   file_offset) {


    SDL_RWops* sdl_file = (SDL_RWops*)file;
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
    brewpanel_file_handle file,
    mem_data              write_buffer,
    u64                   write_buffer_size,
    u64                   file_offset) {
    
    SDL_RWops* sdl_file = (SDL_RWops*)file;
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
    auto system_time_now  = std::chrono::system_clock::to_time_t(system_clock_now);
    auto local_time       = *localtime(&system_time_now);
    
    bp_system_time.hours   = local_time.tm_hour;
    bp_system_time.minutes = local_time.tm_min;
    bp_system_time.seconds = local_time.tm_sec;

    return(bp_system_time);
}

internal controller_handle
brewpanel_sdl2_api_controller_handle() {
    //TODO
    return(NULL);
}

internal void
brewpanel_sdl2_api_controller_close(
    controller_handle controller) {
    //TODO
}

internal void
brewpanel_sdl2_api_controller_write_buffer(
    controller_handle controller,
    mem_data          write_buffer,
    u64               write_buffer_size) {

}

internal void
brewpanel_sdl2_api_controller_start_read_thread(
    BrewPanelControlCommData* controller_comm_data) {
    //TODO
    return(NULL);
}