#include "random.hpp"
#include "ui/menu.hpp"
#include "game/factory.hpp"

using namespace std;

#define testMode

int main()
{
    setSeed();

#ifndef testMode
    auto const& options = getGameOptions();
#else
#include "game/options.hpp"
    GameOptions options{
        .gameMode = GameMode::OneShip,
        .gameType = GameType::Singleplayer,
        .fieldSize = {10, 10},
        .movesMap = movesMap4,
        .difficulty = Difficulty::Hard,
        .multiplayerType = MultiplayerType::OneScreen,
    };
#endif

    auto game = createGame(options);
    game->start();

    return 0;
}
