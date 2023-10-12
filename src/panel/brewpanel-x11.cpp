

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>

#include "brewpanel-x11-api.cpp"
#include "brewpanel-core.cpp"

struct BrewPanelX11Window {
    bool     running;
    Display* display;
    int      screen;
    Window   window;
    GC       gc;
    XEvent   event;
};  

unsigned long black, white;


struct coord
{
    int x, y;
} dot;

internal void
brewpanel_x11_process_event(
    BrewPanelX11Window* x11_window) {

    local char text[255];
    local KeySym   key;

    XNextEvent(x11_window->display, &x11_window->event);
    if (x11_window->event.type == Expose && x11_window->event.xexpose.count == 0)
    {
        XClearWindow(x11_window->display, x11_window->window);
    }
    if (x11_window->event.type == KeyPress && XLookupString(&x11_window->event.xkey, text, 255, &key, 0) == 1)
    {
        if (text[0] == 'q')
        {
            x11_window->running = false;
        }
    }
    if (x11_window->event.type == ButtonPress)
    {
        int x = x11_window->event.xbutton.x, y = x11_window->event.xbutton.y;
        dot.x = x;
        dot.y = y;
    }
}

int main(int argc, char** argv)
{
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

    dot.x = 100;
    dot.y = 100;
    black = BlackPixel(x11_window.display, x11_window.screen);
    white = WhitePixel(x11_window.display, x11_window.screen);

    XSetStandardProperties(x11_window.display, x11_window.window, "Brewpanel", "Brewpanel", None, NULL, 0, NULL);
    XSelectInput(x11_window.display, x11_window.window, ExposureMask | ButtonPressMask | KeyPressMask);
    XSetBackground(x11_window.display, x11_window.gc, white);
    XSetForeground(x11_window.display, x11_window.gc, black);
    XClearWindow(x11_window.display, x11_window.window);
    XMapRaised(x11_window.display,   x11_window.window);

    while (x11_window.running)
    {
        brewpanel_x11_process_event(&x11_window);
    }

    XFreeGC(x11_window.display, x11_window.gc);
    XDestroyWindow(x11_window.display, x11_window.window);
    XCloseDisplay(x11_window.display);
    exit(0);
    return(0);
}