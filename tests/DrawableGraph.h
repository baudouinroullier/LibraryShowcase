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
        int weight = 2;
    };
};


class DrawableGraph : public sf::Drawable
{
public:
    DrawableGraph();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Font m_font;

    graph::Graph<GraphData> m_graph;
};

#endif // DRAWABLEGRAPH_H
