#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "DrawableGraph.h"

int main()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window{sf::VideoMode{{800, 800}}, "Graph", sf::Style::Default, sf::State::Windowed, settings};

    DrawableGraph dg;

    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
            else
                dg.processEvent(event);
        }

        window.clear(sf::Color::White);
        window.draw(dg);
        window.display();
    }

    return 0;
}
