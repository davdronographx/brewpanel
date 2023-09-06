#include <chrono>
#include <rs-232/rs232.h>
#include <rs-232/rs232.c>
#include <initguid.h>
#include <windows.h>
#include <Setupapi.h>
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

#define BUFF_LEN 20

//https://aticleworld.com/get-com-port-of-usb-serial-device/
internal controller_handle
brewpanel_sdl2_api_windows_controller_handle(
    BrewPanelControllerInfo controller_info) {

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

            if (query_result == ERROR_SUCCESS && registry_value_type == REG_SZ) {

                //we have a valid registry key, so we need to make sure that its the one we want
                //if it is, the portName will be returned as the 
                brewpanel_nop();
            }

            RegCloseKey(device_registry_key);
        }
    } 

    return(NULL);
}

internal controller_handle
brewpanel_sdl2_api_controller_handle(
    BrewPanelControllerInfo controller_info) {

    return brewpanel_sdl2_api_windows_controller_handle(controller_info);
}