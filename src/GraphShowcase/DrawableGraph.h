#ifndef DRAWABLEGRAPH_H
#define DRAWABLEGRAPH_H

#include <SFML/Graphics.hpp>
#include "Graph/algo.h"
#include "Interact/Shape.h"


class DrawableGraph : public sf::Drawable
{
public:
    struct Data
    {
        using Node = act::Shape;
        // struct Node
        // {
        //     sf::Vector2i pos;
        //     int radius = 10;
        //     sf::Color color;
        // };

        struct Edge
        {
            double weight = 2;
            sf::Color color;
        };
    };

    DrawableGraph();

    void processEvent(sf::Event event);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void _createRelativeNeighborhoodGraph();

    sf::Font m_font;

    graph::Graph<Data> m_graph;
    std::vector<std::pair<graph::EdgeIdx, graph::NodeIdx>> m_path;

    graph::NodeIdx m_start = 0;
    graph::NodeIdx m_end = 0;
};

#endif // DRAWABLEGRAPH_H
