#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>

#include "brewpanel-types.hpp"

internal mem_data
brewpanel_x11_api_memory_allocate(
    u64 size) {

    //TODO
    return(NULL);
}


internal void
brewpanel_x11_api_memory_free(
    u64      size,
    mem_data memory) {

    //TODO
}

internal brewpanel_file_handle
brewpanel_x11_api_file_open(
    str file_path) {

    //TODO
    return(NULL);
}

internal u64
brewpanel_x11_api_file_get_size(
    brewpanel_file_handle file) {

    //TODO
    return(0);
}

internal brewpanel_file_handle
brewpanel_x11_api_file_create(
    str file_path) {

    //TODO 
    return(NULL);
}

internal void
brewpanel_x11_api_file_close(
    brewpanel_file_handle file) {
    
    //TODO
}

internal void
brewpanel_x11_api_file_read(
    brewpanel_file_handle file,
    mem_data              read_buffer,
    u64                   read_buffer_size,
    u64                   file_offset) {

    //TODO
}

internal void
brewpanel_x11_api_file_write(
    brewpanel_file_handle file,
    mem_data              write_buffer,
    u64                   write_buffer_size,
    u64                   file_offset) {

    //TODO
}

internal BrewPanelSystemTime
brewpanel_x11_api_system_time(){
    
    BrewPanelSystemTime system_time = {0};

    //TODO

    return(system_time);
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
