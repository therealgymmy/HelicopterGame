#include <cmath>
#include <unistd.h>

#include "control.h"
#include "error.h"
#include "events.h"
#include "resources.h"
#include "x_resources.h"
#include "x_handler.h"

int main () {
    // set up for multithreading
    XInitThreads();

    Resources resources;
    XHandler xhandler(&resources);

    /*
    updateSize(xhandler.windowAttributes());
    Tower::setDelay(2);
    Missile::setDelay(2);
    Bomb::setDelay(4);

    xhandler.setShowCredit(true);
    */

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // dispatch rendering thread
    RenderArg renderArg = { &resources, &xhandler, &mutex };
    pthread_t renderT;
    pthread_create(&renderT, NULL, render, static_cast<void*>(&renderArg));

    // dispatch events thread
    EventsArg eventsArg = { &resources, &xhandler, &mutex };
    pthread_t eventsT;
    pthread_create(&eventsT, NULL, events, static_cast<void*>(&eventsArg));

    // dispatch controlling thread
    ControlArg controlArg = { &resources, &xhandler, &mutex };
    pthread_t controlT;
    pthread_create(&controlT, NULL, control, static_cast<void*>(&controlArg));

    /*
    // cleans credit screen
    usleep(1000 * 1000 * 2);
    xhandler.setShowCredit(false);
    */

    // join threads
    pthread_join(renderT,  NULL);
    pthread_join(eventsT,  NULL);
    pthread_join(controlT, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;

}

/*
int main () {
    XInfo xinfo_;

    if (!(xinfo_.display_ = XOpenDisplay(""))) {
        error("Can't open display");
    }
    xinfo_.screen_ = DefaultScreen(xinfo_.display_);

    XSizeHints hints;
    hints.x = 100;
    hints.y = 100;
    hints.width = 800;
    hints.height = 600;
    hints.flags = PPosition | PSize;

    unsigned long white = XWhitePixel(xinfo_.display_, xinfo_.screen_);
    unsigned long black = XBlackPixel(xinfo_.display_, xinfo_.screen_);

    xinfo_.window_ = XCreateSimpleWindow(
            xinfo_.display_,                    // display where window appears
            DefaultRootWindow(xinfo_.display_), // window's parent
            hints.x, hints.y,                   // upper left corner location
            hints.width, hints.height,          // window size
            5,                                  // window border width
            black,                              // window foreground colour
            white);                             // window background colour

    XSetStandardProperties(
            xinfo_.display_,    // display
            xinfo_.window_,     // window
            "Project 1",        // window title
            "P1",               // icon title
            None,               // pixmap for the icon
            None, None,         // cmd arguments
            &hints);            // hints for the window

    // get window attributes
    XGetWindowAttributes(xinfo_.display_, xinfo_.window_, &xinfo_.attr_);

    int depth = DefaultDepth(xinfo_.display_, xinfo_.screen_);

    xinfo_.buffer_ = XCreatePixmap(
            xinfo_.display_, xinfo_.window_, hints.width, hints.height, depth);

    // init values for the GC
    XGCValues values;
    values.cap_style = CapButt;
    values.join_style = JoinBevel;
    // which values to check when creating the GC
    unsigned long valuemask = GCCapStyle | GCJoinStyle;

    // set up gc_draw
    xinfo_.black_ = XCreateGC(xinfo_.display_, xinfo_.window_, valuemask, &values);

    XSetForeground(
            xinfo_.display_,
            xinfo_.black_,
            BlackPixel(xinfo_.display_, xinfo_.screen_));

    XSetBackground(
            xinfo_.display_,
            xinfo_.black_,
            BlackPixel(xinfo_.display_, xinfo_.screen_));

    XSetFillStyle(xinfo_.display_, xinfo_.black_, FillSolid);

    XSetLineAttributes(
            xinfo_.display_, xinfo_.black_,
            2,              // line width
            LineSolid,      // line drawing style
            CapRound,       // cap drawing style (line'end)
            JoinRound);     // lines join style

    // set up gc_erase
    xinfo_.white_ = XCreateGC(xinfo_.display_, xinfo_.window_, valuemask, &values);

    XSetForeground(
            xinfo_.display_,
            xinfo_.white_,
            WhitePixel(xinfo_.display_, xinfo_.screen_));

    XSetBackground(
            xinfo_.display_,
            xinfo_.white_,
            BlackPixel(xinfo_.display_, xinfo_.screen_));

    XSetFillStyle(xinfo_.display_, xinfo_.white_, FillSolid);

    XSetLineAttributes(
            xinfo_.display_, xinfo_.white_,
            2,              // line width
            LineSolid,      // line drawing style
            CapRound,       // cap drawing style (line'end)
            JoinRound);     // lines join style

    XMapRaised(xinfo_.display_, xinfo_.window_);

    XFlush(xinfo_.display_);

    XSelectInput(xinfo_.display_, xinfo_.window_,
                 StructureNotifyMask);

    TowerList towerlist;

    XWindowAttributes &attr = xinfo_.attr_;

    while (true) {
        towerlist.move();

        TowerList::Towers::iterator first = towerlist.towers_.begin();
        int firstx = first->x() / Tower::ScaleX;
        int firstw = Tower::W / Tower::ScaleX;
        for (TowerList::Towers::iterator itr = towerlist.towers_.begin();
             itr != towerlist.towers_.end();
             ++itr) {
            itr->draw(&xinfo_, firstx);
            firstx += firstw;
        }

        XCopyArea(xinfo_.display_, xinfo_.buffer_, xinfo_.window_, xinfo_.black_
                , 0, 0, attr.width, attr.height, 0, 0);
        XFlush(xinfo_.display_);
        XFillRectangle(xinfo_.display_, xinfo_.buffer_, xinfo_.white_,
                0, 0, attr.width, attr.height);


        // resize event
        if (XPending(xinfo_.display_)) {
            XEvent event;
            XNextEvent(xinfo_.display_, &event);

            switch (event.type) {
                case ConfigureNotify:
                    if (event.xconfigure.width  != attr.width ||
                            event.xconfigure.height != attr.height) {
                        XGetWindowAttributes(xinfo_.display_, xinfo_.window_, &attr);

                        attr::DrawableBase::ScaleX = std::ceil((double)PLANE_W / (double)attr.width);
                        attr::DrawableBase::ScaleY = std::ceil((double)PLANE_H / (double)attr.height);

                        attr.width  = PLANE_W / attr::DrawableBase::ScaleX;
                        attr.height = PLANE_H / attr::DrawableBase::ScaleY;

                        std::cout << "w: " << attr.width << std::endl;
                        std::cout << "h: " << attr.height << std::endl;

                        XFreePixmap(xinfo_.display_, xinfo_.buffer_);
                        xinfo_.buffer_ = XCreatePixmap(
                                xinfo_.display_, xinfo_.window_,
                                attr.width, attr.height, depth);
                    }
                    break;
            }

        }

        usleep(1000 * 15);
    }

    return 0;
}
*/
