#include <Windows.h>
#include <windowsx.h>

#include "brewpanel-types.hpp"
#include "brewpanel-platform-api.hpp"
#include "brewpanel-win32-api.cpp"
#include "brewpanel-core.cpp"

global bool running;
global HWND window_handle;
global HDC  device_context;

global HDC        bitmap_device_context;
global HBITMAP    bitmap_handle;
global BITMAPINFO bitmap_info;
global HDC        paint_context;
global RECT       client_rect;

global BrewPanelInput brewpanel_input;

internal void
brewpanel_win32_draw_bitmap() {

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
        (void*)&brewpanel_state->back_buffer.pixels,
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
        &((void*)bitmap_data),
        0,0
    );
}

internal void
brewpanel_win32_process_pending_messages(
    HWND window_handle) {

    MSG window_message = {0};
    while (PeekMessage(&window_message, 0,0,0,PM_REMOVE)) {

        switch(window_message.message) {

            case WM_LBUTTONDOWN: {

                brewpanel_input.click = true;

                //get mouse position
                POINT current_cursor_position = {0};
                GetCursorPos(&current_cursor_position);

                brewpanel_input.mouse_x_pos = GET_X_LPARAM(window_message.lParam);
                brewpanel_input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - GET_Y_LPARAM(window_message.lParam);

            } break;

            case WM_LBUTTONUP: {

                brewpanel_input.click = false;

                brewpanel_input.mouse_x_pos = GET_X_LPARAM(window_message.lParam);
                brewpanel_input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - GET_Y_LPARAM(window_message.lParam);

            } break;

            case WM_MOUSEMOVE: {

                //get mouse position
                POINT current_cursor_position = {0};
                GetCursorPos(&current_cursor_position);

                brewpanel_input.mouse_x_pos = GET_X_LPARAM(window_message.lParam);
                brewpanel_input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - GET_Y_LPARAM(window_message.lParam);

            } break;

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
            paint_context = BeginPaint(window_handle,&paint);

            u32 x = paint.rcPaint.left;
            u32 y = paint.rcPaint.top;

            u32 width  = paint.rcPaint.right - x;
            u32 height = paint.rcPaint.bottom - y;

            PatBlt(
                paint_context,
                x, y,
                width, height,
                BLACKNESS
            );         

            client_rect = {0};
            GetClientRect(window_handle,&client_rect);

            brewpanel_win32_draw_bitmap();

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

s32 WINAPI 
wWinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    PWSTR pw_str,
    s32 cmd_show) {
    
    //get the controller info
    s32     argc;
    LPWSTR* argv;

    argv = CommandLineToArgvW(GetCommandLineW(),&argc);
    brewpanel_assert(argc == 4);

    char vendor_id_buffer[32];
    char product_id_buffer[32];
    char serial_number_buffer[32];

    wcstombs(vendor_id_buffer,     argv[1], 32);
    wcstombs(product_id_buffer,    argv[2], 32);
    wcstombs(serial_number_buffer, argv[3], 32);

    BrewPanelControllerInfo controller_info = {0};
    controller_info.vendor_id     = vendor_id_buffer;
    controller_info.product_id    = product_id_buffer;
    controller_info.serial_number = serial_number_buffer;

    //create the api
    platform_api = {0};
    platform_api.memory_allocate               = brewpanel_win32_api_allocate_memory;
    platform_api.memory_free                   = brewpanel_win32_api_free_memory;
    platform_api.file_open                     = brewpanel_win32_api_open_file;
    platform_api.file_get_size                 = brewpanel_win32_api_get_file_size;
    platform_api.file_create                   = brewpanel_win32_api_create_file;
    platform_api.file_close                    = brewpanel_win32_api_close_file;
    platform_api.file_read                     = brewpanel_win32_api_read_file;
    platform_api.file_write                    = brewpanel_win32_api_write_file;
    platform_api.system_time_get               = brewpanel_win32_api_get_system_time;
    platform_api.controller_handle             = brewpanel_win32_api_controller_handle;
    platform_api.controller_write              = brewpanel_win32_api_controller_write;
    platform_api.controller_close              = brewpanel_win32_api_controller_close;
    platform_api.controller_read               = brewpanel_win32_api_controller_read; 
    platform_api.controller_thread_start_read  = brewpanel_win32_api_controller_thread_start_read;
    platform_api.controller_thread_start_write = brewpanel_win32_api_controller_thread_start_write;

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

    LONG lStyle = GetWindowLong(window_handle, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(window_handle, GWL_STYLE, lStyle);

    brewpanel_assert(window_handle);

    device_context = GetDC(window_handle);

    running = true;
    
    brewpanel_core_init(controller_info);
    brewpanel_assert(brewpanel_state != NULL);

    brewpanel_win32_draw_bitmap();
    while(running) {

        brewpanel_win32_process_pending_messages(window_handle);

        if (brewpanel_core_update_and_render(&brewpanel_input)){
            brewpanel_win32_draw_bitmap();
        }

        SwapBuffers(device_context);
    }

    return 0;
}