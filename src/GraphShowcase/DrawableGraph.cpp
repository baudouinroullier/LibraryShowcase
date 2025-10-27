#include "DrawableGraph.h"
#include <fmt/core.h>
#include <cmath>
#include <random>
#include <Interact/Drag.h>


DrawableGraph::DrawableGraph()
{
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(100, 700);

    const int N = 1000;

    for (int i=0; i<N; ++i)
    {
        act::Shape s;
        auto* c = s.makeShape<sf::CircleShape>(4);
        c->setOrigin({2, 2});
        c->setPosition({distrib(gen), distrib(gen)});
        c->setOutlineColor(sf::Color::Black);
        c->setOutlineThickness(2);
        c->setFillColor(sf::Color::White);
        s.makeInteraction<act::Drag>([this](sf::Shape&, bool){});
        m_graph.createNode(std::move(s));
    }

    _createRelativeNeighborhoodGraph();
}

void DrawableGraph::processEvent(std::optional<sf::Event> event)
{
    for (auto& n : m_graph.nodes())
    {
        if (n.second.data.processEvent(event))
        {
            _createRelativeNeighborhoodGraph();
            return;
        }
    }

    if (sf::Event::MouseButtonPressed* mousePress = event->getIf<sf::Event::MouseButtonPressed>();
        mousePress && mousePress->button == sf::Mouse::Button::Left)
    {
        act::Shape s;
        auto* c = s.makeShape<sf::CircleShape>(4);
        c->setOrigin({2, 2});
        c->setPosition(sf::Vector2f{mousePress->position});
        c->setOutlineColor(sf::Color::Black);
        c->setOutlineThickness(2);
        c->setFillColor(sf::Color::White);
        s.makeInteraction<act::Drag>([this](sf::Shape&, bool){});
        m_graph.createNode(std::move(s));
        _createRelativeNeighborhoodGraph();
    }

    if (sf::Event::MouseButtonPressed* mousePress = event->getIf<sf::Event::MouseButtonPressed>();
        mousePress && mousePress->button == sf::Mouse::Button::Right)
    {
        const int N = m_graph.nodes().size();
        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, N-1);

        m_start = distrib(gen);
        m_end = distrib(gen);
        m_path = graph::shortestPath(m_graph, m_start, m_end,
            [](const Data::Edge& edge){ return edge.weight; },
            [](const Data::Node& n1, const Data::Node& n2)
            { return std::sqrt(std::pow(n1.getShape()->getPosition().x - n2.getShape()->getPosition().x, 2.) + std::pow(n1.getShape()->getPosition().y - n2.getShape()->getPosition().y, 2.)); });

        for (auto& [i,n] : m_graph.nodes())
            n.data.getShape()->setOutlineColor(sf::Color::Black);
        for (auto& [i,e] : m_graph.edges())
            e.data.color = sf::Color::Black;

        for (auto& [e,n] : m_path)
        {
            m_graph.node(n).data.getShape()->setOutlineColor(sf::Color::Red);
            if (e != -1) m_graph.edge(e).data.color = sf::Color::Red;
        }
    }
}

void DrawableGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::RectangleShape rec;

    for (const auto& [i, e] : m_graph.edges())
    {
        sf::Vector2f d1Pos = m_graph.node(e.startNode).data.getShape()->getPosition();
        sf::Vector2f d2Pos = m_graph.node(e.endNode).data.getShape()->getPosition();
        sf::Angle angle = sf::radians(std::atan2(d1Pos.y - d2Pos.y, d1Pos.x - d2Pos.x));
        double dist = std::sqrt(std::pow(d1Pos.x-d2Pos.x, 2.)+std::pow(d1Pos.y-d2Pos.y, 2.));

        rec.setSize(sf::Vector2f(dist, e.data.weight));
        rec.setOrigin({dist/2, e.data.weight/2});
        rec.setPosition({(d1Pos.x+d2Pos.x)/2, (d1Pos.y+d2Pos.y)/2});
        rec.setRotation(angle);
        rec.setFillColor(e.data.color);

        target.draw(rec, states);
    }

    for (const auto& [i,n] : m_graph.nodes())
    {
        target.draw(n.data, states);
    }
}

void DrawableGraph::_createRelativeNeighborhoodGraph()
{
    auto copy = m_graph.edges();
    for (auto& [i,e] : copy)
        m_graph.deleteEdge(i);

    const int N = m_graph.nodes().size();

    for (int i = 0; i < N; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            auto pi = m_graph.node(i).data.getShape()->getPosition();
            auto pj = m_graph.node(j).data.getShape()->getPosition();
            double dij = std::sqrt(std::pow(pi.x - pj.x, 2.) + std::pow(pi.y - pj.y, 2.));
            bool createLink = true;

            for (int k = 0; k < N; ++k)
            {
                if (k != i && k != j)
                {
                    auto pk = m_graph.node(k).data.getShape()->getPosition();
                    if (std::sqrt(std::pow(pi.x - pk.x, 2.) + std::pow(pi.y - pk.y, 2.)) < dij &&
                        std::sqrt(std::pow(pj.x - pk.x, 2.) + std::pow(pj.y - pk.y, 2.)) < dij)
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
