#include <algorithm>
#include <functional>
#include <sstream>
#include <unistd.h>

#include "error.h"
#include "resources.h"
#include "x_handler.h"
#include "x_resources.h"

#define AUTHOR     "Hong Lu"
#define STUDENT_ID "234"

static
const int REFRESH_RATE = 15;   // (1000 / 15), roughly 60 frames per sec

static
void render (RenderArg *renderArg);

XHandler::XHandler (Resources *resources)
: resources_(resources),
  showCredit_(false),
  score_(0) {
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

    // set up black_
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

    // set up white_
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

    setupGC(xinfo_, xinfo_.grey_,  "grey");
    setupGC(xinfo_, xinfo_.green_, "green");
    setupGC(xinfo_, xinfo_.brown_, "brown");
    setupGC(xinfo_, xinfo_.red_,   "red");

    XMapRaised(xinfo_.display_, xinfo_.window_);

    XFlush(xinfo_.display_);
}

XHandler::~XHandler () {
}

const XWindowAttributes* XHandler::windowAttributes () const {
    return &xinfo_.attr_;
}

// draws a frame
void XHandler::draw () {
    if (!showCredit_) {
        drawBombs();
        drawExplosions();
        drawHeli();
        drawMissiles();
        drawTowers();

        drawHealthBar();
    }
    else {
        drawCredits();
    }
}

void XHandler::drawCredits () {
    // border
    Display *display = xinfo_.display_;
    Pixmap   buffer  = xinfo_.buffer_;
    GC       black   = xinfo_.black_;
    GC       white   = xinfo_.white_;

    XFillRectangle(display, buffer, black,
             0,
             0,
             PLANE_W / attr::DrawableBase::ScaleX,
             PLANE_H / attr::DrawableBase::ScaleY);

    XFillRectangle(display, buffer, white,
             100 / attr::DrawableBase::ScaleX,
             100 / attr::DrawableBase::ScaleY,
             (PLANE_W - 200) / attr::DrawableBase::ScaleX,
             (PLANE_H - 200) / attr::DrawableBase::ScaleY);

    // output text
}

void XHandler::drawHealthBar () {
    resources_->healthBar_.draw(&xinfo_);
}

void XHandler::drawBombs () {
    resources_->bombs_.draw(&xinfo_);
}

void XHandler::drawExplosions () {
    resources_->explosions_.draw(&xinfo_);
}

void XHandler::drawHeli () {
    resources_->heli_.draw(&xinfo_);
}

void XHandler::drawMissiles () {
    resources_->missiles_.draw(&xinfo_);
}

void XHandler::drawTowers () {
    resources_->towers_.draw(&xinfo_);
}

void XHandler::updateAttr (XWindowAttributes *attr) {
    xinfo_.attr_ = *attr;

    int depth = DefaultDepth(xinfo_.display_, xinfo_.screen_);
    XFreePixmap(xinfo_.display_, xinfo_.buffer_);
    xinfo_.buffer_ = XCreatePixmap(
            xinfo_.display_, xinfo_.window_,
            attr->width, attr->height, depth);
}

void XHandler::refresh () {
    Display           *display = xinfo_.display_;
    Window            &window  = xinfo_.window_;
    Pixmap            &buffer  = xinfo_.buffer_;
    GC                &draw    = xinfo_.black_;
    GC                &erase   = xinfo_.white_;
    XWindowAttributes &attr    = xinfo_.attr_;

    unsigned int winW = attr.width;
    unsigned int winH = attr.height;

    XCopyArea(display, buffer, window, draw, 0, 0, winW, winH, 0, 0);
    XFlush(display);

    // clear buffer
    XFillRectangle(display, buffer, erase, 0, 0, winW, winH);
}

void* render (void *renderArg) {
    render(static_cast<RenderArg*>(renderArg));
    return NULL;
}

void render (RenderArg *renderArg) {
    XHandler *xhandler  = renderArg->xhandler_;
    pthread_mutex_t *mutex = renderArg->mutex_;

    while (true) {
        if (pthread_mutex_lock(mutex) == -1) {
            error("cannot lock mutex in render thread");
        }

        xhandler->draw();

        if (pthread_mutex_unlock(mutex) == -1) {
            error("cannot unlock mutex in render thread");
        }

        xhandler->refresh();
        usleep(1000 * REFRESH_RATE);
    };
}
