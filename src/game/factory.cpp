#include "factory.hpp"

#include <stdexcept>

#include "player.hpp"
#include "one_ship/game.hpp"
#include "one_ship/players.hpp"
#include "memory.hpp"

shared_ptr<IGame> createGame(const GameOptions& options) {
    shared_ptr<IGame> game;

    switch (options.gameMode) {
    case GameMode::Classic:
        throw std::runtime_error("Classic mode is WIP");
    case GameMode::OneShip:
        auto player1 = MakeShared<OneShipScreenPlayer>();

        shared_ptr<OneShipPlayer> player2;
        switch (options.gameType) {
        case GameType::Singleplayer:
            player2 = MakeShared<OneShipEnemyPlayer>(options.difficulty);
            break;
        case GameType::Multiplayer:
            switch (options.multiplayerType) {
            case MultiplayerType::Host:
                // TODO: implement
                break;
            case MultiplayerType::Client:
                // TODO: implement
                break;
            case MultiplayerType::OneScreen:
                player2 = MakeShared<OneShipScreenPlayer>();
                break;
            }
            break;
        }

        if (!player2) {
            throw std::runtime_error("Player 2 is not initialized");
        }
        game = MakeShared<OneShipGame>(options.fieldSize, player1, player2, options.movesMap);
        break;
    }
    return game;
}
