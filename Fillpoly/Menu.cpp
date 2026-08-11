#include "Menu.hpp"

#include <algorithm>
#include <cstdint>
#include <string>

Menu::Menu(const sf::Font& font, float windowWidth, float menuHeight)
    : font(font),
    width(windowWidth),
    height(menuHeight),

    drawButton{ sf::RectangleShape(), sf::Text(font) },
    selectButton{ sf::RectangleShape(), sf::Text(font) },
    clearButton{ sf::RectangleShape(), sf::Text(font) },
    edgesButton{ sf::RectangleShape(), sf::Text(font) },
    applyButton{sf::RectangleShape(), sf::Text(font) },

    rgbLabels{
          sf::Text(font),
          sf::Text(font),
          sf::Text(font)
    },

    rgbTexts{
        sf::Text(font),
        sf::Text(font),
        sf::Text(font)
    },

    selectedColor(0, 255, 255),

    rgbInput{ "0", "255", "255" }
{
    initializeButtons();
    initializeColors();

    const std::array<std::string, 3> labels = { "R", "G", "B" };

    constexpr float startX = 750.f;
    constexpr float fieldY = 12.f;
    constexpr float fieldWidth = 60.f;
    constexpr float fieldHeight = 36.f;
    constexpr float spacing = 115.f;

    for (int i = 0; i < 3; ++i)
    {
        rgbFields[i].setSize({ fieldWidth, fieldHeight });
        rgbFields[i].setPosition({ startX + i * spacing, fieldY });

        rgbFields[i].setFillColor(sf::Color(70, 70, 70));
        rgbFields[i].setOutlineColor(sf::Color::White);
        rgbFields[i].setOutlineThickness(1.f);

        rgbLabels[i].setString(labels[i]);
        rgbLabels[i].setCharacterSize(16);
        rgbLabels[i].setFillColor(sf::Color::White);

        rgbLabels[i].setPosition({ startX + i * spacing - 25.f, fieldY + 8.f });

        rgbTexts[i].setString(rgbInput[i]);
        rgbTexts[i].setCharacterSize(16);
        rgbTexts[i].setFillColor( sf::Color::White);
        rgbTexts[i].setPosition({ startX + i * spacing + 8.f, fieldY + 8.f });
    }

    currentColorBox.setSize({ 30.f, 30.f });
    currentColorBox.setPosition({ 680.f, 15.f });
    currentColorBox.setOutlineColor(sf::Color::White);
    currentColorBox.setOutlineThickness(1.f);

    updateCurrentColorBox();
}

void Menu::initializeButtons()
{
    constexpr float buttonHeight = 40.f;
    constexpr float top = 10.f;

    auto configureButton = [&](Button& button, const std::string& text,
            float x, float buttonWidth)
        {
            button.shape.setSize({ buttonWidth, buttonHeight });
            button.shape.setPosition({ x, top });
            button.shape.setFillColor(sf::Color(60, 60, 60));
            button.shape.setOutlineColor(sf::Color::White);
            button.shape.setOutlineThickness(1.f);

            button.text.setString(text);
            button.text.setCharacterSize(15);
            button.text.setFillColor(sf::Color::White);

            button.text.setPosition({ x + 10.f, top + 9.f });
        };

    configureButton(drawButton, "DESENHAR", 10.f, 100.f);
    configureButton(selectButton, "SELECIONAR", 120.f, 115.f);
    configureButton(clearButton, "LIMPAR", 245.f, 90.f);
    configureButton(applyButton, "APLICAR", 1070.f, 85.f);
    configureButton(edgesButton, "ARESTAS: ON", 1360.f, 125.f);
}

void Menu::initializeColors()
{
    colors =
    {
        Color(255, 0, 0),       // RED
        Color(0, 255, 0),       // GREEN
        Color(0, 0, 255),       // BLUE
        Color(255, 255, 0),     // YELLOW
        Color(255, 0, 255)      // MAGENTA
    };

    const float startX = 500.f;
    const float y = 17.f;
    const float size = 25.f;
    const float spacing = 32.f;

    for (size_t i = 0; i < colorButtons.size(); ++i)
    {
        colorButtons[i].setSize({ size, size });
        colorButtons[i].setPosition({
            startX + static_cast<float>(i) * spacing,
            y
            });

        colorButtons[i].setFillColor(toSFML(colors[i]));

        colorButtons[i].setOutlineColor(sf::Color::White);
        colorButtons[i].setOutlineThickness(1.f);
    }
}

void Menu::updateCurrentColorBox()
{
    currentColorBox.setFillColor(
        sf::Color(
            selectedColor.r,
            selectedColor.g,
            selectedColor.b
        )
    );
}

sf::Color Menu::toSFML(Color color) const
{
    return sf::Color(
        color.r,
        color.g,
        color.b,
        color.a
    );
}

void Menu::handleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto& mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            handleMouseClick(mouse->position);
        }

        return;
    }
    if (event.is<sf::Event::TextEntered>())
    {
        if (activeRGBField >= 0)
        {
            const auto* text = event.getIf<sf::Event::TextEntered>();
            handleTextEntered(text->unicode);
        }

        return;
    }

    if (event.is<sf::Event::KeyPressed>())
    {
        if (activeRGBField >= 0)
        {
            const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
            handleKeyPressed(keyPressed->code);
        }

        return;
    }
}

