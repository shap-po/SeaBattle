#ifndef GAME_PLAYER_HPP_
#define GAME_PLAYER_HPP_

#include "field.hpp"
#include "types.hpp"
#include "memory.hpp"

#include <string>

class Player
{
protected:
    bool isComputer = false;
    bool isHost = false;

    shared_ptr<Field> field;
    shared_ptr<Field> enemyField; // field with hidden ships
    Coord lastShot = Coord(-1, -1);
    Coord lastEnemyShot = Coord(-1, -1);
public:
    Player(bool isComputer, bool isHost) : isComputer(isComputer), isHost(isHost) {}
    Player(const Player&) = default;
    virtual ~Player() = default;

    bool isComputerPlayer() const { return isComputer; }
    bool isHostPlayer() const { return isHost; }

    Coord getLastShot() const { return lastShot; }
    void setLastShot(Coord coord) { lastShot = coord; }
    Coord getLastEnemyShot() const { return lastEnemyShot; }
    void setLastEnemyShot(Coord coord) { lastEnemyShot = coord; }

    shared_ptr<Field> getField() const { return field; }
    shared_ptr<Field> getEnemyField() const { return enemyField; }
    void setField(shared_ptr<Field> newField) { field = newField; }
    void setEnemyField(shared_ptr<Field> newField) { enemyField = newField; }

    virtual Coord makeShot() = 0;

    virtual void setShotResult(Coord coord, Cell cell) { enemyField->setCell(coord, cell);lastShot = coord; }
    virtual void setEnemyShotResult(Coord coord, Cell cell) { field->setCell(coord, cell);lastEnemyShot = coord; }

    virtual void communicateMessage(const std::string& message) {}; // regular messages
    virtual void communicateTitle(const std::string& title) {}; // screen title (for split screen)
    virtual void communicateWarning(const std::string& warning) { communicateMessage(warning); } // for invalid data / bot error detection

    virtual void switchToNextPlayer() const {} // for two players on one screen only
    virtual void showResult() const {} // for human players only
};

#endif // GAME_PLAYER_HPP_
