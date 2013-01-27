#include <sstream>

#include "error.h"
#include "misc.h"

HealthBar::HealthBar ()
: x_(50), y_(50),
  w_(800), h_(60),
  hp_(100) {}

void HealthBar::draw (XInfo *xinfo) {
    frame_.draw<attr::BLACK>(xinfo,
                             x_ / ScaleX,
                             y_ / ScaleY);
    if (hp_ > 50) {
        Draw::draw<attr::GREEN>(xinfo,
                                (x_ + 20) / ScaleX,
                                (y_ + 20) / ScaleY);
    }
    else {
        Draw::draw<attr::RED>(xinfo,
                              (x_ + 20) / ScaleX,
                              (y_ + 20) / ScaleY);
    }
}

int Score::x_ = 3400;
int Score::y_ = 100;

Score::Score ()
: score_(0) {}

void Score::draw (XInfo *xinfo) {
    XFillRectangle(xinfo->display_,
                   xinfo->buffer_,
                   xinfo->white_,
                   x_ / attr::DrawableBase::ScaleX,
                   y_ / 5 / attr::DrawableBase::ScaleY,
                   500 / attr::DrawableBase::ScaleX,
                   100 / attr::DrawableBase::ScaleY);

    XDrawRectangle(xinfo->display_,
                   xinfo->buffer_,
                   xinfo->black_,
                   x_ / attr::DrawableBase::ScaleX,
                   y_ / 5 / attr::DrawableBase::ScaleY,
                   500 / attr::DrawableBase::ScaleX,
                   100 / attr::DrawableBase::ScaleY);

    std::stringstream ss;
    ss << "Score: " << score_ / 10;

    XDrawString(xinfo->display_,
                xinfo->buffer_,
                xinfo->black_,
                (x_ + 50) / attr::DrawableBase::ScaleX,
                y_ / attr::DrawableBase::ScaleY,
                ss.str().c_str(), ss.str().length());
}
