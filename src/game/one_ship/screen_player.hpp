#ifndef GAME_ONE_SHIP_SCREEN_PLAYER_HPP_
#define GAME_ONE_SHIP_SCREEN_PLAYER_HPP_

#include "player.hpp"
#include "ui/player.hpp"

class OneShipScreenPlayer : public OneShipPlayer
{
private:
    PlayerUI ui;
public:
    explicit OneShipScreenPlayer() : OneShipPlayer(false, true), ui(*this) {}
    Coord placeShip() const override;
    Coord makeShot() override;
    Coord makeMove() const override;

    void communicateMessage(const std::string& message) override;
    void communicateTitle(const std::string& title) override;

    void switchToNextPlayer() const override { ui.switchToNextPlayer(); }
    void showResult() const override { ui.showResult(); }
};

#endif // GAME_ONE_SHIP_SCREEN_PLAYER_HPP_
