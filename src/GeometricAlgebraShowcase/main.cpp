#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "GeometricAlgebra/Vector.h"
#include "GeometricAlgebra/Multivector.h"
#include "Interact/Shape.h"
#include "Interact/Drag.h"


int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window{{1000, 1000}, "ShowCase", sf::Style::Default, settings};
    sf::Event event;

    const int radius = 10;
    act::Shape circles[10];

    for (int i = 0; i<10; ++i)
    {
        auto c = circles[i].makeShape<sf::CircleShape>(radius);
        c->setOrigin(radius, radius);
        c->setFillColor(sf::Color::White);
        c->setOutlineColor(sf::Color::Black);
        c->setOutlineThickness(2);
        c->setPosition(rand() % 600 + 200, rand() % 600 + 200);
        circles[i].makeInteraction<act::Drag>([](sf::Shape& shape, bool active)
                                              { shape.setOutlineThickness(2 + 2 * (int) active); });
        // circles[i].makeInteraction<act::Hover>([](sf::Shape& shape, act::Hover::State state) { shape.setOutlineThickness(2 + 2 * (int)state); });
        circles[i].makeInteraction<act::Hover>([](sf::Shape& shape, bool active)
                                               { shape.setOutlineColor(active ? sf::Color::Blue : sf::Color::Black);  });
    }

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            for (act::Shape& c : circles | std::views::reverse)
                if (c.processEvent(event)) break;
        }

        window.clear(sf::Color::White);
        for (auto& c : circles)
            window.draw(c);
        window.display();
    }

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


