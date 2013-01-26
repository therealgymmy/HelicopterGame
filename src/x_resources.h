#ifndef X_RESOURCES_H
#define X_RESOURCES_H

#include <string>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define PLANE_W 4000
#define PLANE_H 3000

struct XInfo {
    Display          *display_;
    int               screen_;
    Window            window_;
    XWindowAttributes attr_;

    Pixmap buffer_;

    GC     black_;
    GC     white_;
    GC     grey_;
    GC     green_;
    GC     brown_;
    GC     red_;
};

inline
void setupGC (XInfo &xinfo, GC &target, std::string color) {
    // init values for the GC
    XGCValues values;
    values.cap_style = CapButt;
    values.join_style = JoinBevel;

    // which values to check when creating the GC
    unsigned long valuemask = GCCapStyle | GCJoinStyle;

    Colormap colormap = DefaultColormap(xinfo.display_, DefaultScreen(xinfo.display_));
    XColor c;

    target = XCreateGC(xinfo.display_, xinfo.window_, valuemask, &values);

    XAllocNamedColor(xinfo.display_, colormap, color.c_str(), &c, &c);

    XSetForeground(
            xinfo.display_,
            target,
            c.pixel);

    XSetBackground(
            xinfo.display_,
            target,
            BlackPixel(xinfo.display_, xinfo.screen_));

    XSetFillStyle(xinfo.display_, target, FillSolid);

    XSetLineAttributes(
            xinfo.display_, target,
            2,              // line width
            LineSolid,      // line drawing style
            CapRound,       // cap drawing style (line'end)
            JoinRound);     // lines join style


}

#endif//X_RESOURCES_H
