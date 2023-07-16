#pragma once

#include <Windows.h>
#include "brewpanel-types.hpp"
#include "brewpanel-platform-api.hpp"

internal mem_data
brewpanel_win32_allocate_memory(
    u64 size) {

    mem_data memory = VirtualAlloc(0,size,MEM_COMMIT,PAGE_READWRITE);
    return(memory);
}

internal void
brewpanel_win32_free_memory(
    u64      size,
    mem_data memory) {
    
    VirtualFree(memory,size,MEM_RELEASE);
}

internal file_handle
brewpanel_win32_get_read_write_file_handle(
    str file_path) {

    HANDLE file_handle = 
        CreateFile(
            file_path,
            (GENERIC_READ | GENERIC_WRITE),
            (FILE_SHARE_READ | FILE_SHARE_WRITE),
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL
    );

    return (file_handle == INVALID_HANDLE_VALUE)
        ? NULL
        : file_handle;
}

internal u64
brewpanel_win32_get_file_size(
    file_handle file) {

    return(0);
} 

internal file_handle
brewpanel_win32_create_file(
    str file_path) {

    HANDLE file_handle = 
        CreateFile(
            file_path,
            (GENERIC_READ | GENERIC_WRITE),
            (FILE_SHARE_READ | FILE_SHARE_WRITE),
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL
    );

    return (file_handle == INVALID_HANDLE_VALUE)
        ? NULL
        : file_handle;
}

internal void
brewpanel_win32_close_file(
    file_handle file) {
    
    CloseHandle(file);
}