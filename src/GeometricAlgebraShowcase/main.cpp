#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "GeometricAlgebra/Vector.h"
#include "GeometricAlgebra/Multivector.h"


class MovableCircle : public sf::CircleShape
{
public:
    MovableCircle(): sf::CircleShape{radius} {
        setOrigin(radius, radius);
        setFillColor(sf::Color::White);
        setOutlineColor(sf::Color::Black);
        setOutlineThickness(2);
        setPosition(rand()%600+200, rand()%600+200);
    }

    void processEvent(sf::Event e)
    {
        if (state == Fixed && e.type == sf::Event::EventType::MouseMoved && std::abs(e.mouseMove.x - getPosition().x) < radius && std::abs(e.mouseMove.y - getPosition().y) < radius)
            state = Hover;
        else if (state == Hover)
        {
            if (e.type == sf::Event::EventType::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Left)
            {
                state = Held;
                clickPos = getPosition() - sf::Vector2f{e.mouseButton.x, e.mouseButton.y};
            }
            else if (e.type == sf::Event::EventType::MouseMoved && !(std::abs(e.mouseMove.x - getPosition().x) < radius && std::abs(e.mouseMove.y - getPosition().y) < radius))
                state = Fixed;
        }
        else if (state == Held)
        {
            if (e.type == sf::Event::EventType::MouseButtonReleased)
                state = Hover;
            else if (e.type == sf::Event::MouseMoved)
                setPosition(clickPos + sf::Vector2f{e.mouseMove.x , e.mouseMove.y});
        }

        // setFillColor(state == Fixed ? sf::Color::White : sf::Color{128,128,128});
        setOutlineThickness(2+2*state);
    }

    enum MoveState
    {
        Fixed, Hover, Held
    } state = Fixed;

    sf::Vector2f clickPos;
    static constexpr double radius = 10;
};
int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window{{1000, 1000}, "ShowCase", sf::Style::Default, settings};
    sf::Event event;

    MovableCircle circles[10];

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            for (auto& c : circles)
                c.processEvent(event);
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


