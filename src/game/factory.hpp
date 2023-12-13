#ifndef GAME_FACTORY_HPP_
#define GAME_FACTORY_HPP_

#include "game.hpp"
#include "options.hpp"
#include "../memory.hpp"

shared_ptr<IGame> createGame(const GameOptions& options);

#endif // GAME_FACTORY_H
