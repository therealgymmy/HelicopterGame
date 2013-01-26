#include "error.h"
#include "missile.h"
#include "tower.h"

int Missile::w_ = 20;
int Missile::h_ = 80;
int Missile::speed_ = 5;
const int Missile::speedMax_;
const int Missile::speedMin_;

static int cooldown = 1000;

Missile::Missile (int x, int y)
: x_(x), y_(y) {}

MissileList::MissileList ()
: recharge_(0) {}

void MissileList::draw (XInfo *xinfo) {
    for (Missiles::iterator itr = missiles_.begin();
         itr != missiles_.end();
         ++itr) {
        int x = itr->x() / Missile::ScaleX;
        int y = itr->y() / Missile::ScaleY;
        itr->draw<attr::BLACK>(xinfo, x, y);
    }
}

void MissileList::move () {
    for (Missiles::iterator itr = missiles_.begin();
         itr != missiles_.end();
         ++itr) {

        itr->setX(itr->x() - Tower::speed());
        itr->setY(itr->y() - Missile::speed());

        if (itr->y() < 0) {
            itr = missiles_.erase(itr);
        }
    }
}

void MissileList::addMissile (int x, int y) {
    if (recharge_ < cooldown) {
        missiles_.push_back(Missile(x, y));
        recharge_ += 200;
    }
}

void MissileList::recharge () {
    if (recharge_) {
        --recharge_;
    }
}
