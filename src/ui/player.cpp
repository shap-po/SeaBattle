#include "player.hpp"

Component PlayerUI::drawMyField(
    const CoordSet& clickableCells,
    passCoord onClick,
    bool hideShips
) const {
    return FieldComponent(
        player.getField().get(),
        "My field",
        hideShips,
        player.getLastEnemyShot(),
        clickableCells,
        onClick
    );
}
Component PlayerUI::drawEnemyField(
    const CoordSet& clickableCells,
    passCoord onClick,
    bool hideShips
) const {
    return FieldComponent(
        player.getEnemyField().get(),
        "Enemy field",
        hideShips,
        player.getLastShot(),
        clickableCells,
        onClick
    );
}
Component PlayerUI::combineFields(const Component& ownFieldEl, const  Component& enemyFieldEl) const
{
    using namespace ftxui;
    auto msg = message;
    auto tit = title;
    Component result = Renderer(Container::Horizontal(
        { ownFieldEl, enemyFieldEl }),
        [&ownFieldEl, &enemyFieldEl, msg, tit]() {
            return vbox({
                vbox(
                    text(tit) | bold,
                    text(msg)
                ) | center | border,
                hbox(
                    ownFieldEl->Render() | flex_shrink,
                    separatorEmpty(),
                    enemyFieldEl->Render() | flex_shrink
                ) | hcenter
                }) | hcenter | flex_shrink;
        });
    return result;
}


Coord PlayerUI::askCoord(bool ownField, const CoordSet& clickableCells) const
{
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    Coord coord;
    auto setCoord = [&coord, &screen](Coord c) {
        coord = c;
        screen.Exit();
        };

    Component fieldEl;
    Component enemyFieldEl;
    if (ownField) {
        fieldEl = drawMyField(clickableCells, setCoord);
        enemyFieldEl = drawEnemyField();
    }
    else {
        fieldEl = drawMyField();
        enemyFieldEl = drawEnemyField(clickableCells, setCoord);
    }

    auto result = combineFields(fieldEl, enemyFieldEl);
    screen.Loop(result);

    return coord;
}


Coord PlayerUI::askShotCoord() const
{
    auto coord = askCoord(false, player.getEnemyField()->getAvailableShots());
    return coord;
}

Coord PlayerUI::askPlaceShipCoord() const
{
    auto coord = askCoord(true, player.getField()->getAvailableShots());
    return coord;
}

void PlayerUI::switchToNextPlayer() const
{
    auto screen = ftxui::ScreenInteractive::Fullscreen();
    auto fields = combineFields(
        drawMyField({}, nullptr, true), // hide ships on own field
        drawEnemyField()
    );
    auto btn = ftxui::Button("Next player", [&screen]() { screen.Exit(); });
    auto result = ftxui::Renderer(
        ftxui::Container::Vertical({
            fields,
            btn
            }),
        [&fields, &btn]() {
            return ftxui::vbox({
                fields->Render(),
                btn->Render() | ftxui::hcenter
                });
        });
    screen.Loop(result);
}

void PlayerUI::showResult() const
{
    auto screen = ftxui::ScreenInteractive::Fullscreen();
    auto fields = combineFields(
        drawMyField(),
        drawEnemyField()
    );
    auto btn = ftxui::Button("Exit", [&screen]() { screen.Exit(); });
    auto result = ftxui::Renderer(
        ftxui::Container::Vertical({
            fields,
            btn
            }),
        [&fields, &btn]() {
            return ftxui::vbox({
                fields->Render(),
                btn->Render() | ftxui::hcenter
                });
        });
    screen.Loop(result);
}
