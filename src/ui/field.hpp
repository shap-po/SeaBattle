#ifndef UI_FIELD_HPP_
#define UI_FIELD_HPP_

#include "ui.hpp"
#include "game/field.hpp"

// function that passes Coord from component to outside world
using passCoord = const std::function<void(Coord)>&;

Component FieldComponent(
    const Field* field,
    const std::string& title,
    bool hideShips = false,
    Coord recentShot = Coord(-1, -1),
    const CoordSet& clickableCells = {},
    passCoord onClick = nullptr
);


#endif // UI_FIELD_HPP_
