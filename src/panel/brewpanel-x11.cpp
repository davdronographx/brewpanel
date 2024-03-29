#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/extensions/XInput2.h>
#include <stdio.h>
#include <stdlib.h>

#include "brewpanel-x11-api.cpp"
#include "brewpanel-core.cpp"
#include "brewpanel-x11.h"


#define PIXEL_BUFFER_SIZE (BREW_PANEL_WIDTH_PIXELS * BREW_PANEL_HEIGHT_PIXELS * 4)
struct BrewPanelX11Window {
    bool            running;
    Display*        display;
    int             screen;
    Window          window;
    GC              gc;
    XEvent          event;
    BrewPanelInput  input;
    XImage*         panel_image;
    Pixmap*         pixmap;
    mem_byte        pixel_buffer[PIXEL_BUFFER_SIZE];
};

unsigned long black, white;

internal void
brewpanel_x11_simulate_click(
    s32 x,
    s32 y) {

}

internal void
brewpanel_x11_process_event(
    BrewPanelX11Window* x11_window) {

    XAllowEvents(x11_window->display, AsyncBoth, CurrentTime);
    XNextEvent(x11_window->display, &x11_window->event);

    if (x11_window->event.xcookie.type == GenericEvent) {

        XGetEventData(x11_window->display, &x11_window->event.xcookie);

        if (x11_window->event.xcookie.evtype == XI_TouchBegin) {
    

            XGetEventData(x11_window->display, &x11_window->event.xcookie);
            XGetEventData(x11_window->display, &x11_window->event.xcookie);
            f64 x = *(f64*)x11_window->event.xcookie.data;*(f64*)
            XGetEventData(x11_window->display, &x11_window->event.xcookie);
            f64 y = *(f64*)x11_window->event.xcookie.data;
            XFreeEventData(x11_window->display, &x11_window->event.xcookie);
        }
    }
            


    switch(x11_window->event.type) {

        case ButtonPress: {

            x11_window->input.click = true;
            x11_window->input.mouse_x_pos = x11_window->event.xbutton.x;
            x11_window->input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - x11_window->event.xbutton.y;
        } break;

        case ButtonRelease: {

            x11_window->input.click = false;
            x11_window->input.mouse_x_pos = x11_window->event.xbutton.x;
            x11_window->input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - x11_window->event.xbutton.y;
        } break;

        case MotionNotify: {

            x11_window->input.mouse_x_pos = x11_window->event.xbutton.x;
            x11_window->input.mouse_y_pos = BREW_PANEL_HEIGHT_PIXELS - x11_window->event.xbutton.y;
        } break;

        case Expose: {

            XPutImage(
                    x11_window->display,
                    x11_window->window,
                    x11_window->gc,
                    x11_window->panel_image,
                    0,0,0,0,
                    BREW_PANEL_WIDTH_PIXELS,
                    BREW_PANEL_HEIGHT_PIXELS
            );

        } break;

        case GraphicsExpose: {

            XPutImage(
                    x11_window->display,
                    x11_window->window,
                    x11_window->gc,
                    x11_window->panel_image,
                    0,0,0,0,
                    BREW_PANEL_WIDTH_PIXELS,
                    BREW_PANEL_HEIGHT_PIXELS
            );

        } break;

        default: {
            break;
        }
    }
}

int main(int argc, char** argv)
{
    //initialize the controller info
    brewpanel_assert(argc == 5);

    BrewPanelControllerInfo controller_info = {0};
    controller_info.vendor_id     = argv[1];
    controller_info.product_id    = argv[2];
    controller_info.serial_number = argv[3];
    controller_info.port          = argv[4];

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

    //create the image
    Visual* bitmap_visual =
        DefaultVisual(
            x11_window.display,
            x11_window.screen
    );


    XSetWindowAttributes attributes = {0};
    attributes.background_pixel = XWhitePixel(x11_window.display,x11_window.screen);
 
     x11_window.window = 
        XCreateWindow(
            x11_window.display,
            XRootWindow(x11_window.display,x11_window.screen),
            center_x,
            center_y,
            BREW_PANEL_WIDTH_PIXELS,
            BREW_PANEL_HEIGHT_PIXELS,
            0,
            DefaultDepth(x11_window.display,x11_window.screen),
            InputOutput,
            bitmap_visual,
            CWBackPixel,
            &attributes
    );


    x11_window.running = true;


    x11_window.panel_image =
        XCreateImage(
            x11_window.display,
            bitmap_visual,
            DefaultDepth(x11_window.display,x11_window.screen),
            ZPixmap,
            0,
            x11_window.pixel_buffer,
            BREW_PANEL_WIDTH_PIXELS,
            BREW_PANEL_HEIGHT_PIXELS,
            32,
            0
    );

    XSetStandardProperties(x11_window.display, x11_window.window, "Brewpanel", "Brewpanel", None, NULL, 0, NULL);
    XSelectInput(x11_window.display, x11_window.window,  ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | PointerMotionMask);
    
    x11_window.gc      = XCreateGC(x11_window.display, x11_window.window, 0, 0);


    //add the extension
    // if (!XQueryExtension(x11_window.display,"XInputExtension",NULL,NULL,NULL)) {
    //     brewpanel_panic();
    // }

    XIEventMask x_event_mask;
    unsigned char mask[(XI_LASTEVENT + 7)/8] = {0};
    x_event_mask.deviceid = XIAllDevices;
    x_event_mask.mask_len = sizeof(mask);
    XISetMask(mask,XI_TouchBegin);
    XISetMask(mask,XI_TouchEnd);
    XISetMask(mask,XI_TouchUpdate);

    // XISelectEvents(x11_window.display,x11_window.window,&x_event_mask,1);


    XFlush(x11_window.display);
    XMapWindow(x11_window.display,   x11_window.window);
    XFlush(x11_window.display);


    while (x11_window.running)
    {
        while (XPending(x11_window.display)) {
            brewpanel_x11_process_event(&x11_window);
        }

        if (brewpanel_core_update_and_render(&x11_window.input)) {
    
            mem_data pixel_buffer = brewpanel_back_buffer_data();

            for (
                u32 row = 0;
                row < BREW_PANEL_HEIGHT_PIXELS;
                ++row
            ) {

                for (
                    u32 column = 0;
                    column < BREW_PANEL_WIDTH_PIXELS;
                    ++column
                ) {

                    u32 source_index      = (row * BREW_PANEL_WIDTH_PIXELS + column) * 4;
                    u32 destination_index = ((BREW_PANEL_HEIGHT_PIXELS - row - 1) * BREW_PANEL_WIDTH_PIXELS + (column)) * 4;

                    x11_window.pixel_buffer[destination_index + 0] = pixel_buffer[source_index + 0]; 
                    x11_window.pixel_buffer[destination_index + 1] = pixel_buffer[source_index + 1]; 
                    x11_window.pixel_buffer[destination_index + 2] = pixel_buffer[source_index + 2]; 
                    x11_window.pixel_buffer[destination_index + 3] = pixel_buffer[source_index + 3]; 
                }
            }
        }

                    XPutImage(
                    x11_window.display,
                    x11_window.window,
                    x11_window.gc,
                    x11_window.panel_image,
                    0,0,0,0,
                    BREW_PANEL_WIDTH_PIXELS,
                    BREW_PANEL_HEIGHT_PIXELS
            );
    }


    XFreeGC(x11_window.display, x11_window.gc);
    XDestroyWindow(x11_window.display, x11_window.window);
    XCloseDisplay(x11_window.display);
    exit(0);
    return(0);
}