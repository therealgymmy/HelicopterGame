#include "bomb.h"
#include "error.h"
#include "tower.h"

int Bomb::w_ = BombBtm::w();
int Bomb::h_ = (BombTop::h() + BombMid::h()) * 0.8 + BombBtm::h();
int Bomb::speed_ = 5;
const int Bomb::speedMax_;
const int Bomb::speedMin_;

static int cooldown = 20;

Bomb::Bomb (int x, int y, Bomb::Inertia inertia)
: x_(x), y_(y), inertia_(inertia) {}

void Bomb::draw (XInfo *xinfo, int x, int y) {
    int tx = x + (BombBtm::w() - BombTop::w()) / 2;
    int mx = x + (BombBtm::w() - BombMid::w()) / 2;
    int bx = x;

    int ty = y;
    int my = ty + BombTop::h() * 0.8;
    int by = my + BombMid::h() * 0.8;

    top_.draw<attr::BROWN>(xinfo,
                           tx / Bomb::ScaleX,
                           ty / Bomb::ScaleY);

    mid_.draw<attr::BROWN>(xinfo,
                           mx / Bomb::ScaleX,
                           my / Bomb::ScaleY);

    btm_.draw<attr::BROWN>(xinfo,
                           bx / Bomb::ScaleX,
                           by / Bomb::ScaleY);
}

BombList::BombList ()
: recharge_(0) {}

void BombList::draw (XInfo *xinfo) {
    for (Bombs::iterator itr = bombs_.begin();
         itr != bombs_.end();
         ++itr) {
        itr->draw(xinfo, itr->x(), itr->y());
    }
}

void BombList::move () {
    for (Bombs::iterator itr = bombs_.begin();
         itr != bombs_.end();
         ++itr) {

        if (itr->hasInertia() == Bomb::NUL) {
            itr->setX(itr->x() - Tower::speed() / 2);
        }
        else if (itr->hasInertia() == Bomb::FORWARD) {
            itr->setX(itr->x() - Tower::speed() / 3);
        }
        else {
            itr->setX(itr->x() - Tower::speed());
        }

        itr->setY(itr->y() + Bomb::speed());

        if (itr->y() > PLANE_H) {
            itr = bombs_.erase(itr);
        }
    }
}

void BombList::addBomb (int x, int y, Bomb::Inertia inertia) {
    if (!recharge_) {
        bombs_.push_back(Bomb(x, y, inertia));
        recharge_ = cooldown;
    }
}

void BombList::recharge () {
    if (recharge_) {
        --recharge_;
    }
}
