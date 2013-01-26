#include "explosion.h"
#include "tower.h"

int Explosion::Speed_ = 5;

Explosion::Explosion (int x, int y)
: x_(x), y_(y),
  w_(0), h_(0) {}

void Explosion::draw (XInfo *xinfo, int x, int y) {
    Draw::draw<attr::GREEN>(xinfo,
                            (x - w_ / 2) / ScaleX,
                            (y - h_ / 2) / ScaleY);
}

void Explosion::inflate () {
    w_ += 5;
    h_ += 5;
}

void ExplosionList::draw (XInfo *xinfo) {
    for (Explosions::iterator itr = explosions_.begin();
         itr != explosions_.end();
         ++itr) {
        itr->draw(xinfo, itr->x(), itr->y());
    }
}

void ExplosionList::move () {
    for (Explosions::iterator itr = explosions_.begin();
         itr != explosions_.end();
         ++itr) {
        itr->setX(itr->x() - Tower::speed());
        itr->inflate();
        if (itr->w() > 300 &&
            itr->h() > 300) {
            itr = explosions_.erase(itr);
        }
    }
}

void ExplosionList::addExplosion (int x, int y) {
    explosions_.push_back(Explosion(x, y));
}
