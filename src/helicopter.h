#ifndef HELICOPTER_H
#define HELICOPTER_H

#include "attributes.h"

#define HeliClass(n, w, h, s, p) \
    INTERNAL_SHAPE_DECL(helicopter_##n, w, h, s, p) { \
        INTERNAL_SHAPE_INIT(helicopter_##n, w, h, s, p) \
    }

typedef class HeliClass(head, 240, 200, Ellipse,   Border) HeliHead;
typedef class HeliClass(hin,  220, 180, Ellipse,   Fill)   HeliHeadIn;
typedef class HeliClass(top,  360,  20, Rectangle, Fill)   HeliTop;
typedef class HeliClass(mid,  120,  20, Rectangle, Fill)   HeliMid;
typedef class HeliClass(end,  80,   80, Ellipse,   Border) HeliEnd;
typedef class HeliClass(ein,  60,   60, Ellipse,   Fill)   HeliEndIn;

#undef HeliClass

class Helicopter : public attr::XYCoordinates <Helicopter, attr::IsMember>,
                   public attr::WHSizes       <Helicopter, attr::IsStatic>,
                   public attr::Speed         <Helicopter, attr::IsStatic> {

friend class attr::XYCoordinates <Helicopter, attr::IsMember>;
friend class attr::WHSizes       <Helicopter, attr::IsStatic>;
friend class attr::Speed         <Helicopter, attr::IsStatic>;

public:
    typedef attr::XYCoordinates <Helicopter, attr::IsMember> Coord;
    typedef attr::WHSizes       <Helicopter, attr::IsStatic> Size;
    typedef attr::Speed         <Helicopter, attr::IsStatic> Speed;
    typedef attr::DrawableBase                               Draw;

    Helicopter ();

    void draw (XInfo *xinfo);
    void move ();
    void setBlink () { blink_ = 100; }

    bool up_;
    bool down_;
    bool left_;
    bool right_;

    int delayUp_;
    int delayDown_;
    int delayLeft_;
    int delayRight_;

private:
    int x_;
    int y_;

    // Helicopter components
    HeliHead head_;
    HeliMid  mid_;
    HeliEnd  end_;
    HeliTop  top_;
    HeliHeadIn headin_;
    HeliEndIn endin_;

    int blink_;

    static int w_;
    static int h_;

    static int speed_;
    static const int speedMax_ = 10;
    static const int speedMin_ = 1;
};

#endif//HELICOPTER_H
