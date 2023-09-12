#pragma once

#include <Windows.h>
#include <stdio.h>
#include <Setupapi.h>
#include <initguid.h>
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

#define BUFF_LEN 20


internal controller_handle
brewpanel_win32_api_controller_handle(
    BrewPanelControllerInfo controller_info
) {
    HANDLE comm_handle;

    //put together the device id
    char expected_device_id[128] = {0};
    sprintf(
        expected_device_id,
        "vid_%s&pid_%s",
        controller_info.vendor_id,
        controller_info.product_id
    );

    HDEVINFO device_info = SetupDiGetClassDevs(
        NULL,
        "USB",
        NULL,
        DIGCF_ALLCLASSES | DIGCF_PRESENT
    );

    SP_DEVINFO_DATA device_info_data = {0};
    device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
    DEVPROPTYPE property_type;
    
    s32   device_index = 0;
    BYTE  device_info_buffer[1024] = {0};
    DWORD device_info_buffer_size = 0;
    HKEY  device_registry_key;

    //we are going to loop through the available devices and look for the controller
    for (
        bool device_found = true;
        device_found;
        device_found=SetupDiEnumDeviceInfo(device_info,++device_index,&device_info_data)
    ) {
        if (
            SetupDiGetDeviceRegistryProperty(
                device_info,
                &device_info_data,
                SPDRP_HARDWAREID,
                &property_type,
                (BYTE*)device_info_buffer,
                sizeof(device_info_buffer),
                &device_info_buffer_size
            )
        ) {

            //we have a device, so we are going to look at its details
            device_registry_key = 
                SetupDiOpenDevRegKey(
                    device_info,
                    &device_info_data,
                    DICS_FLAG_GLOBAL,
                    0,
                    DIREG_DEV,
                    KEY_READ
            );

            if (device_registry_key == INVALID_HANDLE_VALUE) {
                continue;
            }

            wchar_t port_name[BUFF_LEN];
            DWORD   registry_value_type  = 0;
            char    port_name_value[32]  = {0};
            DWORD   port_name_value_size = sizeof(char) * 32;

            u64 query_result = RegQueryValueEx(
                device_registry_key,
                "portName",
                NULL,
                &registry_value_type,
                (LPBYTE)port_name_value,
                &port_name_value_size
            );

            RegCloseKey(device_registry_key);
            
            if (query_result == ERROR_SUCCESS && registry_value_type == REG_SZ) {

                //TODO: I took a shortcut to get things working
                //but we really need to check the device id against the one we
                //put together right after the function call up top

                //we have a valid registry key, so we need to make sure that its the one we want
                //if it is, the portName will be used to open the device handle
                port_name_value[port_name_value_size] = '\0';
                char comm_port_name[16] = {0};
                sprintf(comm_port_name,"\\\\\\\\.\\\\%s",port_name_value);


                comm_handle = CreateFile(
                    "\\\\.\\COM4",
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_FLAG_OVERLAPPED,
                    NULL
                );

                //at this point, we should never fail
                if(comm_handle == INVALID_HANDLE_VALUE){
                    break;
                }

                //update the port settings
                DCB comm_port_info = {0};
                SecureZeroMemory(&comm_port_info, sizeof(DCB));
                comm_port_info.DCBlength = sizeof(DCB);

                if(!GetCommState(comm_handle,&comm_port_info)) {
                    break;
                }

                comm_port_info.BaudRate = CBR_9600;
                comm_port_info.ByteSize = 8;
                comm_port_info.Parity   = NOPARITY;
                comm_port_info.StopBits = ONESTOPBIT;

                if(!SetCommState(comm_handle,&comm_port_info)) {
                    break;
                }

                //set the timeouts
                COMMTIMEOUTS timeouts = {0};
                timeouts.ReadIntervalTimeout         = 50;
                timeouts.ReadTotalTimeoutConstant    = 50;
                timeouts.ReadTotalTimeoutMultiplier  = 10;
                timeouts.WriteTotalTimeoutConstant   = 50;
                timeouts.WriteTotalTimeoutMultiplier = 10;

                if (!SetCommTimeouts(comm_handle, &timeouts)) {
                    break;
                }

                return(comm_handle);
            }
        }
    } 

    return(NULL);
}

