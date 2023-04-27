#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#include "bmp.h"

int8_t exitloop = 0;

int main(void)
{
    BMP bmp_file = openBMP("test.bmp");

    Display *display = XOpenDisplay(NULL);

    int screen = XDefaultScreen(display);

    Window window = XCreateSimpleWindow(
        display, RootWindow(display, screen),
        0, 0,
        bmp_file.width, bmp_file.height,
        0, 0,
        0
    );
    XWindowAttributes winattr = {0};
    XGetWindowAttributes(display, window, &winattr);

    XImage *image = XCreateImage(
        display, winattr.visual, winattr.depth,
        ZPixmap, 0, (char *) bmp_file.memory,
        bmp_file.width, bmp_file.height,
        32, bmp_file.width*4
    );

    GC graphics = XCreateGC(display, window, 0, NULL);

    Atom delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &delete_window, 1);

    XSelectInput(display, window, ExposureMask);

    XMapWindow(display, window);
    XSync(display, False);

    XEvent event;

    while(!exitloop) {
        while(XPending(display) > 0) {
            XNextEvent(display, &event);
            if(event.type == ClientMessage) {
                if((Atom) event.xclient.data.l[0] == delete_window) {
                    exitloop = 1;   
                }
            }
        }

        XPutImage(
            display,
            window,
            graphics,
            image,
            0, 0,
            0, 0,
            bmp_file.width, bmp_file.height
        );

        XSync(display, False);
    }
    XCloseDisplay(display);

    free(bmp_file.memory);
    
    return 0;
}