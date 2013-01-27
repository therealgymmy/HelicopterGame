#include <cstdlib>
#include "error.h"
#include "tower.h"

int MLauncher::w_ = 40;
int MLauncher::h_ = 80;

int Tower::w_ = 160;
int Tower::h_ = 120;
int Tower::speed_ = 5;
const int Tower::speedMax_;
const int Tower::speedMin_;

Tower::Tower (int x, int y, int h, bool hasLauncher)
: x_(x), y_(y), numBlocks_(h), hasLauncher_(hasLauncher) {}


void Tower::draw (XInfo *xinfo, int x) {
    int y = y_ / ScaleY;
    int height = Tower::Size::h() / ScaleY;

    // temporary fix
    // adjust y to align properly along bottom of screen
    y = xinfo->attr_.height - numBlocks_ * height;

    for (int i = 0; i < numBlocks_ ; ++i) {
        reinterpret_cast<Fill&>(*this).draw<attr::NAVAJO>(xinfo, x, y + i * height);
        Draw::draw<attr::BLACK>(xinfo, x, y + i * height);
        if (!i && hasLauncher_) {
            // draw launcher
            launcher_.draw<attr::GREY>(
                    xinfo,
                    x + ((Tower::Size::w() - MLauncher::Size::w()) / 2) / ScaleX,
                    y);
        }
    }
}

void Tower::fire (MissileList &mlist) {
    mlist.addMissile(x_ + (Tower::Size::w() - Missile::Size::w()) / 2, y_);
}

TowerList::TowerList () {
    addTower();
}

void TowerList::draw (XInfo *xinfo) {
    TowerList::Towers::iterator first = towers_.begin();
    int firstx = first->x() / Tower::ScaleX;
    int firstw = Tower::Size::w() / Tower::ScaleX;
    for (TowerList::Towers::iterator itr = towers_.begin();
         itr != towers_.end();
         ++itr) {
        itr->draw(xinfo, firstx);
        firstx += firstw;
    }
}

void TowerList::move () {
    Towers::iterator last  = --towers_.end();
    Towers::iterator first = towers_.begin();
    Towers::iterator itr = towers_.begin();

    if (last->x() <= PLANE_W - Tower::Size::w()) {
        addTower();
    }

    for (; itr != towers_.end(); ++itr) {
        itr->setX(itr->x() - Tower::speed());
    }

    if (first->x() <= 0) {
        towers_.erase(first);
    }
}

void TowerList::addTower () {
    int h = rand() % 10 + 1;
    int m = rand() % 5;
    bool mlauncher = false;

    if (!m) {
        mlauncher = true;
    }

    towers_.push_back(Tower(PLANE_W,
                            PLANE_H - h * Tower::Size::h(),
                            h,
                            mlauncher));
}

void TowerList::fire (MissileList &mlist) {
    for (Towers::iterator itr = towers_.begin();
         itr != towers_.end();
         ++itr) {
        if (itr->hasLauncher() && !(rand() % 100)) {
            itr->fire(mlist);
        }
    }
}
