#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <string>

#include "Color.hpp"

class Menu
{
public:
    enum class Mode
    {
        Draw,
        Select
    };

    enum class Action
    {
        None,
        Draw,
        Select,
        ChangeEdges,
        Clear
    };

public:
    Menu(const sf::Font& font, float windowWidth, float menuHeight = 60.f);

    void handleEvent(const sf::Event& event);

    void draw(sf::RenderWindow& window);

    Action consumeAction();

    Color getSelectedColor() const;
    bool areEdgesEnabled() const;
    Mode getMode();

    bool isMouseOverMenu(sf::Vector2i mousePosition) const;

private:
    struct Button
    {
        sf::RectangleShape shape;
        sf::Text text;
    };

private:
    void initializeButtons();
    void initializeColors();

    void handleMouseClick(sf::Vector2i mousePosition);
    void handleTextEntered(char32_t character);
    void handleKeyPressed(sf::Keyboard::Key key);
    void updateCurrentColorBox();
    void updateRGBInputTexts();

    void applyRGBColor();

    sf::Color toSFML(Color color) const;

    bool isInside(const sf::RectangleShape& shape,
        sf::Vector2i position) const;

private:
    float width;
    float height;

    sf::Font font;

    Button drawButton;
    Button selectButton;
    Button clearButton;
    Button edgesButton;
    Button applyButton;

    std::array<sf::RectangleShape, 5> colorButtons;
    std::array<Color, 5> colors;

    sf::RectangleShape currentColorBox;

    std::array<sf::RectangleShape, 3> rgbFields;
    std::array<sf::Text, 3> rgbLabels;
    std::array<sf::Text, 3> rgbTexts;

    Color selectedColor;

    Action pendingAction = Action::None;
    Mode mode = Mode::Draw;

    bool edgesEnabled = true;

    std::array<std::string, 3> rgbInput;
    int activeRGBField = -1;


};