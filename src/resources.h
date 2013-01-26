#ifndef RESOURCES_H
#define RESOURCES_H

#include "bomb.h"
#include "explosion.h"
#include "helicopter.h"
#include "misc.h"
#include "missile.h"
#include "tower.h"

struct Resources {
    BombList      bombs_;
    ExplosionList explosions_;
    Helicopter    heli_;
    MissileList   missiles_;
    TowerList     towers_;

    HealthBar     healthBar_;
};

#endif//RESOURCES_H
