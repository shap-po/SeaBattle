#include "menu.hpp"

#include <vector>
#include <string>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>

#include "memory.hpp"

using namespace ftxui;
using std::vector;
using std::string;

const string fieldSizeSeparators = "xX*хХ"; // allowed separators for custom field size (x,X, *, ua x,X)

// Custom toggle component
class EasyToggleBase : public ComponentBase {
public:
    const vector<string> _entries;
    int selected;
    Component toggle;

    EasyToggleBase(const vector<string>& entries, int defaultSelected) : _entries(entries), selected(defaultSelected) {
        toggle = Toggle(&_entries, &selected);
        Add(toggle);
    }

    ftxui::Element Render() override { return toggle->Render(); }
};


shared_ptr<EasyToggleBase> EasyToggle(const vector<string>& entries, int defaultSelected = 0) {
    return MakeShared<EasyToggleBase>(entries, defaultSelected);
}


// Custom input component
class EasyInputBase : public ComponentBase {
public:
    string value;
    Component input;

    EasyInputBase(
        const string& value,
        const string& placeholder = "",
        bool isCustomFieldSize = false
    ) : value(value) {
        auto options = InputOption();
        options.multiline = false;
        options.placeholder = placeholder;
        input = Input(&this->value, options);
        if (isCustomFieldSize) applyCustomFieldHandler();
        Add(input);
    }

    ftxui::Element Render() override { return input->Render(); }

private:
    void applyCustomFieldHandler() {
        input |= CatchEvent(
            [this](const Event& event) {
                if (event.is_character())
                {
                    if (value.size() >= 5) return true; // limit input length
                    char character = event.character()[0];
                    if (isdigit(character)) return false; // allow digits
                    // if there is already a separator in the string, ignore the event
                    if (value.find_first_of(fieldSizeSeparators) != string::npos) return true;
                    // if character is a separator
                    for (char separator : fieldSizeSeparators)
                    {
                        if (character == separator)
                        {
                            // force adding 'x' instead of any other separator
                            value += 'x';
                            // move cursor 1 to the right
                            input->OnEvent(Event::ArrowRight);
                            return true;
                        }
                    }
                    return true; // ignore other characters
                }
                return false; // allow other events
            }
        );
    }
};

shared_ptr<EasyInputBase> EasyInput(
    const string& value,
    const string& placeholder = "",
    bool isCustomFieldSize = false
) {
    return MakeShared<EasyInputBase>(value, placeholder, isCustomFieldSize);
}



GameOptions getGameOptions()
{
    auto scr = ScreenInteractive::Fullscreen();

    // define menu components
    auto gameMode = EasyToggle(gameModeEntries, 1);
    auto gameType = EasyToggle(gameTypeEntries, 1);
    auto fieldSize = EasyToggle(fieldSizeEntries);
    auto customFieldSize = EasyInput("", "Enter field size", true);
    auto movesMap = EasyToggle({ "4", "8" });
    auto difficulty = EasyToggle(difficultyEntries);
    auto multiplayerType = EasyToggle(multiplayerTypeEntries, 2);
    auto ipInput = EasyInput("", "Enter IP address");
    auto startBtn = Button("Start game", scr.ExitLoopClosure());

    auto empty = Renderer(Container::Horizontal({}), []() { return hbox(); }); // empty component for tabs

    // define tabs (see GameOptions struct for details)
    auto gameModeDependentTab = Container::Tab({
        empty, // classic
        Renderer(movesMap, [movesMap]() { return window(text("Moves map"), movesMap->Render()); }), // one ship
        }, &gameMode->selected);

    // auto gameTypeDependentTab = Container::Tab({
    //     Renderer(difficulty, [&]() { return window(text("Difficulty"), difficulty->Render()); }), // singleplayer
    //     Renderer(multiplayerType, [&]() { return window(text("Multiplayer"), multiplayerType->Render()); }), // multiplayer
    //     empty, // one screen
    //     }, &gameType->selected); // TODO: uncomment when multiplayer is implemented

    auto gameTypeDependentTab = Renderer(difficulty, [&]() { return window(text("Difficulty"), difficulty->Render()); }); // singleplayer

    auto renderer = Renderer(
        Container::Vertical({
            gameMode,
            gameType,
            Container::Horizontal({
                fieldSize,
                customFieldSize,
            }),
            gameModeDependentTab,
            gameTypeDependentTab,
            startBtn,
            }),
            [&]() {
            return
                window(text("SeaBattle") | hcenter | bold,
                    vbox({
                        // window(text("Game mode"), gameMode->Render()), //TODO: uncomment when classic mode is implemented
                        window(text("Game type"), gameType->Render()),

                        window(text("Field size"),
                            hbox({
                                fieldSize->Render(),
                                fieldSize->selected == fieldSizeEntries.size() - 1 ?
                                    hbox({separator(), customFieldSize->Render()}) : hbox()
                                })
                        ),

                        gameModeDependentTab->Render(),
                        gameTypeDependentTab->Render(),

                        startBtn->Render() | hcenter,
                        })

                );
        }
    );

    scr.Loop(renderer);

    GameOptions options;
    options.gameMode = (GameMode)gameMode->selected;
    options.gameType = (GameType)gameType->selected;
    Size size;
    if (fieldSize->selected == fieldSizeEntries.size() - 1) // custom field size
    {
        string customVal = customFieldSize->value;
        auto xPos = customVal.find('x');
        if (xPos == string::npos) { // no x, field is square
            size = { stoi(customVal), stoi(customVal) };
        }
        else if (xPos != 0 && xPos != customVal.size() - 1) // x is not the first or the last character
        {
            auto s1 = customVal.substr(0, xPos);
            auto s2 = customVal.substr(xPos + 1);
            size = { stoi(s1), stoi(s2) };
        }
        else size = { 10, 10 }; // default value if input is invalid

        if (size.first == 0 || size.second == 0) size = { 10, 10 }; // default value if input is
        // limit size
        if (size.first < minFieldSize) size.first = minFieldSize;
        if (size.second < minFieldSize) size.second = minFieldSize;
        if (size.first > maxFieldSize) size.first = maxFieldSize;
        if (size.second > maxFieldSize) size.second = maxFieldSize;
    }
    else
    {
        size = fieldSizes[fieldSize->selected];
    }
    options.fieldSize = size;

    options.movesMap = movesMap->selected == 0 ? movesMap4 : movesMap8;

    options.difficulty = (Difficulty)difficulty->selected;
    options.multiplayerType = (MultiplayerType)multiplayerType->selected;

    return options;
}
