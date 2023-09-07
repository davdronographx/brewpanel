#pragma once

#include <Windows.h>

#include "brewpanel-types.hpp"
#include "brewpanel-platform-api.hpp"

internal mem_data
brewpanel_win32_api_allocate_memory(
    u64 size) {

    mem_data memory = (mem_data)VirtualAlloc(0,size,MEM_COMMIT,PAGE_READWRITE);
    return(memory);
}

internal void
brewpanel_win32_api_free_memory(
    u64      size,
    mem_data memory) {
    
    VirtualFree(memory,size,MEM_RELEASE);
}

internal file_handle
brewpanel_win32_api_open_file(
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
brewpanel_win32_api_get_file_size(
    file_handle file) {

    u64 file_size = 
        file != INVALID_HANDLE_VALUE 
        ? GetFileSize(file,NULL)
        : 0;

    return(file_size);
} 

internal file_handle
brewpanel_win32_api_create_file(
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
brewpanel_win32_api_close_file(
    file_handle file) {
    
    CloseHandle(file);
}

global u64 bytes_read = 0;

VOID CALLBACK
brewpanel_win32_api_file_io_completion_routine(
    DWORD          error_code,
    DWORD          bytes_transferred,
    LPOVERLAPPED   lpOverlapped) {
        
    bytes_read = bytes_transferred;
}


internal void
brewpanel_win32_api_read_file(
    file_handle file_handle,
    mem_data    file_buffer,
    u64         file_buffer_size,
    u64         file_offset) {

    //make sure we have a valid buffer to copy into
    brewpanel_assert(file_buffer);

    //read the file
    OVERLAPPED overlapped = {0};
    overlapped.Offset = file_offset;
    
    bool read_result =
        ReadFileEx(
            file_handle,
            file_buffer,
            file_buffer_size,
            &overlapped,
            brewpanel_win32_api_file_io_completion_routine
    );
 
    file_buffer[file_buffer_size] = '\0';

    brewpanel_assert(read_result);
}

internal void
brewpanel_win32_api_write_file(
    file_handle file_handle,
    mem_data    write_buffer,
    u64         write_buffer_size,
    u64         file_offset) {

    brewpanel_assert(write_buffer);

    //write to the file
    OVERLAPPED overlapped = {0};
    overlapped.Offset = file_offset;
    
    bool write_result =
        WriteFileEx(
            file_handle,
            write_buffer,
            write_buffer_size,
            &overlapped,
            brewpanel_win32_api_file_io_completion_routine
    );

    brewpanel_assert(write_result);
}

internal BrewPanelSystemTime
brewpanel_win32_api_get_system_time() {

    BrewPanelSystemTime bp_system_time = {0};

    SYSTEMTIME system_time = {0};
    GetLocalTime(&system_time);

    bp_system_time.hours   = system_time.wHour;
    bp_system_time.minutes = system_time.wMinute;
    bp_system_time.seconds = system_time.wSecond;

    return(bp_system_time);
}