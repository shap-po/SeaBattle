#ifndef GAME_GAME_HPP_
#define GAME_GAME_HPP_

#include "types.hpp"
#include "field.hpp"
#include "player.hpp"

#include "../random.hpp"
#include "../memory.hpp"
#include <iostream>


class IGame {
public:
    virtual ~IGame() = default;
    virtual void start() = 0;
};

template <class PlayerType>
class Game : public IGame
{
protected:
    shared_ptr<PlayerType> player1;
    shared_ptr<PlayerType> player2;
    shared_ptr<Field> field1;
    shared_ptr<Field> field2;
    bool isPlayer1Move;


    shared_ptr<PlayerType> getWinner() const {
        if (field1->isGameOver()) return player2;
        if (field2->isGameOver()) return player1;
        return nullptr;
    }
    shared_ptr<PlayerType> getLoser() const {
        if (field1->isGameOver()) return player1;
        if (field2->isGameOver()) return player2;
        return nullptr;
    }

public:
    Game(
        Size fieldSize,
        shared_ptr<PlayerType> player1, shared_ptr<PlayerType> player2) :
        player1(player1),
        player2(player2),
        isPlayer1Move(player2->isComputerPlayer() ? true : randInt(1)),
        field1(MakeShared<Field>(fieldSize)),
        field2(MakeShared<Field>(fieldSize))
    {
        player1->setField(field1);
        player1->setEnemyField(field2);

        player2->setField(field2);
        player2->setEnemyField(field1);
    }
    ~Game() override = default;

    bool isSplitScreen() const { return player1->isHostPlayer() && player2->isHostPlayer(); }

    void nextTurn() { isPlayer1Move = !isPlayer1Move; }
    shared_ptr<PlayerType> currentPlayer() const { return isPlayer1Move ? player1 : player2; }
    shared_ptr<PlayerType> enemyPlayer() const { return isPlayer1Move ? player2 : player1; }
    shared_ptr<Field> currentField() const { return isPlayer1Move ? field1 : field2; }
    shared_ptr<Field> enemyField() const { return isPlayer1Move ? field2 : field1; }

    virtual void playerTurn() {
        auto player = currentPlayer();
        auto enemyPlayer = this->enemyPlayer();
        auto enemyField = this->enemyField();
        if (isSplitScreen())
        {
            player->communicateTitle("It's player " + std::to_string(isPlayer1Move ? 1 : 2) + " turn");
        }
        Coord shot;
        while (true)
        {
            shot = player->makeShot();
            if (enemyField->canShot(shot))
            {
                break;
            }
            player->communicateWarning("You can't shot here!");
        }
        Cell cell = enemyField->shot(shot);
        player->setShotResult(shot, cell);
        enemyPlayer->setEnemyShotResult(shot, cell);
    }

    void start() override {
        prepare();
        gameLoop();
        end();
    }
    virtual void prepare() {}
    virtual void end() {
        getWinner()->communicateMessage("You win!");
        getLoser()->communicateMessage("You lose!");

        if (isSplitScreen()) currentPlayer()->showResult();
        else player1->showResult();
    }
    virtual void gameLoop() {
        while (true)
        {
            playerTurn();

            if (enemyField()->isGameOver()) break;

            if (isSplitScreen()) currentPlayer()->switchToNextPlayer();
            nextTurn();
        }
    }
};

#endif // GAME_GAME_HPP_
