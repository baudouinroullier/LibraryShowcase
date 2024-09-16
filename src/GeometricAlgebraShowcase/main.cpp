#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include "GeometricAlgebra/Vector.h"
#include "GeometricAlgebra/Multivector.h"
#include "Interact/Shape.h"
#include "Interact/Drag.h"
#include "Interact/Hover.h"
#include "Interact/ArrowShape.h"



class MovableArrow : public act::ArrowShape
{
public:
    MovableArrow(double width, sf::Color color) :
        ArrowShape(width, color)
    {
        sf::CircleShape* s = m_startPoint.makeShape<sf::CircleShape>(width);
        s->setOrigin(width, width);
        s->setFillColor(sf::Color::Transparent);
        s->setOutlineColor(sf::Color::Black);
        s->setOutlineThickness(2);
        s->setPosition(500, 500);

        sf::CircleShape* e = m_endPoint.makeShape<sf::CircleShape>(width);
        e->setOrigin(width, width);
        e->setFillColor(sf::Color::Transparent);
        e->setOutlineColor(sf::Color::Black);
        e->setOutlineThickness(2);
        e->setPosition(600, 500);

        m_startPoint.makeInteraction<act::Drag>([this](sf::Shape& shape, bool active)
                                                {
                                                    shape.setOutlineThickness(shape.getOutlineThickness() + active ? 2 : -2);
                                                    setStartPosition(m_startPoint.getShape()->getPosition());
                                                });
        m_endPoint.makeInteraction<act::Drag>([this](sf::Shape& shape, bool active)
                                              {
                                                  shape.setOutlineThickness(shape.getOutlineThickness() + active ? 2 : -2);
                                                  setEndPosition(m_endPoint.getShape()->getPosition());
                                              });
    }

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        ArrowShape::draw(target, states);
        target.draw(m_endPoint, states);
        target.draw(m_startPoint, states);
    }
public:
    act::Shape m_startPoint;
    act::Shape m_endPoint;
};



int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    const double scale = 250;
    const sf::Vector2f origin{2*scale, 2*scale};
    sf::RenderWindow window{{4*scale, 4*scale}, "ShowCase", sf::Style::Default, settings};
    window.setFramerateLimit(60);
    sf::Event event;

    MovableArrow a{10, sf::Color::Red};
    MovableArrow b{10, sf::Color::Blue};
    act::ArrowShape c{10, sf::Color::Magenta};

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            a.m_startPoint.processEvent(event) ||
            a.m_endPoint.processEvent(event) ||
            b.m_startPoint.processEvent(event) ||
            b.m_endPoint.processEvent(event);

            galg::Multivector2 mvA = galg::Multivector2{0, a.m_startPoint.getShape()->getPosition().x - 2*scale, a.m_startPoint.getShape()->getPosition().y - 2*scale, 0} / scale;
            galg::Multivector2 mvB = galg::Multivector2{0, b.m_startPoint.getShape()->getPosition().x - 2*scale, b.m_startPoint.getShape()->getPosition().y - 2*scale, 0} / scale;
            galg::Multivector2 mvC = scale*(mvA + mvB);

            c.setStartPosition(mvC.vector().x() + 2*scale, mvC.vector().y() + 2*scale);
            c.setEndPosition(2*scale, 2*scale);
        }

        window.clear(sf::Color::White);

        window.draw(a);
        window.draw(b);
        window.draw(c);
        window.display();
    }

    return 0;
}


