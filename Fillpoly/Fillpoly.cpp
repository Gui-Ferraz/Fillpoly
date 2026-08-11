#include "Color.hpp"
#include "Framebuffer.hpp"
#include "Menu.hpp"
#include "Polygon.hpp"
#include "Rasterizer.hpp"
#include "Renderer.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>
#include<filesystem>

#define NO_POLYGON_SELECTED -1

const Color WINDOW_BACKGROUND_COLOR = {128, 128, 128};
const Color LINE_COLOR = Colors::Black;
const Color FILL_COLOR = Colors::Red;
const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 900;

enum class DrawingState
{
    Idle,
    DrawingOuter,
    DrawingHole
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Fillpoly - Computação Gráfica");

    sf::Font font;
    if (!font.openFromFile("Assets/DejaVuSans.ttf"))
    {
        std::cerr << "Falha ao carregar fonte.\n";
        return 1;
    }
    Menu menu(font, WINDOW_WIDTH);

    Framebuffer framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BACKGROUND_COLOR);
    Renderer renderer(framebuffer.getWidth(), framebuffer.getHeight());

    std::vector<Polygon> polygons;
    long int selectedPolygon = NO_POLYGON_SELECTED;
    Polygon currentPolygon;
    Contour currentContour;
    DrawingState state = DrawingState::Idle;

    const Color ContourColor = Colors::White;
    Color edgeColor = ContourColor;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            menu.handleEvent(*event);

            switch (menu.consumeAction())
            {
            case Menu::Action::Draw:
                if (state == DrawingState::Idle)
                    std::cout << "Mode: Drawing" << std::endl;
                break;
            
            case Menu::Action::Select:
                std::cout << "Mode: Selecting" << std::endl;
                break;
           
            case Menu::Action::ChangeEdges:
                if (menu.areEdgesEnabled())
                    edgeColor = ContourColor;
                else
                    edgeColor = WINDOW_BACKGROUND_COLOR;
                for (Polygon& p : polygons)
                    Rasterizer::drawPolygon(framebuffer, p, edgeColor);

                std::cout << "Edges changed" << std::endl;
                break;
            
            case Menu::Action::Clear:
                if (state == DrawingState::Idle)
                {
                    selectedPolygon = NO_POLYGON_SELECTED;
                    currentPolygon.clear();
                    currentContour.clear();
                    polygons.clear();
                    framebuffer.clear();
                    state = DrawingState::Idle;
                }
                std::cout << "Screen Cleared" << std::endl;
                break;
            
            case Menu::Action::None:
                break;

            }

            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (menu.isMouseOverMenu(mouse->position))
                    continue;

                if (mouse->button == sf::Mouse::Button::Left)
                {
                    if (menu.getMode() == Menu::Mode::Draw)
                    {
                        if (state != DrawingState::DrawingHole)
                            state = DrawingState::DrawingOuter;

                        currentContour.vertices.push_back({ mouse->position.x, mouse->position.y });

                        if (currentContour.vertices.size() < 2)
                            continue;

                        Point p1 = currentContour.vertices[currentContour.vertices.size() - 2];
                        Point p2 = currentContour.vertices[currentContour.vertices.size() - 1];
                        Rasterizer::drawLine(framebuffer, p1, p2, LINE_COLOR);
                    }
                    else if (menu.getMode() == Menu::Mode::Select)
                    {
                        selectedPolygon = NO_POLYGON_SELECTED;
                        for (long int i = 0; i < polygons.size(); ++i)
                        {
                            Point mousePosition = { mouse->position.x, mouse->position.y };
                            if (Rasterizer::pointInPolygon(mousePosition, polygons[i]))
                            {
                                selectedPolygon = i;
                                std::cout << "Polygon selected" << std::endl;
                                break;
                            }
                        }
                    }
                }
                else if (mouse->button == sf::Mouse::Button::Right)
                {
                    if (menu.getMode() == Menu::Mode::Draw)
                    {
                        if (currentContour.vertices.size() < 3)     // 3 is the mininum vertices for a contour
                            continue;

                        if (state == DrawingState::DrawingOuter)
                        {
                            currentPolygon.outer = currentContour;

                            state = DrawingState::DrawingHole;
                        }
                        else if (state == DrawingState::DrawingHole)
                        {
                            currentPolygon.holes.push_back(currentContour);
                        }

                        Point p1 = currentContour.vertices[currentContour.vertices.size() - 1];
                        Point p2 = currentContour.vertices[0];
                        Rasterizer::drawLine(framebuffer, p1, p2, LINE_COLOR);

                        currentContour.clear();
                    }
                }
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    if (menu.getMode() == Menu::Mode::Draw)
                    {
                        if (state == DrawingState::DrawingHole)
                        {
                            if (!currentContour.empty())
                                continue;

                            Rasterizer::drawPolygon(framebuffer, currentPolygon, edgeColor);

                            polygons.push_back(currentPolygon);

                            currentPolygon.clear();
                            currentContour.clear();

                            state = DrawingState::Idle;

                            std::cout << "Polygon drawed." << std::endl;
                        }
                    }
                    else if (menu.getMode() == Menu::Mode::Select)
                    {
                        if (selectedPolygon != NO_POLYGON_SELECTED)
                        {
                            Rasterizer::fillpoly(framebuffer, polygons[selectedPolygon], menu.getSelectedColor());
                            Rasterizer::drawPolygon(framebuffer, polygons[selectedPolygon], edgeColor);
                        }
                    }
                }
                else if (keyPressed->code == sf::Keyboard::Key::Delete)
                {
                    if (menu.getMode() == Menu::Mode::Select)
                    {
                        if (selectedPolygon != NO_POLYGON_SELECTED)
                        {
                            polygons.erase(polygons.begin() + selectedPolygon);

                            selectedPolygon = NO_POLYGON_SELECTED;

                            framebuffer.clear();
                            currentContour.clear();
                            currentPolygon.clear();
                            for (Polygon& p : polygons)
                            {
                                Rasterizer::drawPolygon(framebuffer, p, edgeColor);
                            }
                        }
                    }
                }
            }

        }

        window.clear();
        renderer.render(window, framebuffer);
        menu.draw(window);
        window.display();
    }

    return 0;
}
