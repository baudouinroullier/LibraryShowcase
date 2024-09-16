#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "GeometricAlgebra/Vector.h"
#include "GeometricAlgebra/Multivector.h"
#include "Interact/Shape.h"
#include "Interact/Drag.h"
#include "Interact/Hover.h"
#include "Interact/ArrowShape.h"



class MovableArrow : public sf::Drawable
{
public:
    MovableArrow() :
        m_arrow(10)
    {
        const double w = 10;
        sf::CircleShape* s = m_startPoint.makeShape<sf::CircleShape>(w);
        s->setOrigin(w, w);
        s->setFillColor(sf::Color::Transparent);
        s->setOutlineColor(sf::Color::Black);
        s->setOutlineThickness(2);
        s->setPosition(500, 500);

        sf::CircleShape* e = m_endPoint.makeShape<sf::CircleShape>(w);
        e->setOrigin(w, w);
        e->setFillColor(sf::Color::Transparent);
        e->setOutlineColor(sf::Color::Black);
        e->setOutlineThickness(2);
        e->setPosition(600, 500);

        m_startPoint.makeInteraction<act::Drag>([this](sf::Shape& shape, bool active)
                                                {
                                                    shape.setOutlineThickness(shape.getOutlineThickness() + active ? 2 : -2);
                                                    m_arrow.setStartPosition(m_startPoint.getShape()->getPosition());
                                                });
        m_endPoint.makeInteraction<act::Drag>([this](sf::Shape& shape, bool active)
                                              {
                                                  shape.setOutlineThickness(shape.getOutlineThickness() + active ? 2 : -2);
                                                  m_arrow.setEndPosition(m_endPoint.getShape()->getPosition());
                                              });
    }

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(m_arrow, states);
        target.draw(m_endPoint, states);
        target.draw(m_startPoint, states);
    }
public:
    act::ArrowShape m_arrow;
    act::Shape m_startPoint;
    act::Shape m_endPoint;
};



int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window{{1000, 1000}, "ShowCase", sf::Style::Default, settings};
    window.setFramerateLimit(60);
    sf::Event event;

    MovableArrow arrow;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            arrow.m_startPoint.processEvent(event);
            arrow.m_endPoint.processEvent(event);
        }

        window.clear(sf::Color::White);

        window.draw(arrow);
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


