#ifndef GAME_BASE_HPP_
#define GAME_BASE_HPP_

#include <utility>
#include <set>

using uint = unsigned int;

using Coord = std::pair<int, int>;
using Size = std::pair<uint, uint>;

using CoordSet = std::set<Coord>;

enum class Cell
{
    EMPTY = 0,
    ALIVE = 1,
    MISS = 2,
    DEAD = 3,
};

#endif // GAME_BASE_HPP_
