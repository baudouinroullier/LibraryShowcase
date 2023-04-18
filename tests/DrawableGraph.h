#ifndef DRAWABLEGRAPH_H
#define DRAWABLEGRAPH_H

#include <SFML/Graphics.hpp>
#include <graph/GraphPointer.h>

class GraphData
{
public:
    class Node
    {
    public:
        int x = 0;
        int y = 0;
        int radius = 10;
        sf::Color color;
    };

    class Edge
    {
    public:
        int weight;
    };
};


class DrawableGraph : public sf::Drawable
{
public:
    DrawableGraph();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    graph::NodeWithPointer<GraphData> m_nodes[5];
};

#endif // DRAWABLEGRAPH_H