void Menu::handleMouseClick(sf::Vector2i mousePosition)
{
    if (isInside(drawButton.shape, mousePosition))
    {
        mode = Mode::Draw;
        pendingAction = Action::Draw;
        return;
    }

    if (isInside(selectButton.shape, mousePosition))
    {
        mode = Mode::Select;
        pendingAction = Action::Select;
        return;
    }

    if (isInside(clearButton.shape, mousePosition))
    {
        pendingAction = Action::Clear;
        return;
    }

    for (int i = 0; i < colorButtons.size(); ++i)
    {
        if (isInside(colorButtons[i], mousePosition))
        {
            selectedColor = colors[i];
            updateCurrentColorBox();
            return;
        }
    }

    for (int i = 0; i < rgbFields.size(); ++i)
    {
        if (isInside(rgbFields[i], mousePosition))
        {
            activeRGBField = i;
            return;
        }
    }

    if (isInside(applyButton.shape, mousePosition))
    {
        applyRGBColor();
    }

    if (isInside(edgesButton.shape, mousePosition))
    {
        edgesEnabled = !edgesEnabled;

        edgesButton.text.setString(
            edgesEnabled ? "ARESTAS: ON" : "ARESTAS: OFF"
        );
        pendingAction = Action::ChangeEdges;

        return;
    }
}

void Menu::handleTextEntered(char32_t character)
{
    if (activeRGBField < 0)
        return;

    if (character >= U'0' && character <= U'9')
    {
        std::string& value = rgbInput[activeRGBField];

        if (value.size() < 3)
        {
            value += static_cast<char>(character);
        }
    }
    updateRGBInputTexts();
}

void Menu::handleKeyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Key::Backspace)
    {
        if (activeRGBField >= 0 && !rgbInput[activeRGBField].empty())
        {
            rgbInput[activeRGBField].pop_back();
        }
        updateRGBInputTexts();
        return;
    }

    if (key == sf::Keyboard::Key::Enter)
    {
        if(activeRGBField >= 0)
            applyRGBColor();

        return;
    }

    if (key == sf::Keyboard::Key::Escape)
    {
        activeRGBField = -1;
        return;
    }
}

void Menu::updateRGBInputTexts()
{
    for (size_t i = 0; i < rgbTexts.size(); ++i)
    {
        rgbTexts[i].setString(rgbInput[i]);
    }
}

void Menu::applyRGBColor()
{
    int values[3] = { 0, 0, 0 };

    for (int i = 0; i < 3; ++i)
    {
        if (rgbInput[i].empty())
        {
            values[i] = 0;
        }
        else
        {
            values[i] = std::stoi(rgbInput[i]);
        }

        values[i] = std::clamp(values[i], 0, 255);
    }

    selectedColor = Color(
        static_cast<std::uint8_t>(values[0]),
        static_cast<std::uint8_t>(values[1]),
        static_cast<std::uint8_t>(values[2])
    );

    activeRGBField = -1;

    updateCurrentColorBox();
}

void Menu::draw(sf::RenderWindow& window)
{
    sf::RectangleShape background;

    background.setSize({ width, height });
    background.setPosition({ 0.f, 0.f });
    background.setFillColor(sf::Color(35, 35, 35));

    window.draw(background);

    window.draw(drawButton.shape);
    window.draw(drawButton.text);

    window.draw(selectButton.shape);
    window.draw(selectButton.text);

    window.draw(clearButton.shape);
    window.draw(clearButton.text);

    for (const auto& colorButton : colorButtons)
    {
        window.draw(colorButton);
    }

    window.draw(currentColorBox);

    for (int i = 0; i < 3; ++i)
    {
        window.draw(rgbLabels[i]);
        window.draw(rgbFields[i]);
        window.draw(rgbTexts[i]);
    }

    window.draw(applyButton.shape);
    window.draw(applyButton.text);

    window.draw(edgesButton.shape);
    window.draw(edgesButton.text);

    // Highlight current mode
    if (mode == Mode::Draw)
    {
        drawButton.shape.setOutlineColor(sf::Color::Blue);
    }
    else
    {
        selectButton.shape.setOutlineColor(sf::Color::Blue);
    }

}

bool Menu::isInside(
    const sf::RectangleShape& shape, sf::Vector2i position) const
{
    return shape.getGlobalBounds().contains(
        sf::Vector2f(
            static_cast<float>(position.x),
            static_cast<float>(position.y)
        )
    );
}

bool Menu::isMouseOverMenu(
    sf::Vector2i mousePosition) const
{
    return mousePosition.y < height;
}

Menu::Action Menu::consumeAction()
{
    Action action = pendingAction;
    pendingAction = Action::None;

    return action;
}

Color Menu::getSelectedColor() const
{
    return selectedColor;
}

bool Menu::areEdgesEnabled() const
{
    return edgesEnabled;
}

Menu::Mode Menu::getMode()
{
    return mode;
}
