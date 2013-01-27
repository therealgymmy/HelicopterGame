#include "error.h"
#include "helicopter.h"

int Helicopter::w_ = HeliHead::w() + HeliMid::w() + HeliEnd::w();
int Helicopter::h_ = HeliHead::h();
int Helicopter::speed_ = 10;
const int Helicopter::speedMax_;
const int Helicopter::speedMin_;
static int delayU = 0;
static int delayD = 0;
static int delayL = 0;
static int delayR = 0;

Helicopter::Helicopter ()
: up_(false),
  down_(false),
  left_(false),
  right_(false),
  delayUp_(0),
  delayDown_(0),
  delayLeft_(0),
  delayRight_(0),
  x_(500), y_(500),
  blink_(0) {}

void Helicopter::draw (XInfo *xinfo) {
    if (blink_ > 0) {
        --blink_;
        if (!(blink_ & 0x8)) {
            return;
        }
    }
    end_.draw<attr::BLACK>(xinfo,
                           x_ / Draw::ScaleX,
                           (y_ + (HeliHead::h() - HeliEnd::h()) / 2) / Draw::ScaleY);

    mid_.draw<attr::WHITE>(xinfo,
                           (x_ + HeliEnd::w()) / Draw::ScaleX,
                           (y_ + (HeliHead::h() - HeliMid::h()) / 2) / Draw::ScaleY);

    head_.draw<attr::WHITE>(xinfo,
                            (x_ + HeliEnd::w() + HeliMid::w()) / Draw::ScaleX,
                            y_ / Draw::ScaleY);

    top_.draw<attr::WHITE>(xinfo,
                           (x_ + HeliEnd::w() + HeliMid::w() - (HeliTop::w() - HeliHead::w()) / 2) / Draw::ScaleX,
                           (y_ - HeliTop::h()) / Draw::ScaleY);

    headin_.draw<attr::GREEN>(xinfo,
                              (x_ + HeliEnd::w() + HeliMid::w() + (HeliHead::w() - HeliHeadIn::w()) / 2) / Draw::ScaleX,
                              (y_ + (HeliHead::h() - HeliHeadIn::h()) / 2) / Draw::ScaleY);

    endin_.draw<attr::RED>(xinfo,
                           (x_ + (HeliEnd::w() - HeliEndIn::w()) / 2) / Draw::ScaleX,
                           (y_ + (HeliHead::h() - HeliEndIn::h()) / 2) / Draw::ScaleY);
}

void Helicopter::move () {
    if ((up_    || ++delayU < delayUp_  )  &&
        y_ > top_.h()) {
        y_ -= (speed_ - 0.3 * delayU);
    }

    if ((down_  || ++delayD < delayDown_)  &&
        y_ < PLANE_H) {
        y_ += (speed_ - 0.3 * delayD);
    }

    if ((left_  || ++delayL < delayLeft_)  &&
        x_ > 0) {
        x_ -= (speed_ - 0.3 * delayL);
    }

    if ((right_ || ++delayR < delayRight_) &&
        x_ < PLANE_W - w_) {
        x_ += (speed_ - 0.3 * delayR);
    }

    if (delayU >= delayUp_) {
        delayU = 0;
        delayUp_ = 0;
    }
    if (delayD >= delayDown_) {
        delayD = 0;
        delayDown_ = 0;
    }
    if (delayR >= delayRight_) {
        delayR = 0;
        delayRight_ = 0;
    }
    if (delayL >= delayLeft_) {
        delayL = 0;
        delayLeft_ = 0;
    }
}
