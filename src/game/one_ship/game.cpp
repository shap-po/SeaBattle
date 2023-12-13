#include "game.hpp"


void OneShipGame::playerTurn()
{
    auto player = currentPlayer();
    auto enemyPlayer = this->enemyPlayer();
    auto enemyField = this->enemyField();
    auto field = this->currentField();

    Game::playerTurn(); // make shot

    // get manhattan distance to nearest ship
    int distance = enemyField->distanceToNearestShip(player->getLastShot());
    player->distanceFromLastShot = distance;

    if (enemyField->isGameOver()) return; // skip move if game is over

    player->communicateMessage("Distance to the ship: " + std::to_string(distance));

    // move ship
    auto move = Coord(-1, -1);
    auto availableMoves = field->getAvailableMoves(player->shipCoord, movesMap);
    while (move == Coord(-1, -1))
    {
        move = player->makeMove();
        if (availableMoves.find(move) == availableMoves.end())
        {
            player->communicateWarning("You can't move to this cell");
            move = Coord(-1, -1);
        }
    }
    field->setCell(player->shipCoord, Cell::EMPTY);
    field->setCell(move, Cell::ALIVE);
    player->shipCoord = move;
}

void OneShipGame::playerPlaceShip() {
    auto player = currentPlayer();
    auto field = this->currentField();
    auto shipCoord = Coord(-1, -1);
    while (shipCoord == Coord(-1, -1))
    {
        shipCoord = player->placeShip();
        if (!field1->isOnField(shipCoord)) {
            player->communicateWarning("You can't place ship here");
            shipCoord = Coord(-1, -1);
        }
    }
    field->setCell(shipCoord, Cell::ALIVE);
    player->shipCoord = shipCoord;
    nextTurn();
}

void OneShipGame::prepare() {
    playerPlaceShip();
    playerPlaceShip();
}

