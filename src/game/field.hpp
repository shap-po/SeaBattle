#ifndef GAME_FIELD_HPP_
#define GAME_FIELD_HPP_

#include <vector>

#include "types.hpp"
#include "../memory.hpp"

class Field
{
private:
    Size size;
    std::vector<std::vector<Cell>> cells;

public:
    explicit Field(Size size);
    ~Field() = default;

    Cell getCell(Coord coord) const;
    void setCell(Coord coord, Cell cell);
    Size getSize() const;
    Cell shot(Coord coord);
    bool isEmpty(Coord coord) const;
    bool isAlive(Coord coord) const;
    bool isMiss(Coord coord) const;
    bool isDead(Coord coord) const;
    bool canShot(Coord coord) const;
    bool isGameOver() const;
    bool isOnField(Coord coord) const;
    CoordSet getAvailableShots() const;

    CoordSet getAvailableMoves(Coord coord, const CoordSet& movesMap) const;
    int distanceToNearestShip(Coord coord) const;
    static int manhattanDistance(Coord coord1, Coord coord2);
};

#endif // GAME_FIELD_HPP_
