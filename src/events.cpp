#include <unistd.h>

#include "error.h"
#include "events.h"

static
void pollForEvents (EventsArg *eventsArg);

void* events (void *eventsArg) {
    pollForEvents(static_cast<EventsArg*>(eventsArg));
    return NULL;
}

void pollForEvents (EventsArg *eventsArg) {
    Resources       *resources = eventsArg->resources_;
    XHandler        *xhandler  = eventsArg->xhandler_;
    pthread_mutex_t *mutex     = eventsArg->mutex_;

    Display *display = xhandler->display();
    Window   window  = xhandler->window();
    XSelectInput(display, window,
                 KeyPressMask |
                 KeyReleaseMask |
                 StructureNotifyMask);

    Helicopter &helicopter = resources->heli_;
    KeySym key;
    XWindowAttributes attr;

    XEvent event;
    while (true) {
        XNextEvent(display, &event);

        if (pthread_mutex_lock(mutex) == -1) {
            error("cannot lock mutex in control thread");
        }

        switch (event.type) {
            case ConfigureNotify:
                if (event.xconfigure.width  != attr.width ||
                        event.xconfigure.height != attr.height) {
                    XGetWindowAttributes(display, window, &attr);

                    attr::DrawableBase::ScaleX = (double)PLANE_W / (double)attr.width;
                    attr::DrawableBase::ScaleY = (double)PLANE_H / (double)attr.height;

                    std::cout << attr::DrawableBase::ScaleX << std::endl;
                    std::cout << attr::DrawableBase::ScaleY << std::endl;
                    std::cout << "w: " << attr.width << std::endl;
                    std::cout << "h: " << attr.height << std::endl;

                    xhandler->updateAttr(&attr);
                }

                break;

            case KeyPress:
                XLookupString(&event.xkey, NULL, 0, &key, NULL);
                switch (key) {
                    case XK_f:
                        if (xhandler->showCredit()) {
                            xhandler->setShowCredit(false);
                        }
                        else {
                            xhandler->setShowCredit(true);
                        }
                        break;

                    case XK_Up:
                        helicopter.up_ = true;
                        break;
                    case XK_Down:
                        helicopter.down_ = true;
                        break;
                    case XK_Left:
                        helicopter.left_ = true;
                        break;
                    case XK_Right:
                        helicopter.right_ = true;
                        break;

                    case XK_a:
                        Tower::incSpeed();
                        Missile::incSpeed();
                        break;
                    case XK_z:
                        Tower::decSpeed();
                        Missile::decSpeed();
                        break;

                    /*
                    case XK_q:  // slow down scroll speed
                        Tower::incDelay(1);
                        Missile::incDelay(1);
                        Bomb::incDelay(2);
                        break;
                    case XK_w:  // speed up scroll speed
                        Tower::decDelay(1);
                        Missile::decDelay(1);
                        Bomb::decDelay(2);
                        break;

                    */
                    case XK_d:
                        if (helicopter.right_) {
                            resources->bombs_.addBomb(helicopter.x(),
                                                      helicopter.y(),
                                                      Bomb::FORWARD);
                        }
                        else if (helicopter.left_) {
                            resources->bombs_.addBomb(helicopter.x(),
                                                      helicopter.y(),
                                                      Bomb::BACKWARD);
                        }
                        else {
                            resources->bombs_.addBomb(helicopter.x(),
                                                      helicopter.y(),
                                                      Bomb::NUL);
                        }

                        /*
                        if (helicopter.movement() == Helicopter::LEFT) {
                            resources->bombs_.push_back(
                                    Bomb(
                                        helicopter.x() + Helicopter::w(),
                                        helicopter.y() + Helicopter::h(),
                                        false));
                        }
                        else {
                            resources->bombs_.push_back(
                                    Bomb(
                                        helicopter.x() + Helicopter::w(),
                                        helicopter.y() + Helicopter::h(),
                                        true));
                        }
                        */
                        break;

                    default:
                        break;
                }
                break;

            case KeyRelease:
                XLookupString(&event.xkey, NULL, 0, &key, NULL);
                switch (key) {
                    case XK_Up:
                        helicopter.up_ = false;
                        helicopter.delayUp_ = 30;
                        break;
                    case XK_Down:
                        helicopter.down_ = false;
                        helicopter.delayDown_ = 30;
                        break;
                    case XK_Left:
                        helicopter.left_ = false;
                        helicopter.delayLeft_ = 30;
                        break;
                    case XK_Right:
                        helicopter.right_ = false;
                        helicopter.delayRight_ = 30;
                        break;
                    default:
                        break;
                }
                break;

            default:
                break;

        }

        if (pthread_mutex_unlock(mutex) == -1) {
            error("cannot unlock mutex in control thread");
        }
    }
}
