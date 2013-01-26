#ifndef X_HANDLER_H
#define X_HANDLER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include "x_resources.h"

struct Resources;

class XHandler {
public:
    XHandler (Resources *resources);
    ~XHandler ();

    const XWindowAttributes* windowAttributes () const;
    void draw    ();
    void refresh ();

    void setShowCredit (bool s) { showCredit_ = s; }
    bool showCredit () const { return showCredit_; }

    Display* display () { return xinfo_.display_; }
    Window&  window  () { return xinfo_.window_;  }

    void updateAttr (XWindowAttributes *attr);

private:
    XHandler (const XHandler&); // = delete

    void drawBombs      ();
    void drawExplosions ();
    void drawHeli       ();
    void drawMissiles   ();
    void drawTowers     ();

    void drawCredits ();
    void drawHealthBar ();

    Resources *resources_;
    XInfo      xinfo_;
    bool       showCredit_;
    int        score_;
};

struct RenderArg {
    Resources *resources_;
    XHandler  *xhandler_;
    pthread_mutex_t *mutex_;
};

void* render (void *renderArg);

#endif//X_HANDLER_H
