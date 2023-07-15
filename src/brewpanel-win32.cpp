#include <Windows.h>
#include <windowsx.h>

#include "brewpanel-types.hpp"
#include "brewpanel-platform-api.hpp"
#include "brewpanel-core.cpp"

global bool running;

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

        case WM_SIZE: {

            //get the screen dimensions
            u32 screen_width  = GetSystemMetrics(SM_CXSCREEN);
            u32 screen_height = GetSystemMetrics(SM_CYSCREEN);

            u32 x_offset = (screen_width / 2) - (BREW_PANEL_WIDTH_PIXELS / 2); 
            u32 y_offset = (screen_height / 2) - (BREW_PANEL_HEIGHT_PIXELS / 2); 

            //size the window
            RECT window_rect   = {0};
            window_rect.top    = y_offset;
            window_rect.left   = x_offset;
            window_rect.bottom = BREW_PANEL_HEIGHT_PIXELS;
            window_rect.right  = BREW_PANEL_WIDTH_PIXELS;
            
            //set window position
            SetWindowPos(
                    window_handle, 
                    NULL, 
                    window_rect.left,
                    window_rect.top,
                    window_rect.right,
                    window_rect.bottom,
                            SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);


        } break;

        //window close
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
        } break;

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
    
    //create the api
    platform_api = {0};
    platform_api.memory_allocate = brewpanel_win32_allocate_memory;
    platform_api.memory_free     = brewpanel_win32_free_memory;

    //open the window
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
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

    running = true;
    
    brewpanel_core_init();
    brewpanel_assert(brewpanel_state != NULL);


    while(running) {

        brewpanel_core_update_and_render();

        brewpanel_win32_process_pending_messages(window_handle);
        SwapBuffers(device_context);
    }

    return 0;
}