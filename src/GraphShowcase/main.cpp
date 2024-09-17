#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "DrawableGraph.h"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window{{800, 800}, "ShowCase", sf::Style::Default, settings};
    sf::Event event;

    DrawableGraph dg;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
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
