#ifndef BOMB_H
#define BOMB_H

#include <list>
#include "attributes.h"

#define BombClass(n, w, h, s, p) \
    INTERNAL_SHAPE_DECL(bomb_##n, w, h, s, p) { \
        INTERNAL_SHAPE_INIT(bomb_##n, w, h, s, p) \
    }

typedef class BombClass(top,    60,  20, Ellipse, Fill) BombTop;
typedef class BombClass(mid,    90,  30, Ellipse, Fill) BombMid;
typedef class BombClass(bottom, 120, 40, Ellipse, Fill) BombBtm;

#undef BombClass

class Bomb : public attr::XYCoordinates <Bomb, attr::IsMember>,
             public attr::WHSizes       <Bomb, attr::IsStatic>,
             public attr::Speed         <Bomb, attr::IsStatic>,
             public attr::Drawable      <Bomb, attr::Ellipse, attr::Fill> {

friend class attr::XYCoordinates <Bomb, attr::IsMember>;
friend class attr::WHSizes       <Bomb, attr::IsStatic>;
friend class attr::Speed         <Bomb, attr::IsStatic>;
friend class attr::Drawable      <Bomb, attr::Ellipse, attr::Fill>;

public:
    typedef attr::XYCoordinates <Bomb, attr::IsMember>            Coord;
    typedef attr::WHSizes       <Bomb, attr::IsStatic>            Size;
    typedef attr::Speed         <Bomb, attr::IsStatic>            Speed;
    typedef attr::Drawable      <Bomb, attr::Ellipse, attr::Fill> Draw;

    enum Inertia {
        NUL,
        FORWARD,
        BACKWARD,
    };

    Bomb (int x, int y, Inertia inertia);

    void draw (XInfo *xinfo, int x, int y);

    Inertia hasInertia () { return inertia_; }

private:
    int x_;
    int y_;
    Inertia inertia_;
    BombTop top_;
    BombMid mid_;
    BombBtm btm_;

    static int w_;
    static int h_;

    static int speed_;
    static const int speedMax_ = 10;
    static const int speedMin_ = 1;
};

class BombList {
public:
    typedef std::list<Bomb> Bombs;

    BombList ();

    void draw (XInfo *xinfo);
    void move ();
    void addBomb (int x, int y, Bomb::Inertia inertia);
    void recharge ();

    Bombs& bombs () { return bombs_; }

private:
    Bombs bombs_;
    int recharge_;
};

#endif//BOMB_H
