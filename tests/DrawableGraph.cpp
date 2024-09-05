#include "DrawableGraph.h"
#include <fmt/core.h>
#include <cmath>

DrawableGraph::DrawableGraph()
{
    m_font.loadFromFile("/usr/share/fonts/truetype/fonts-yrsa-rasa/Yrsa-Regular.ttf");


    graph::NodeIdx n1 = m_graph.createNode(GraphData::Node{250, 600, 10, sf::Color::Black});
    graph::NodeIdx n2 = m_graph.createNode(GraphData::Node{550, 600, 10, sf::Color::Black});
    graph::NodeIdx n3 = m_graph.createNode(GraphData::Node{250, 300, 10, sf::Color::Black});
    graph::NodeIdx n4 = m_graph.createNode(GraphData::Node{550, 300, 10, sf::Color::Black});
    graph::NodeIdx n5 = m_graph.createNode(GraphData::Node{400, 100, 10, sf::Color::Black});

    m_graph.linkNodes(n1, n2, {2});
    m_graph.linkNodes(n1, n3, {2});
    m_graph.linkNodes(n2, n3, {2});
    m_graph.linkNodes(n2, n4, {2});
    m_graph.linkNodes(n3, n4, {2});
    m_graph.linkNodes(n3, n5, {2});
    m_graph.linkNodes(n4, n5, {2});
}

void DrawableGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::CircleShape c;
    c.setOutlineColor(sf::Color::Black);
    c.setOutlineThickness(3);

    sf::RectangleShape rec;
    sf::Text text;

    text.setFont(m_font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);

    for (const auto& [i, e] : m_graph.edges())
    {
        GraphData::Node d1 = m_graph.node(e.startNode).data;
        GraphData::Node d2 = m_graph.node(e.endNode).data;
        double angle = std::atan2(d1.y-d2.y, d1.x-d2.x);
        double dist = std::sqrt(std::pow(d1.x-d2.x, 2.)+std::pow(d1.y-d2.y, 2.));

        const double gamma = 2.2;
        sf::Color color(
            std::pow(0.5*std::pow(d1.color.r/255., gamma) + 0.5*std::pow(d2.color.r/255., gamma), 1/gamma)*255,
            std::pow(0.5*std::pow(d1.color.g/255., gamma) + 0.5*std::pow(d2.color.g/255., gamma), 1/gamma)*255,
            std::pow(0.5*std::pow(d1.color.b/255., gamma) + 0.5*std::pow(d2.color.b/255., gamma), 1/gamma)*255);


        rec.setSize(sf::Vector2f(dist, e.data.weight));
        rec.setOrigin(dist/2, e.data.weight/2);
        rec.setPosition((d1.x+d2.x)/2, (d1.y+d2.y)/2);
        rec.setRotation(180*angle/M_PI);
        rec.setFillColor(color);

        target.draw(rec, states);

        text.setString(std::to_string(i));
        text.setPosition((d1.x+d2.x)/2, (d1.y+d2.y)/2);

        target.draw(text, states);
    }

    for (const auto& [i,n] : m_graph.nodes())
    {
        const double r = n.data.radius;
        c.setRadius(r);
        c.setOrigin(r, r);
        c.setPosition(n.data.x, n.data.y);
        c.setFillColor(n.data.color);
        target.draw(c, states);

        text.setString(std::to_string(i));
        text.setPosition(n.data.x, n.data.y);

        target.draw(text, states);
    }
}
