#ifndef GAME_ONE_SHIP_GAME_HPP_
#define GAME_ONE_SHIP_GAME_HPP_

#include "../game.hpp"
#include "player.hpp"

#include "../../memory.hpp"

template class Game<OneShipPlayer>;

class OneShipGame : public Game<OneShipPlayer>
{
protected:
    CoordSet movesMap;

public:
    OneShipGame(
        Size fieldSize,
        shared_ptr<OneShipPlayer> player1, shared_ptr<OneShipPlayer> player2,
        const CoordSet& movesMap)
        : Game(fieldSize, player1, player2), movesMap(movesMap) {
        player1->movesMap = movesMap;
        player2->movesMap = movesMap;
    }

    void prepare() override;
    void playerTurn() override;

    void playerPlaceShip();
};

#endif // GAME_ONE_SHIP_GAME_HPP_
