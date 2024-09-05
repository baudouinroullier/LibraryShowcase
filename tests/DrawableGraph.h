#ifndef DRAWABLEGRAPH_H
#define DRAWABLEGRAPH_H

#include <SFML/Graphics.hpp>
#include <graph/GraphWithVector.h>

struct GraphData
{
    struct Node
    {
        int x = 0;
        int y = 0;
        int radius = 10;
        sf::Color color;
    };

    struct Edge
    {
        double weight = 2;
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
    graph::NodeIdx m_end = 1;
};

#endif // DRAWABLEGRAPH_H
