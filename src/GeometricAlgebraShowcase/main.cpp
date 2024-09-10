#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "GeometricAlgebra/Vector.h"
#include "GeometricAlgebra/Multivector.h"

int main()
{
    // sf::ContextSettings settings;
    // settings.antialiasingLevel = 8;

    // sf::RenderWindow window{{800, 800}, "ShowCase", sf::Style::Default, settings};
    // sf::Event event;

    // while (window.isOpen())
    // {
    //     while (window.pollEvent(event))
    //     {
    //         if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    //             window.close();
    //     }

    //     window.clear(sf::Color::White);
    //     window.display();
    // }

    galg::Vec2 a{1,2};
    galg::Vec2 b{3,4};

    fmt::print("a = {} {}\n", a.x(), a.y());
    fmt::print("b = {} {}\n", b.x(), b.y());
    fmt::print("a.b = {} \n", galg::dot(a, b));
    fmt::print("ab = {} \n", a*b);
    return 0;
}
