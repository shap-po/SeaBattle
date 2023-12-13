#ifndef GAME_ONE_SHIP_ENEMY_PLAYER_HPP_
#define GAME_ONE_SHIP_ENEMY_PLAYER_HPP_

// #define SHOW_ENEMY_DEBUG_STRATEGY // shows strategy on the screen

#include "player.hpp"
#include "game/options.hpp"

#ifdef SHOW_ENEMY_DEBUG_STRATEGY
#include "ui/player.hpp"
#endif

class OneShipEnemyPlayer : public OneShipPlayer
{
private:
    Difficulty difficulty;
    CoordSet possibleShipLocations;

#ifdef SHOW_ENEMY_DEBUG_STRATEGY
    PlayerUI ui;
public:
    explicit OneShipEnemyPlayer(Difficulty difficulty) : OneShipPlayer(true, false), difficulty(difficulty), ui(*this) {}
    void showStrategy(const CoordSet& shots) const;
#else
public:
    explicit OneShipEnemyPlayer(Difficulty difficulty) : OneShipPlayer(true, false), difficulty(difficulty) {}
#endif
    Coord placeShip() const override;
    Coord makeShot() override;
    Coord makeMove() const override;

    void expandPossibleShipLocations();
    void tryFindShip();
    CoordSet findBestShots() const;
    Coord chooseShot(const CoordSet& shots) const;
};

#endif // GAME_ONE_SHIP_ENEMY_PLAYER_HPP_
