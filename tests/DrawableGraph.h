#ifndef DRAWABLEGRAPH_H
#define DRAWABLEGRAPH_H

#include <SFML/Graphics.hpp>
#include <graph/algo.h>
#include "bla/Vector.hpp"

struct GraphData
{
    struct Node
    {
        sf::Vector2i pos;
        int radius = 10;
        sf::Color color;
    };

    struct Edge
    {
        double weight = 2;
        sf::Color color;
    };
};


class DrawableGraph : public sf::Drawable
{
public:
    DrawableGraph();

    void manageMouseEvent(sf::Event::MouseButtonEvent event);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Font m_font;

    graph::Graph<GraphData> m_graph;
    std::vector<std::pair<graph::EdgeIdx, graph::NodeIdx>> m_path;

    graph::NodeIdx m_start = 0;
    graph::NodeIdx m_end = 0;
};

#endif // DRAWABLEGRAPH_H
