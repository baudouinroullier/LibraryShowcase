#include "DrawableGraph.h"
#include <fmt/core.h>
#include <cmath>

DrawableGraph::DrawableGraph()
{
    m_nodes[0].data() = {250, 600, 10, sf::Color::Black};
    m_nodes[1].data() = {550, 600, 10, sf::Color::Black};
    m_nodes[2].data() = {250, 300, 10, sf::Color::Black};
    m_nodes[3].data() = {550, 300, 10, sf::Color::Black};
    m_nodes[4].data() = {400, 100, 10, sf::Color::Black};

    m_nodes[0].connectNode(&m_nodes[1])->data().weight = 2;
    m_nodes[0].connectNode(&m_nodes[2])->data().weight = 2;
    m_nodes[1].connectNode(&m_nodes[2])->data().weight = 2;
    m_nodes[1].connectNode(&m_nodes[3])->data().weight = 2;
    m_nodes[2].connectNode(&m_nodes[3])->data().weight = 2;
    m_nodes[2].connectNode(&m_nodes[4])->data().weight = 2;
    m_nodes[3].connectNode(&m_nodes[4])->data().weight = 2;
}

void DrawableGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::CircleShape c;
    c.setOutlineColor(sf::Color::Black);
    c.setOutlineThickness(3);

    sf::RectangleShape rec;

    for (const auto& n : m_nodes)
    {
        const auto& data = n.data();
        for (auto* e : n.getEdges())
        {
            if (e->getStart() == &n)
            {
                const auto* other = e->getEnd();
                const auto& odata = other->data();

                double angle = std::atan2(data.y-odata.y, data.x-odata.x);
                double dist = std::sqrt(std::pow(data.x-odata.x, 2.)+std::pow(data.y-odata.y, 2.));

                const double gamma = 2.2;
                sf::Color color(
                    std::pow(0.5*std::pow(data.color.r/255., gamma) + 0.5*std::pow(odata.color.r/255., gamma), 1/gamma)*255,
                    std::pow(0.5*std::pow(data.color.g/255., gamma) + 0.5*std::pow(odata.color.g/255., gamma), 1/gamma)*255,
                    std::pow(0.5*std::pow(data.color.b/255., gamma) + 0.5*std::pow(odata.color.b/255., gamma), 1/gamma)*255);


                rec.setSize(sf::Vector2f(dist, e->data().weight));
                rec.setOrigin(dist/2, e->data().weight/2);
                rec.setPosition((data.x+odata.x)/2, (data.y+odata.y)/2);
                rec.setRotation(180*angle/M_PI);
                rec.setFillColor(color);

                target.draw(rec, states);
            }
        }
    }

    for (const auto& n : m_nodes)
    {
        const auto& data = n.data();
        const double r = data.radius;
        c.setRadius(r);
        c.setOrigin(r, r);
        c.setPosition(data.x, data.y);
        c.setFillColor(data.color);
        target.draw(c, states);
    }
}
