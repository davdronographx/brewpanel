#include <Windows.h>
#include <windowsx.h>

#include "brewpanel-types.hpp"
#include "brewpanel-platform-api.hpp"
#include "brewpanel-core.cpp"

internal void
brewpanel_win32_process_pending_messages(
    HWND window_handle) {

    MSG window_message = {0};
    while (PeekMessage(&window_message, 0,0,0,PM_REMOVE)) {

        switch(window_message.message) {
            default: {
                TranslateMessage(&window_message);
                DispatchMessage(&window_message);
            }
        }
    }
}

LRESULT CALLBACK
brewpanel_win32_callback(
    HWND window_handle,
    u32 message,
    WPARAM w_param,
    LPARAM l_param) {

    switch (message) {

        default: {
            return DefWindowProc(window_handle,message,w_param,l_param);
        }
    }

    return(0);
}

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

s32 WINAPI 
wWinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    PWSTR pw_str,
    s32 cmd_show) {
    
    platform_api = {0};
    platform_api.memory_allocate = brewpanel_win32_allocate_memory;
    platform_api.memory_free     = brewpanel_win32_free_memory;

    WNDCLASS window_class      = {0};
    window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc   = (WNDPROC)brewpanel_win32_callback;
    window_class.hInstance     = instance;
    window_class.lpszClassName = "brepanel-window-class";

    brewpanel_assert(RegisterClass(&window_class));

    HWND window_handle = CreateWindowEx(
        0,
        window_class.lpszClassName,
        "BrewPanel",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        instance,
        0);

    brewpanel_assert(window_handle);

    HDC device_context = GetDC(window_handle);

    bool running = true;

    BrewPanelState* brewpanel_state = brewpanel_core_create_state();
    brewpanel_assert(brewpanel_state != NULL);


    while(running) {

        brewpanel_win32_process_pending_messages(window_handle);
        SwapBuffers(device_context);
    }

    return 0;
}