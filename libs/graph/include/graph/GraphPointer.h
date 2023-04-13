#ifndef GRAPHPOINTER_H
#define GRAPHPOINTER_H

#include "GraphItf.h"
#include <memory>

namespace graph
{

template<class Data>
class GraphEdgesPointer : public GraphEdgeItf<Data>
{
};

template<class Data>
class GraphNodePointer : public GraphNodeItf<Data>
{
public:
    GraphNodePointer();

    std::vector<GraphNodeItf<Data>*> getNeighbors() override;
    std::vector<GraphEdgeItf<Data>*> getEdges() override;

protected:
    std::vector<std::shared_ptr<GraphEdgesPointer<Data>>> m_edges;
};

}

#endif // GRAPHPOINTER_H
