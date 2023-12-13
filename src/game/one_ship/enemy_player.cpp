#include "enemy_player.hpp"
#include "random.hpp"
#include <algorithm>

#ifdef SHOW_ENEMY_DEBUG_STRATEGY
void OneShipEnemyPlayer::showStrategy(const CoordSet& shots) const {

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    auto fields = ui.combineFields(
        ui.drawMyField(),
        ui.drawEnemyField(shots, nullptr, false)
    );
    auto btn = ftxui::Button("Continue", [&screen]() { screen.Exit(); });
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
#endif

Coord OneShipEnemyPlayer::placeShip() const
{
    auto [w, h] = field->getSize();
    switch (difficulty)
    {
    case Difficulty::Easy: // random location
        return Coord(randInt(0, w - 1), randInt(0, h - 1));
    case Difficulty::Medium: // place ship further from the edges
        return Coord(randInt(1, w - 2), randInt(1, h - 2));
    case Difficulty::Hard: // place ship close to the center
        return Coord(randInt(w / 3, w * 2 / 3), randInt(h / 3, h * 2 / 3));
    }
}

Coord OneShipEnemyPlayer::chooseShot(const CoordSet& shots) const {
#ifdef SHOW_ENEMY_DEBUG_STRATEGY
    showStrategy(shots);
#endif
    return randElement(shots);
}

Coord OneShipEnemyPlayer::makeShot()
{
    if (difficulty == Difficulty::Easy) // make random shot
        return chooseShot(enemyField->getAvailableShots());

    tryFindShip(); // try to find location of the ship on previous shot
    expandPossibleShipLocations();

    if (difficulty == Difficulty::Medium) // make random shot from possible locations
        return chooseShot(possibleShipLocations);

    // shot in the best possible location
    auto bestShots = findBestShots();
    return chooseShot(bestShots);
}

CoordSet firstBestShots(int fieldWidth, int fieldHeight) {
    // for the first shot, get cells near smallest edge
    // 4|5|6|7|8|8|7|6|5|4
    // 4|6|7|8|8|8|8|7|6|4
    // 4|6|7|8|8|8|8|7|6|4
    // 4|5|6|7|8|8|7|6|5|4
    CoordSet bestShots;
    if (fieldWidth < fieldHeight) {
        for (int i = 0; i < fieldWidth; i++) {
            bestShots.insert(Coord(i, 0));
            bestShots.insert(Coord(i, fieldHeight - 1));
        }
    }
    else if (fieldWidth > fieldHeight) {
        for (int i = 0; i < fieldHeight; i++) {
            bestShots.insert(Coord(0, i));
            bestShots.insert(Coord(fieldWidth - 1, i));
        }
    }
    else {
        for (int i = 0; i < fieldWidth; i++) {
            bestShots.insert(Coord(i, 0));
            bestShots.insert(Coord(i, fieldHeight - 1));
            bestShots.insert(Coord(0, i));
            bestShots.insert(Coord(fieldWidth - 1, i));
        }
    }

    return bestShots;
}

CoordSet OneShipEnemyPlayer::findBestShots() const {
    auto [w, h] = field->getSize();

    if (lastShot == Coord(-1, -1)) return firstBestShots(w, h); // if no last shot, return first best shots

    int minPositions = w + h;
    std::vector<std::vector<int>> shotVariants(w, std::vector<int>(h, 0)); // count possible positions for each shot

    int maxPositions; // trying to minimize this

    int positions = 0;
    for (auto shot : possibleShipLocations) { // try all possible shots
        maxPositions = 0;
        for (int distance = 1; distance < w + h; distance++) { // try all possible distances
            // count possible positions for this distance
            positions = std::count_if(possibleShipLocations.begin(), possibleShipLocations.end(), [distance, shot](Coord pos) {return Field::manhattanDistance(pos, shot) == distance; });
            if (positions > maxPositions) maxPositions = positions; // update max positions
        }
        shotVariants[shot.first][shot.second] = maxPositions; // save max positions for this shot
        if (maxPositions < minPositions) minPositions = maxPositions; // if this shot is better than previous, update min positions
    }

    CoordSet bestShots;
    for (int i = 0; i < shotVariants.size(); i++) { // find all shots with min positions
        for (int j = 0; j < shotVariants[i].size(); j++) {
            if (shotVariants[i][j] == minPositions) bestShots.insert(Coord(i, j));
        }
    }

    return bestShots;
}


Coord OneShipEnemyPlayer::makeMove() const
{
    if (difficulty == Difficulty::Easy) // make random move
    {
        auto moves = field->getAvailableMoves(shipCoord, movesMap);
        return randElement(moves);
    }
    // don't stay in one place (if possible)
    CoordSet cutMovesMap;
    for (auto move : movesMap)
    {
        if (move != Coord(0, 0)) cutMovesMap.insert(move);
    }
    auto moves = field->getAvailableMoves(shipCoord, cutMovesMap);
    if (moves.empty()) moves = field->getAvailableMoves(shipCoord, movesMap);

    if (difficulty == Difficulty::Medium) return randElement(moves);

    auto [w, h] = field->getSize();
    // avoid edges
    CoordSet withoutEdges;
    for (auto move : moves)
    {
        if (move.first == 0 || move.first == w - 1) continue;
        if (move.second == 0 || move.second == h - 1) continue;
        withoutEdges.insert(move);
    }
    if (withoutEdges.empty()) withoutEdges = moves;
    // avoid already shot cells
    CoordSet withoutShots;
    for (auto move : withoutEdges)
    {
        if (field->isMiss(move)) continue;
        withoutShots.insert(move);
    }
    if (withoutShots.empty()) withoutShots = withoutEdges;
    return randElement(withoutShots);
}

void OneShipEnemyPlayer::expandPossibleShipLocations() {
    if (possibleShipLocations.empty()) { // init possible ship locations
        possibleShipLocations = enemyField->getAvailableShots();
        return;
    }
    if (lastShot == Coord(-1, -1)) return; // skip if no last shot
    CoordSet newPossibleShipLocations;
    for (auto [x, y] : possibleShipLocations)
    {
        for (auto [dx, dy] : movesMap)
        {
            Coord newCoord = { x + dx, y + dy };
            if (enemyField->canShot(newCoord))
            {
                newPossibleShipLocations.insert(newCoord);
            }
        }
    }
    possibleShipLocations = newPossibleShipLocations;

}

void OneShipEnemyPlayer::tryFindShip() {
    if (lastShot == Coord(-1, -1)) return; // skip if no last shot
    // remove last shot from possible ship locations
    possibleShipLocations.erase(lastShot);
    CoordSet newPossibleShipLocations;
    for (auto coord : possibleShipLocations)
    {
        if (Field::manhattanDistance(coord, lastShot) == distanceFromLastShot)
        {
            newPossibleShipLocations.insert(coord);
        }
    }
    possibleShipLocations = newPossibleShipLocations;
}

