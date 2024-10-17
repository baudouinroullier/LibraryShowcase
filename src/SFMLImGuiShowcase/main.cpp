#include "fmt/core.h"
#include "SFML/Graphics.hpp"

int main()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window{sf::VideoMode{{800, 800}}, "ShowCase", sf::Style::Default, sf::State::Windowed, settings};

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        window.clear(sf::Color::White);
        window.display();
    }

    return 0;
}
