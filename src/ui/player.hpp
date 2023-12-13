#ifndef UI_PLAYER_HPP_
#define UI_PLAYER_HPP_

#include "field.hpp"
#include "game/player.hpp"

class PlayerUI
{
protected:
    Player& player;
    std::string message;
    std::string title;
public:
    explicit PlayerUI(Player& player) : player(player) {}
    Component drawMyField(const CoordSet& clickableCells = {}, passCoord onClick = nullptr, bool hideShips = false) const;
    Component drawEnemyField(const CoordSet& clickableCells = {}, passCoord onClick = nullptr, bool hideShips = true) const;
    Component combineFields(const Component& ownFieldEl, const Component& enemyFieldEl) const;

    void setMsg(std::string_view msg) { message = msg; }
    void clearMsg() { message = ""; }
    std::string_view getMsg() const { return message; }

    void setTitle(std::string_view msg) { title = msg; }
    void clearTitle() { title = ""; }
    std::string_view getTitle() const { return title; }

    Coord askCoord(bool ownField, const CoordSet& clickableCells = {}) const;

    Coord askShotCoord() const;
    Coord askPlaceShipCoord() const;

    void switchToNextPlayer() const;
    void showResult() const;
};

#endif // UI_PLAYER_HPP_
