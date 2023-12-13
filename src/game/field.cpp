#include "field.hpp"
#include <cmath>
#include <memory>

Field::Field(Size size) : size(size)
{
    cells.resize(size.first, std::vector<Cell>(size.second, Cell::EMPTY));
}


Cell Field::getCell(Coord coord) const
{
    return cells[coord.first][coord.second];
}

void Field::setCell(Coord coord, Cell cell)
{
    cells[coord.first][coord.second] = cell;
}

Size Field::getSize() const
{
    return size;
}

Cell Field::shot(Coord coord)
{
    switch (Cell cell = getCell(coord))
    {
    case Cell::EMPTY:
        setCell(coord, Cell::MISS);
        break;
    case Cell::ALIVE:
        setCell(coord, Cell::DEAD);
        break;
    default: // should not happen
        break;
    }
    return getCell(coord);
}

bool Field::isEmpty(Coord coord) const
{
    return cells[coord.first][coord.second] == Cell::EMPTY;
}

bool Field::isAlive(Coord coord) const
{
    return cells[coord.first][coord.second] == Cell::ALIVE;
}

bool Field::isMiss(Coord coord) const
{
    return cells[coord.first][coord.second] == Cell::MISS;
}

bool Field::isDead(Coord coord) const
{
    return cells[coord.first][coord.second] == Cell::DEAD;
}

bool Field::canShot(Coord coord) const
{
    return isOnField(coord) && (isEmpty(coord) || isAlive(coord));
}

bool Field::isGameOver() const
{
    Coord coord;
    for (int i = 0; i < size.first; i++)
    {
        for (int j = 0; j < size.second; j++)
        {
            coord = Coord(i, j);
            if (isAlive(coord))
            {
                return false;
            }
        }
    }
    return true;
}

bool Field::isOnField(Coord coord) const
{
    return coord.first >= 0 && coord.first < size.first && coord.second >= 0 && coord.second < size.second;
}

CoordSet Field::getAvailableShots() const
{
    CoordSet availableShots;
    Coord coord;
    for (int i = 0; i < size.first; i++)
    {
        for (int j = 0; j < size.second; j++)
        {
            coord = Coord(i, j);
            if (canShot(coord))
            {
                availableShots.insert(coord);
            }
        }
    }
    return availableShots;
}

CoordSet Field::getAvailableMoves(Coord coord, const CoordSet& movesMap) const
{
    CoordSet availableMoves;
    Coord move;
    for (auto [x, y] : movesMap)
    {
        move = Coord(coord.first + x, coord.second + y);
        if (!isOnField(move))
        {
            continue;
        }
        if (coord == move || isEmpty(move))
        {
            availableMoves.insert(move);
        }
    }
    return availableMoves;
}

int Field::distanceToNearestShip(Coord coord) const
{
    int distance = size.first + size.second + 1;
    CoordSet availableShots = getAvailableShots();
    for (Coord shot : availableShots)
    {
        if (!isAlive(shot))
            continue;
        distance = std::min(distance, manhattanDistance(coord, shot));
    }
    return distance;
}

int Field::manhattanDistance(Coord coord1, Coord coord2)
{
    return abs(coord1.first - coord2.first) + abs(coord1.second - coord2.second);
}
