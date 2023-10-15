

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>

#include "brewpanel-x11-api.cpp"
#include "brewpanel-core.cpp"
#include "brewpanel-x11.h"

struct BrewPanelX11Window {
    bool            running;
    Display*        display;
    int             screen;
    Window          window;
    GC              gc;
    XEvent          event;
    BrewPanelInput  input;
    XImage*         bitmap;
    Pixmap*         pixmap;
};  

unsigned long black, white;

internal void
brewpanel_x11_process_event(
    BrewPanelX11Window* x11_window) {

    XNextEvent(x11_window->display, &x11_window->event);
    
    switch(x11_window->event.type) {

        case Expose: {

            if (x11_window->event.xexpose.count == 0) {
                XClearWindow(x11_window->display, x11_window->window);
            }
        } break;

        case ButtonPress: {

            x11_window->input.click = true;
            x11_window->input.mouse_x_pos = x11_window->event.xbutton.x;
            x11_window->input.mouse_y_pos = x11_window->event.xbutton.y;
        } break;

        case ButtonRelease: {

            x11_window->input.click = false;
            x11_window->input.mouse_x_pos = x11_window->event.xbutton.x;
            x11_window->input.mouse_y_pos = x11_window->event.xbutton.y;
        } break;

        case MotionNotify: {

            x11_window->input.mouse_x_pos = x11_window->event.xbutton.x;
            x11_window->input.mouse_y_pos = x11_window->event.xbutton.y;
        } break;
    
        default: {
            break;
        }
    }
}

internal void
brewpanel_x11_draw_bitmap() {

//https://stackoverflow.com/questions/6609281/how-to-draw-an-image-from-file-on-window-with-xlib
}

int main(int argc, char** argv)
{
    //initialize the controller info
    brewpanel_assert(argc == 4);

    BrewPanelControllerInfo controller_info = {0};
    controller_info.vendor_id     = argv[1];
    controller_info.product_id    = argv[2];
    controller_info.serial_number = argv[3];

    //initialize the api
    platform_api = {0};
    platform_api.memory_allocate              = brewpanel_x11_api_memory_allocate;
    platform_api.memory_free                  = brewpanel_x11_api_memory_free;
    platform_api.file_open                    = brewpanel_x11_api_file_open;
    platform_api.file_get_size                = brewpanel_x11_api_file_get_size;
    platform_api.file_create                  = brewpanel_x11_api_file_create;
    platform_api.file_close                   = brewpanel_x11_api_file_close;
    platform_api.file_read                    = brewpanel_x11_api_file_read; 
    platform_api.file_write                   = brewpanel_x11_api_file_write; 
    platform_api.system_time_get              = brewpanel_x11_api_system_time;
    platform_api.controller_handle            = brewpanel_x11_api_controller_handle;
    platform_api.controller_close             = brewpanel_x11_api_controller_close;
    platform_api.controller_write             = brewpanel_x11_api_controller_write_buffer;
    platform_api.controller_thread_start_read = brewpanel_x11_api_start_controller_comm_thread;

    //initialize the brewpanel
    brewpanel_core_init(controller_info);
    brewpanel_assert(brewpanel_state);

    //create the window
    BrewPanelX11Window x11_window = {0};
    
    x11_window.display = XOpenDisplay((char *)0);
    x11_window.screen  = DefaultScreen(x11_window.display);

    u32 screen_width  = XDisplayWidth(x11_window.display,x11_window.screen);
    u32 screen_height = XDisplayHeight(x11_window.display,x11_window.screen);

    u32 center_x = screen_width  / 2;
    u32 center_y = screen_height / 2;
    
    x11_window.window  =
        XCreateSimpleWindow(
            x11_window.display, 
            DefaultRootWindow(x11_window.display), 
            center_x, 
            center_y, 
            BREW_PANEL_WIDTH_PIXELS,
            BREW_PANEL_HEIGHT_PIXELS,
            0,
            white,
            black);
    x11_window.gc      = XCreateGC(x11_window.display, x11_window.window, 0, 0);
    x11_window.running = true;
    
    Visual bitmap_visual = {0};
    bitmap_visual.bits_per_rgb = 32;

    mem_data pixel_data = brewpanel_back_buffer_data(); 

    x11_window.bitmap = XCreateImage(
        x11_window.display,
        &bitmap_visual,
        XYBitmap,
        32,
        0,
        pixel_data,
        BREW_PANEL_WIDTH_PIXELS,
        BREW_PANEL_HEIGHT_PIXELS,
        32,
        0
    );

    x11_window.pixmap = XCreatePixmap(
        x11_window.display,
        x11_window.screen,
        BREW_PANEL_WIDTH_PIXELS,
        BREW_PANEL_HEIGHT_PIXELS,
        32
    );

    black = BlackPixel(x11_window.display, x11_window.screen);
    white = WhitePixel(x11_window.display, x11_window.screen);

    XSetStandardProperties(x11_window.display, x11_window.window, "Brewpanel", "Brewpanel", None, NULL, 0, NULL);
    XSelectInput(x11_window.display, x11_window.window, ExposureMask | ButtonPressMask | KeyPressMask | PointerMotionMask);
    XSetBackground(x11_window.display, x11_window.gc, white);
    XSetForeground(x11_window.display, x11_window.gc, black);
    XClearWindow(x11_window.display, x11_window.window);
    XMapRaised(x11_window.display,   x11_window.window);

    while (x11_window.running)
    {
        brewpanel_x11_process_event(&x11_window);

        if (brewpanel_core_update_and_render(&x11_window.input)) {
            brewpanel_x11_draw_bitmap();
        }
    }

    XFreeGC(x11_window.display, x11_window.gc);
    XDestroyWindow(x11_window.display, x11_window.window);
    XCloseDisplay(x11_window.display);
    exit(0);
    return(0);
}