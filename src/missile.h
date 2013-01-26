#ifndef MISSILE_H
#define MISSILE_H

#include <list>
#include "attributes.h"

class Missile : public attr::XYCoordinates <Missile, attr::IsMember>,
                public attr::WHSizes       <Missile, attr::IsStatic>,
                public attr::Speed         <Missile, attr::IsStatic>,
                public attr::Drawable      <Missile, attr::Rectangle, attr::Fill> {

friend class attr::XYCoordinates <Missile, attr::IsMember>;
friend class attr::WHSizes       <Missile, attr::IsStatic>;
friend class attr::Speed         <Missile, attr::IsStatic>;
friend class attr::Drawable      <Missile, attr::Rectangle, attr::Fill>;

public:
    typedef attr::XYCoordinates <Missile, attr::IsMember>              Coord;
    typedef attr::WHSizes       <Missile, attr::IsStatic>              Size;
    typedef attr::Speed         <Missile, attr::IsStatic>              Speed;
    typedef attr::Drawable      <Missile, attr::Rectangle, attr::Fill> Draw;

    Missile (int x, int y);

    static int w_;
    static int h_;

private:
    int x_;
    int y_;

    static int speed_;
    static const int speedMax_ = 10;
    static const int speedMin_ = 1;
};

class MissileList {
public:
    typedef std::list<Missile> Missiles;

    MissileList ();

    void draw (XInfo *xinfo);
    void move ();
    void addMissile (int x, int y);
    void recharge ();

    Missiles& missiles () { return missiles_; }

private:
    Missiles missiles_;
    int recharge_;
};

#endif//MISSILE_H
