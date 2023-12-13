#include "field.hpp"

#include <string>
#include <map>

#include "memory.hpp"

using namespace std;
using namespace ftxui;

// function that creates animation for button
ButtonOption ButtonAnimation(Color background, Color foreground) {
    ButtonOption option;
    option.transform = [](const EntryState& s) { return text(s.label); };
    option.animated_colors.foreground.Set(foreground, background);
    option.animated_colors.background.Set(background, foreground);
    return option;
}

// styles
const auto resentShotStyle = color(Color::DarkSlateGray3);
const auto inactiveCellStyle = color(Color::GrayLight); // cell is not clickable
const auto activeCellStyle = ButtonAnimation(Color::Grey23, Color::Grey35); // cell is clickable (button)

const auto coordStyle = color(Color::Grey78);

const map<Cell, string> cellToSymbol = {
    {Cell::EMPTY, "░░"},
    {Cell::ALIVE, "██"},
    {Cell::MISS,  "  "},
    {Cell::DEAD,  "><"},
};
const string recentShotSymbol = "[]";


// adapter for ftxui::Element to be used as Component
class RenderableElementBase : public ComponentBase {
private:
    Element element;
public:
    explicit RenderableElementBase(Element element) : element(element) {}
    Element Render() override {
        return element;
    }
};
// factory function for RenderableElement
Component RenderableElement(Element element) {
    return MakeShared<RenderableElementBase>(element);
}

class GridComponentBase : public ComponentBase {
private:
    vector<vector<Component>> data;
public:
    explicit GridComponentBase() = default;

    void AddElement(Component element, Coord coord) {
        if (coord.first >= data.size()) {
            data.resize(coord.first + 1);
        }
        if (coord.second >= data[coord.first].size()) {
            data[coord.first].resize(coord.second + 1);
        }
        data[coord.first][coord.second] = move(element);
        Add(data[coord.first][coord.second]); // add element to children
    }

    // render grid as vbox of hboxes
    Element Render() override {
        vector<Element> elements;
        for (const auto& row : data) {
            vector<Element> rowElements;
            for (const auto& el : row) {
                rowElements.push_back(el->Render());
            }
            elements.push_back(hbox(move(rowElements)));
        }
        return vbox(elements);
    }

    bool OnEvent(Event event) override { // "borrowed" from ftxui ContainerBase::OnEvent
        if (event.is_mouse()) {
            return ComponentBase::OnEvent(std::move(event));
        }

        if (!Focused()) {
            return false;
        }

        if (ActiveChild() && ActiveChild()->OnEvent(event)) {
            return true;
        }

        return false;
    }

    Component ActiveChild() override { return nullptr; } // disable focus by keyboard
};

// alias for GridComponent shared pointer
using GridComponent = shared_ptr<GridComponentBase>;

// factory function for GridComponent
GridComponent Grid() {
    return MakeShared<GridComponentBase>();
}

// factory function for Field component
Component FieldComponent(
    const Field* field,
    const string& title,
    bool hideShips,
    Coord recentShot,
    const CoordSet& clickableCells,
    passCoord onClick
) {
    // create grid of cells
    auto grid = Grid();
    for (int i = 0; i < field->getSize().first; i++)
    {
        for (int j = 0; j < field->getSize().second; j++)
        {
            Coord coord = { i, j };
            auto cellType = field->getCell(coord);
            if (hideShips && cellType == Cell::ALIVE) cellType = Cell::EMPTY; // hide ships on enemy field
            auto cellSymbol = cellToSymbol.at(cellType);
            if (coord == recentShot) cellSymbol = recentShotSymbol;

            Component cell; // cell is either button or text, depending on whether it is clickable
            if (clickableCells.count(coord) > 0)
            {
                cell = Button(
                    cellSymbol,
                    [onClick, coord]() {
                        onClick(coord);
                    },
                    activeCellStyle
                );
            }
            else {
                cell = RenderableElement(text(cellSymbol)) | inactiveCellStyle;
            }
            if (coord == recentShot) cell |= resentShotStyle;

            grid->AddElement(cell, coord);
        }
    }

    // wrap grid in window with name
    return Renderer(
        grid,
        [grid, title]() {
            return window(text(title) | hcenter, grid->Render());
        }
    );
}
