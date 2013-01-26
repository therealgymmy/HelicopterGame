#include <cstdlib>
#include <unistd.h>

#include "error.h"
#include "control.h"

static
void processMovement (ControlArg *controlArg);

static
void detectCollision (BombList &bombs,
                      TowerList &towers,
                      ExplosionList &explosions);

static
void detectCollision (MissileList &mlist,
                      Helicopter &heli,
                      HealthBar &hp);

static
void detectCollision (Helicopter &heli, TowerList &towers);

void* control (void *controlArg) {
    processMovement(static_cast<ControlArg*>(controlArg));
    return NULL;
}

void processMovement (ControlArg *controlArg) {
    Resources       *resources = controlArg->resources_;
    XHandler        *xhandler  = controlArg->xhandler_;
    pthread_mutex_t *mutex     = controlArg->mutex_;

    BombList      &bombs      = resources->bombs_;
    ExplosionList &explosions = resources->explosions_;
    Helicopter    &heli       = resources->heli_;
    TowerList     &towers     = resources->towers_;
    MissileList   &missiles   = resources->missiles_;
    HealthBar     &hp         = resources->healthBar_;

    while (true) {
        if (!xhandler->showCredit()) {
            if (pthread_mutex_lock(mutex) == -1) {
                error("cannot lock mutex in control thread");
            }

            // helicopter
            heli.move();

            // explosions
            explosions.move();

            // towers
            towers.move();
            towers.fire(missiles);

            // missiles
            missiles.move();
            missiles.recharge();

            // bombs
            bombs.move();
            bombs.recharge();

            // collision detection
            detectCollision(bombs, towers, explosions);
            detectCollision(missiles, heli, hp);
            detectCollision(heli, towers);

            if (pthread_mutex_unlock(mutex) == -1) {
                error("cannot unlock mutex in control thread");
            }
        }
        usleep(1000 * 15);
    }
}

void detectCollision (BombList &bombs,
                      TowerList &towers,
                      ExplosionList &explosions) {
    TowerList::Towers         &towerlist = towers.towers();
    BombList::Bombs           &bomblist  = bombs.bombs();

    for (TowerList::Towers::iterator t_itr = towerlist.begin();
         t_itr != towerlist.end();
         ++t_itr) {
        int th = t_itr->numBlocks();
        int tx = t_itr->x();
        for (BombList::Bombs::iterator b_itr = bomblist.begin();
             b_itr != bomblist.end();
             ++b_itr) {
            int bx = b_itr->x();
            int by = b_itr->y();

            if (bx >= tx - Bomb::Size::w() &&
                bx <= tx + Tower::Size::w() &&
                by >= PLANE_H - th * Tower::Size::h() - Bomb::Size::h() &&
                by <= PLANE_H) {
                b_itr = bomblist.erase(b_itr);

                t_itr->removeLauncher();

                explosions.addExplosion(bx + Bomb::w(),
                                        by + Bomb::h());
            }
        }
    }
}

void detectCollision (MissileList &mlist,
                      Helicopter &heli,
                      HealthBar &hp) {
    MissileList::Missiles &missiles = mlist.missiles();

    for (MissileList::Missiles::iterator itr = missiles.begin();
         itr != missiles.end();
         ++itr) {
        int mx = itr->x();
        int my = itr->y();
        int hx = heli.x();
        int hy = heli.y();

        if (hx >= mx - Helicopter::Size::w() &&
            hx <= mx + Missile::Size::w() &&
            hy >= my - Helicopter::Size::h() &&
            hy <= my + Missile::Size::h()) {
            hp.decHealth();
            if (hp.health() <= 0) {
                heli.setX(500);
                heli.setY(500);
                hp.restore();
            }

            itr = missiles.erase(itr);
        }
    }
}

void detectCollision (Helicopter &heli, TowerList &towers) {
    TowerList::Towers &towerlist = towers.towers();
    int hx = heli.x();
    int hy = heli.y();

    for (TowerList::Towers::iterator itr = towerlist.begin();
         itr != towerlist.end();
         ++itr) {
        int th = itr->numBlocks();
        int tx = itr->x();
        if (hx >= tx - Helicopter::Size::w() &&
            hx <= tx + Tower::Size::w() &&
            hy >= PLANE_H - th * Tower::Size::h() - Helicopter::Size::h() &&
            hy <= PLANE_H) {
            heli.setX(500);
            heli.setY(500);
        }
    }
}
