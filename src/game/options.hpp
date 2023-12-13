#ifndef GAME_OPTIONS_HPP_
#define GAME_OPTIONS_HPP_

#include "types.hpp"

#include <string>
#include <vector>

using entries = std::vector<std::string>;

const auto minFieldSize = 4;
const auto maxFieldSize = 25;
const auto fieldSizes = std::vector<Size>{
    {10, 10},
    {15, 15},
    {20, 20},
};
const auto fieldSizeEntries = []() {
    entries result;
    for (const auto& [w, h] : fieldSizes)
    {
        result.push_back(std::to_string(w) + "x" + std::to_string(h));
    }
    result.emplace_back("custom");
    return result;
    }
();

enum class GameMode {
    Classic,
    OneShip,
};
const entries gameModeEntries = { "Classic", "One ship" };

enum class GameType {
    Singleplayer,
    Multiplayer,
};
const entries gameTypeEntries = { "Singleplayer", "Multiplayer" };

enum class MultiplayerType {
    Host,
    Client,
    OneScreen,
};
const entries multiplayerTypeEntries = { "Host", "Client", "One screen" };

const CoordSet movesMap8 = {
    Coord(0, 0),
    Coord(-1, -1),
    Coord(-1, 0),
    Coord(-1, 1),
    Coord(0, -1),
    Coord(0, 1),
    Coord(1, -1),
    Coord(1, 0),
    Coord(1, 1),
};
// moves map 4 directions
const CoordSet movesMap4 = {
    Coord(0, 0),
    Coord(-1, 0),
    Coord(0, -1),
    Coord(0, 1),
    Coord(1, 0),
};

enum class Difficulty {
    Easy,
    Medium,
    Hard,
};
const entries difficultyEntries = { "Easy", "Medium", "Hard" };


struct GameOptions
{
    GameMode gameMode;
    GameType gameType;
    Size fieldSize;

    // one ship
    CoordSet movesMap;

    // singleplayer only
    Difficulty difficulty;

    // multiplayer only
    MultiplayerType multiplayerType;
    std::string ip;
};

#endif // GAME_OPTIONS_HPP_