internal void
brewpanel_win32_api_controller_close(
    controller_handle controller_handle) {
        
    brewpanel_assert(CloseHandle(controller_handle));
}

//https://www.xanthium.in/Serial-Port-Programming-using-Win32-API#:~:text=SetCommTimeouts()%20function.-,Writing%20Data%20to%20Serial%20Port,files%20and%20I%2FO%20ports.&text=%2F%2FBytes%20written-,NULL)%3B,(I%20am%20using%20USB2SERIAL).

internal bool
brewpanel_win32_api_controller_write(
    controller_handle controller_handle,
    mem_data          write_buffer,
    u64               write_buffer_size) {

    brewpanel_assert(write_buffer);

    DWORD bytes_written = 0;

    bool result =
        WriteFile(
            controller_handle,
            write_buffer,
            write_buffer_size,
            &bytes_written,
            NULL
    );

    return(result && bytes_written == write_buffer_size);
}

//https://gist.github.com/DavidEGrayson/5e5bb95ae291cdfdffd4
//TODO: for reading from a serial port, we may need a thread with a callback 
//for whenever we detect any data on the port

//likewise for the controller

//https://aticleworld.com/serial-port-programming-using-win32-api/

internal bool
brewpanel_win32_api_controller_read(
    controller_handle controller_handle,
    mem_data          read_buffer,
    u64               read_buffer_size,
    u64*              bytes_read) {

    brewpanel_assert(read_buffer);

    if (!SetCommMask(controller_handle,EV_RXCHAR)) {
        return false;
    }

    DWORD event_mask = 0;

    // if (!WaitCommEvent(controller_handle,&event_mask, NULL)) {
    //     return false;
    // }


    DWORD loop_bytes_read = 0; 

    // bool result;
    // do {

    //     result = 
    //         ReadFile(
    //             controller_handle,
    //             read_buffer,
    //             read_buffer_size,
    //             &loop_bytes_read,
    //             NULL
    //     );

    //     ++(*bytes_read);

    // } while(loop_bytes_read > 0);
    
    // --(*bytes_read);
    
    return (*bytes_read > 0);
}

internal DWORD WINAPI
brewpanel_win32_controller_read(LPVOID payload) {

    controller_comm_data* comm_data = (controller_comm_data*)payload;
    OVERLAPPED overlapped_reader = {0};
    overlapped_reader.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

    brewpanel_assert(overlapped_reader.hEvent);

    bool waiting_on_read = FALSE;

    DWORD bytes_read_local = 0;

    bool thread_running = true;
    while (thread_running) {

        if (comm_data->controller && !waiting_on_read) {

            if (!ReadFile(
                comm_data->controller,
                comm_data->read_buffer,
                BREWPANEL_CONTROL_COMM_DATA_BUFFER_SIZE,
                &bytes_read_local,
                &overlapped_reader)) {

                waiting_on_read = (GetLastError() == ERROR_IO_PENDING);
            }
        }
    }

    return(NULL);
}

internal DWORD WINAPI
brewpanel_win32_controller_write(LPVOID payload) {

    controller_comm_data* comm_data = (controller_comm_data*)payload;


    bool thread_running = true;
    while (thread_running) {

        if (comm_data->controller && comm_data->bytes_to_write > 0) {

            


        }
    }

    return(NULL);
}

internal thread_handle
brewpanel_win32_api_controller_thread_start_read(
    controller_comm_data* comm_data) {
    
    SECURITY_ATTRIBUTES attributes = {0};

    thread_handle thread = CreateThread(
        &attributes,
        0,
        brewpanel_win32_controller_read,
        comm_data,
        NULL,
        NULL
    );

    return(thread);
}

internal thread_handle
brewpanel_win32_api_controller_thread_start_write(
    controller_comm_data* comm_data) {

    SECURITY_ATTRIBUTES attributes = {0};

    thread_handle thread = CreateThread(
        &attributes,
        0,
        brewpanel_win32_controller_write,
        comm_data,
        NULL,
        NULL
    );

    return(thread);
}