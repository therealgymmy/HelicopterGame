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
