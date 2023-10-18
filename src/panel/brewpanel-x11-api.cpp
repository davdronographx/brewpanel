#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include "brewpanel-types.hpp"

internal mem_data
brewpanel_x11_api_memory_allocate(
    u64 size) {
    
    //TODO: we're using malloc here, but we may be able to do something for linux
    mem_data memory = (mem_data)malloc(size);
    return(memory);
}


internal void
brewpanel_x11_api_memory_free(
    u64      size,
    mem_data memory) {

    free(memory);
}

internal brewpanel_file_handle
brewpanel_x11_api_file_open(
    str file_path) {

    //on linux, the file handle is an int
    //TODO: I HATE THIS, it would be better if we passed the handle into the arguments
    //but I'm lazy and want my damn beer
    s32* file_descriptor = (s32*)malloc(sizeof(s32));

    *file_descriptor = open(file_path,O_RDWR | O_TRUNC);

    if (*file_descriptor == -1) {
        free(file_descriptor);
        return(NULL);
    }

    return((brewpanel_file_handle)file_descriptor);
}

internal u64
brewpanel_x11_api_file_get_size(
    brewpanel_file_handle file) {

    s32 file_descriptor = *((s32*)file);

    u64 size = lseek(file_descriptor,0,SEEK_END);

    return(size);
}

internal brewpanel_file_handle
brewpanel_x11_api_file_create(
    str file_path) {

    //on linux, the file handle is an int
    //TODO: I HATE THIS, it would be better if we passed the handle into the arguments
    //but I'm lazy and want my damn beer
    mode_t mode = ALLPERMS;

    s32* file_descriptor = (s32*)malloc(sizeof(s32));

    *file_descriptor = open(file_path,O_RDWR | O_CREAT | O_TRUNC,mode);

    return((brewpanel_file_handle)file_descriptor);
}

internal void
brewpanel_x11_api_file_close(
    brewpanel_file_handle file) {
    
    s32 file_descriptor = *((s32*)file);
    
    close(file_descriptor);

    free((s32*)file);
}

internal void
brewpanel_x11_api_file_read(
    brewpanel_file_handle file,
    mem_data              read_buffer,
    u64                   read_buffer_size,
    u64                   file_offset) {

    s32 file_descriptor = *(s32*)file;

    pread(
        file_descriptor,
        read_buffer,
        read_buffer_size,
        file_offset
    );
}

internal void
brewpanel_x11_api_file_write(
    brewpanel_file_handle file,
    mem_data              write_buffer,
    u64                   write_buffer_size,
    u64                   file_offset) {

    s32 file_descriptor = *(s32*)file;

    pwrite(
        file_descriptor,
        write_buffer,
        write_buffer_size,
        file_offset        
    );
}

internal BrewPanelSystemTime
brewpanel_x11_api_system_time(){
    
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
brewpanel_x11_api_controller_handle(
    BrewPanelControllerInfo controller_info) {

    //TODO
    return(NULL);
}

internal void
brewpanel_x11_api_controller_close(
    controller_handle handle) {
    
    //TODO
}

internal void
brewpanel_x11_api_controller_write_buffer(
    controller_handle controller_handle,
    mem_data          write_buffer,
    u64               write_buffer_size) {

    //TODO
}

internal thread_handle
brewpanel_x11_api_start_controller_comm_thread(
    BrewPanelControlCommData* controller_comm_data) {

    //TODO
    return(NULL);
}
