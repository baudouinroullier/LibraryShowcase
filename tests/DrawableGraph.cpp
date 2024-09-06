#include "DrawableGraph.h"
#include <fmt/core.h>
#include <cmath>
#include <random>

DrawableGraph::DrawableGraph()
{
    m_font.loadFromFile("/usr/share/fonts/truetype/fonts-yrsa-rasa/Yrsa-Regular.ttf");

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(100, 700);

    const int N = 50;

    for (int i=0; i<N; ++i)
        m_graph.createNode({{distrib(gen), distrib(gen)}, 4, sf::Color::Black});

    for (int i=0; i<N; ++i)
    {
        for (int j=i+1; j<N; ++j)
        {
            auto pi = m_graph.node(i).data.pos;
            auto pj = m_graph.node(j).data.pos;
            double dij = std::sqrt(std::pow(pi.x-pj.x, 2.) + std::pow(pi.y-pj.y, 2.));
            bool createLink = true;

            for (int k = 0; k<N; ++k)
            {
                if (k != i && k != j)
                {
                    auto pk = m_graph.node(k).data.pos;
                    if (std::sqrt(std::pow(pi.x-pk.x, 2.) + std::pow(pi.y-pk.y, 2.)) < dij &&
                        std::sqrt(std::pow(pj.x-pk.x, 2.) + std::pow(pj.y-pk.y, 2.)) < dij)
                    {
                        createLink = false;
                        break;
                    }
                }
            }
            if (createLink)
                m_graph.linkNodes(i, j);
        }
    }
}

void DrawableGraph::manageMouseEvent(sf::Event::MouseButtonEvent event)
{
    if (event.button == sf::Mouse::Button::Left)
    {
        m_graph.createNode({{event.x, event.y}, 5, sf::Color::Black});
    }

    if (event.button == sf::Mouse::Button::Right)
    {
        const int N = 50;
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, N-1);

        m_start = distrib(gen);
        m_end = distrib(gen);
        m_path = graph::shortestPath(m_graph, m_start, m_end,
            [](const GraphData::Edge& edge){ return edge.weight; },
            [](const GraphData::Node& n1, const GraphData::Node& n2){ return std::sqrt(std::pow(n1.pos.x - n2.pos.x, 2.) + std::pow(n1.pos.y - n2.pos.y, 2.)); });

        for (auto& [i,n] : m_graph.nodes())
            n.data.color = sf::Color::Black;
        for (auto& [i,e] : m_graph.edges())
            e.data.color = sf::Color::Black;

        for (auto& [e,n] : m_path)
        {
            m_graph.node(n).data.color = sf::Color::Red;
            if (e != -1) m_graph.edge(e).data.color = sf::Color::Red;
        }
    }
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
        double angle = std::atan2(d1.pos.y - d2.pos.y, d1.pos.x - d2.pos.x);
        double dist = std::sqrt(std::pow(d1.pos.x-d2.pos.x, 2.)+std::pow(d1.pos.y-d2.pos.y, 2.));

        rec.setSize(sf::Vector2f(dist, e.data.weight));
        rec.setOrigin(dist/2, e.data.weight/2);
        rec.setPosition((d1.pos.x+d2.pos.x)/2, (d1.pos.y+d2.pos.y)/2);
        rec.setRotation(180*angle/M_PI);
        rec.setFillColor(e.data.color);

        target.draw(rec, states);

        text.setString(std::to_string(i));
        text.setPosition((d1.pos.x+d2.pos.x)/2, (d1.pos.y+d2.pos.y)/2);

        target.draw(text, states);
    }

    for (const auto& [i,n] : m_graph.nodes())
    {
        const double r = n.data.radius;
        c.setRadius(r);
        c.setOrigin(r, r);
        c.setPosition(n.data.pos.x, n.data.pos.y);
        c.setFillColor(sf::Color::White);
        c.setOutlineColor(n.data.color);
        c.setOutlineThickness(2);
        target.draw(c, states);

        text.setString(std::to_string(i));
        text.setPosition(n.data.pos.x, n.data.pos.y);

        target.draw(text, states);
    }
}
