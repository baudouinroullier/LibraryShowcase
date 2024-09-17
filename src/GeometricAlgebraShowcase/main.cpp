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
        s->setOutlineThickness(4);
        s->setPosition(500, 500);

        sf::CircleShape* e = m_endPoint.makeShape<sf::CircleShape>(width);
        e->setOrigin(width, width);
        e->setFillColor(sf::Color::Transparent);
        e->setOutlineColor(sf::Color::Black);
        e->setOutlineThickness(2);
        e->setPosition(750, 500);

        m_startPos = {500, 500};
        m_endPos = {750, 500};

        m_startPoint.makeInteraction<act::Drag>([this](sf::Shape& shape, bool active)
                                                {
                                                    shape.setOutlineThickness(active ? 4 : 2);
                                                    setPosition(m_startPoint.getShape()->getPosition());
                                                    m_endPoint.getShape()->setPosition(m_endPos);
                                                });
        m_endPoint.makeInteraction<act::Drag>([this](sf::Shape& shape, bool active)
                                              {
                                                  shape.setOutlineThickness(active ? 4 : 2);
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

    sf::RectangleShape axes[6];
    axes[0].setSize({1000, 2});
    axes[1].setSize({2, 1000});
    axes[2].setSize({50, 2});
    axes[3].setSize({50, 2});
    axes[4].setSize({2, 50});
    axes[5].setSize({2, 50});
    for (sf::RectangleShape& a : axes)
    {
        a.setOrigin(a.getSize() / 2.f);
        a.setFillColor({128,128,128});
    }
    axes[0].setPosition({2*scale, 2*scale});
    axes[1].setPosition({2*scale, 2*scale});
    axes[2].setPosition({2*scale, 2*scale+scale});
    axes[3].setPosition({2*scale, 2*scale-scale});
    axes[4].setPosition({2*scale+scale, 2*scale});
    axes[5].setPosition({2*scale-scale, 2*scale});

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

            sf::Vector2f aStart = a.m_startPoint.getShape()->getPosition();
            sf::Vector2f aEnd = a.m_endPoint.getShape()->getPosition();
            sf::Vector2f bStart = b.m_startPoint.getShape()->getPosition();
            sf::Vector2f bEnd = b.m_endPoint.getShape()->getPosition();
            galg::Multivector2 mvA = galg::Multivector2{(aEnd - aStart).x, aStart.x - 2 * scale, aStart.y - 2 * scale, (aEnd - aStart).y} / scale;
            galg::Multivector2 mvB = galg::Multivector2{(bEnd - bStart).x, bStart.x - 2 * scale, bStart.y - 2 * scale, (bEnd - bStart).y} / scale;
            galg::Multivector2 mvC = (mvA * mvB);

            mvC *= scale;
            c.setStartPosition(0, 0);
            c.setEndPosition(mvC.rotor().real(), mvC.rotor().imag());
            c.move(mvC.vector().x() + 2*scale, mvC.vector().y() + 2*scale);
        }

        window.clear(sf::Color::White);

        for (sf::RectangleShape& a : axes)
            window.draw(a);

        window.draw(a);
        window.draw(b);
        window.draw(c);
        window.display();
    }

    return 0;
}


