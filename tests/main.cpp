#include <bla/Vector.hpp>
#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "DrawableGraph.h"

int main()
{
    bla::Matrix<3,2> m({1, -2, M_PI, -4, 5, -6});
    bla::Vec2 v{{1., -1.}};
    bla::Vec3 result {m*v};
    fmt::print("{: .3f} × \n{: .3f} = \n{: .3f}\n", m, v, result);
    fflush(stdout);

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
            else if (event.type == sf::Event::MouseButtonReleased)
                dg.manageMouseEvent(event.mouseButton);
        }

        window.clear(sf::Color::White);
        window.draw(dg);
        window.display();
    }

    return 0;
}
