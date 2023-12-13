#ifndef GAME_ONE_SHIP_PLAYER_HPP_
#define GAME_ONE_SHIP_PLAYER_HPP_

#include "game/player.hpp"

class OneShipPlayer : public Player
{
public:
    using Player::Player;

    virtual Coord placeShip() const = 0;
    virtual Coord makeMove() const = 0;

    int distanceFromLastShot = 0;
    Coord shipCoord = Coord(-1, -1);
    CoordSet movesMap;
};



#endif // GAME_ONE_SHIP_PLAYER_HPP_
