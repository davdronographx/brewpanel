#include <Windows.h>
#include <windowsx.h>

#include "brewpanel-types.hpp"
#include "brewpanel-platform-api.hpp"
#include "brewpanel-core.cpp"

global bool running;
global HWND window_handle;
global HDC  device_context;

global HDC        bitmap_device_context;
global HBITMAP    bitmap_handle;
global BITMAPINFO bitmap_info;

internal void
brewpanel_win32_draw_bitmap(
    HDC  paint_context,
    RECT client_rect,
    u32  x,
    u32  y,
    u32  width,
    u32  height) {

    u32 window_width = client_rect.right - client_rect.left;
    u32 window_height = client_rect.bottom - client_rect.top;

    mem_data bitmap_data = brewpanel_back_buffer_data();

    StretchDIBits(
        paint_context,
        0,
        0,
        BREW_PANEL_WIDTH_PIXELS,
        BREW_PANEL_HEIGHT_PIXELS,
        0,
        0,
        window_width,
        window_height,
        &bitmap_data,
        &bitmap_info,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

internal void
brewpanel_win32_resize_bitmap() {

    if (bitmap_handle) {
        DeleteObject(bitmap_handle);
    }

    if (!bitmap_device_context) {
        bitmap_device_context = CreateCompatibleDC(0);
    }

    bitmap_info = {0};
    bitmap_info.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bitmap_info.bmiHeader.biWidth       = BREW_PANEL_WIDTH_PIXELS;
    bitmap_info.bmiHeader.biHeight      = BREW_PANEL_HEIGHT_PIXELS;
    bitmap_info.bmiHeader.biPlanes      = 1;
    bitmap_info.bmiHeader.biBitCount    = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;

    mem_data bitmap_data = brewpanel_back_buffer_data(); 

    bitmap_handle = CreateDIBSection(
        bitmap_device_context,
        &bitmap_info,
        DIB_RGB_COLORS,
        &bitmap_data,
        0,0
    );
}

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

        //resizing
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


            brewpanel_win32_resize_bitmap();

        } break;

        case WM_PAINT: {

            PAINTSTRUCT paint = {0};
            HDC paint_device_context = BeginPaint(window_handle,&paint);

            u32 x = paint.rcPaint.left;
            u32 y = paint.rcPaint.top;

            u32 width  = paint.rcPaint.right - x;
            u32 height = paint.rcPaint.bottom - y;

            PatBlt(
                paint_device_context,
                x, y,
                width, height,
                BLACKNESS
            );         

            RECT client_rect = {0};
            GetClientRect(window_handle,&client_rect);

            brewpanel_win32_draw_bitmap(paint_device_context, client_rect,x,y,width,height);

            EndPaint(window_handle,&paint);

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

    window_handle = CreateWindowEx(
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

    device_context = GetDC(window_handle);

    running = true;
    
    brewpanel_core_init();
    brewpanel_assert(brewpanel_state != NULL);


    while(running) {

        // SwapBuffers(device_context);
        
        brewpanel_core_update_and_render();

        brewpanel_win32_process_pending_messages(window_handle);
    }

    return 0;
}