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
    galg::Vec2 b{-3,4};

    fmt::print("a = {}, b = {}\n", a, b);
    fmt::print("a + b = {}\n", a + b);
    fmt::print("a - b = {}\n", a - b);
    fmt::print("3*a/2 = {} \n", 3*a/2);
    fmt::print("a.b = {} \n", galg::dot(a, b));
    fmt::print("ab = {} = {:.3f}e^(i{:.3f})\n", a * b, std::abs(a*b), std::arg(a * b));

    fmt::print("norm1(b) = {} \n", b.norm1());
    fmt::print("norm2(b) = {} \n", b.norm2());
    fmt::print("normP(b, 4) = {} \n", b.normP(4));
    fmt::print("normInf(b) = {} \n", b.normInf());
    return 0;
}
