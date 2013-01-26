#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <list>
#include "attributes.h"

class Explosion : public attr::XYCoordinates <Explosion, attr::IsMember>,
                  public attr::WHSizes       <Explosion, attr::IsMember>,
                  public attr::Speed         <Explosion, attr::IsStatic>,
                  public attr::Drawable      <Explosion, attr::Ellipse, attr::Fill> {

friend class attr::XYCoordinates <Explosion, attr::IsMember>;
friend class attr::WHSizes       <Explosion, attr::IsMember>;
friend class attr::Speed         <Explosion, attr::IsStatic>;
friend class attr::Drawable      <Explosion, attr::Ellipse, attr::Fill>;

public:
    typedef attr::XYCoordinates <Explosion, attr::IsMember>            Coord;
    typedef attr::WHSizes       <Explosion, attr::IsMember>            Size;
    typedef attr::Speed         <Explosion, attr::IsStatic>            Speed;
    typedef attr::Drawable      <Explosion, attr::Ellipse, attr::Fill> Draw;

    Explosion (int x, int y);

    void draw    (XInfo *xinfo, int x, int y);
    void inflate ();

private:
    int x_;
    int y_;

    int w_;
    int h_;
    static int Speed_;
    static const int SpeedMax_ = 10;
    static const int SpeedMin_ = 1;
};

class ExplosionList {
public:
    typedef std::list<Explosion> Explosions;

    void draw (XInfo *xinfo);
    void move ();
    void addExplosion (int x, int y);

    Explosions& explosions () { return explosions_; }

private:
    Explosions explosions_;
};

#endif//EXPLOSION_H
