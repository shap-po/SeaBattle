#include "screen_player.hpp"

#include "ui/field.hpp"

using std::string;

Coord OneShipScreenPlayer::makeShot()
{
    return ui.askShotCoord();
}
Coord OneShipScreenPlayer::makeMove() const
{
    return ui.askCoord(true, field->getAvailableMoves(shipCoord, movesMap));
}
Coord OneShipScreenPlayer::placeShip() const
{
    return ui.askPlaceShipCoord();
}

void OneShipScreenPlayer::communicateMessage(const string& message)
{
    ui.setMsg(message);
}
void OneShipScreenPlayer::communicateTitle(const string& title)
{
    ui.setTitle(title);
}

