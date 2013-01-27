#ifndef MISC_H
#define MISC_H

#include "attributes.h"

#define HpClass(n, w, h, s, p) \
    INTERNAL_SHAPE_DECL(hp_##n, w, h, s, p) { \
        INTERNAL_SHAPE_INIT(hp_##n, w, h, s, p) \
    }

typedef class HpClass(frame, 840, 100, Rectangle, Fill) HpFrame;

#undef HpClass

class HealthBar : public attr::XYCoordinates <HealthBar, attr::IsMember>,
                  public attr::WHSizes       <HealthBar, attr::IsMember>,
                  public attr::Drawable      <HealthBar, attr::Rectangle, attr::Fill> {

friend class attr::XYCoordinates <HealthBar, attr::IsMember>;
friend class attr::WHSizes       <HealthBar, attr::IsMember>;
friend class attr::Drawable      <HealthBar, attr::Rectangle, attr::Fill>;

public:
    typedef attr::XYCoordinates <HealthBar, attr::IsMember>              Coord;
    typedef attr::WHSizes       <HealthBar, attr::IsMember>              Size;
    typedef attr::Drawable      <HealthBar, attr::Rectangle, attr::Fill> Draw;

    HealthBar ();

    void draw (XInfo *xinfo);

    int health () const { return hp_; }

    void restore   () { w_ = (hp_ = 100) * 8; }
    void incHealth () { if (hp_ < 100) w_ = (hp_ += 10) * 8; }
    void decHealth () { if (hp_ > 0)   w_ = (hp_ -= 10) * 8; }

private:
    int x_;
    int y_;
    int w_;
    int h_;

    int hp_;

    HpFrame frame_;
};

class Score : public attr::XYCoordinates <Score, attr::IsStatic> {

friend class attr::XYCoordinates <Score, attr::IsStatic>;

public:
    typedef attr::XYCoordinates <Score, attr::IsStatic> Coord;

    Score ();

    void draw (XInfo *xinfo);

    int score () const { return score_; }

    void incScore (int i) { score_ += i; }
    void decScore (int i) { score_ -= i; }

private:
    int score_;

    static int x_;
    static int y_;
};

#endif//MISC_H
