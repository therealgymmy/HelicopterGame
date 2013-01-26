#ifndef TOWER_H
#define TOWER_H

#include <list>
#include "attributes.h"
#include "missile.h"

class MLauncher : public attr::WHSizes  <MLauncher, attr::IsStatic>,
                  public attr::Drawable <MLauncher, attr::Rectangle, attr::Fill> {

friend class attr::XYCoordinates <MLauncher, attr::IsMember>;
friend class attr::WHSizes       <MLauncher, attr::IsStatic>;
friend class attr::Drawable      <MLauncher, attr::Rectangle, attr::Fill>;

public:
    typedef attr::XYCoordinates <MLauncher, attr::IsMember>              Coord;
    typedef attr::WHSizes       <MLauncher, attr::IsStatic>              Size;
    typedef attr::Drawable      <MLauncher, attr::Rectangle, attr::Fill> Draw;

private:
    static int w_;
    static int h_;
};

class Tower : public attr::XYCoordinates <Tower, attr::IsMember>,
              public attr::WHSizes       <Tower, attr::IsStatic>,
              public attr::Speed         <Tower, attr::IsStatic>,
              public attr::Drawable      <Tower, attr::Rectangle, attr::Border> {

friend class attr::XYCoordinates <Tower, attr::IsMember>;
friend class attr::WHSizes       <Tower, attr::IsStatic>;
friend class attr::Speed         <Tower, attr::IsStatic>;
friend class attr::Drawable      <Tower, attr::Rectangle, attr::Border>;

public:
    typedef attr::XYCoordinates <Tower, attr::IsMember>                Coord;
    typedef attr::WHSizes       <Tower, attr::IsStatic>                Size;
    typedef attr::Speed         <Tower, attr::IsStatic>                Speed;
    typedef attr::Drawable      <Tower, attr::Rectangle, attr::Border> Draw;


    Tower (int x, int y, int h, bool hasLauncher);

    void draw (XInfo *xinfo, int x);
    void fire (MissileList &mlist);

    void removeLauncher () { hasLauncher_ = false; }
    bool hasLauncher    () { return hasLauncher_; }

    int numBlocks () const { return numBlocks_; }

private:
    int x_;
    int y_;
    int numBlocks_;

    MLauncher launcher_;

    bool hasLauncher_;

    static int w_;
    static int h_;

    static int speed_;
    static const int speedMax_ = 10;
    static const int speedMin_ = 1;
};

class TowerList {
public:
    typedef std::list<Tower> Towers;

    TowerList ();

    void draw (XInfo *xinfo);
    void move ();
    void fire (MissileList &mlist);

    Towers& towers () { return towers_; }

private:
    Towers towers_;

    void addTower ();
};

#endif//TOWER_H
