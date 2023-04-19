#ifndef GRAPHWITHVECTOR_H
#define GRAPHWITHVECTOR_H

#include "GraphItf.h"
#include <algorithm>


namespace graph
{
template<class Data> class NodeWithVector;
template<class Data> class EdgeWithVector;

template<class Data>
class Memory
{
public:
    std::vector<NodeWithVector<Data>> m_nodes;
    std::vector<EdgeWithVector<Data>> m_edges;
};

using Index = size_t;

template<class Data>
class EdgeWithVector : public EdgeItf<Data, NodeWithVector<Data>, EdgeWithVector<Data>>
{
    using Node = typename EdgeItf<Data, NodeWithVector<Data>, EdgeWithVector<Data>>::Node;
    using Edge = typename EdgeItf<Data, NodeWithVector<Data>, EdgeWithVector<Data>>::Edge;
public:
    EdgeWithVector(Index start, Index end) : m_startNode(start), m_endNode(end) {}

    std::pair<Index, Index> getNodes() override { return {m_startNode, m_endNode}; }
    std::pair<const Index, const Index> getNodes() const override { return {m_startNode, m_endNode}; }
    Index& getStart() override { return m_startNode; }
    Index getStart() const override { return m_startNode; }
    Index& getEnd() override { return m_endNode; }
    Index getEnd() const override { return m_endNode; }

protected:
    Index m_index = -1;
    Index m_startNode;
    Index m_endNode;

    Memory<Data>* m_memory;
};

template<class Data>
class NodeWithVector : public NodeItf<Data, NodeWithVector<Data>, EdgeWithVector<Data>>
{
    using Node = typename NodeItf<Data, NodeWithVector<Data>, EdgeWithVector<Data>>::Node;
    using Edge = typename NodeItf<Data, NodeWithVector<Data>, EdgeWithVector<Data>>::Edge;
public:
    NodeWithVector() {}

    Index connectNode(Node* neighbor) override
    {
        Index edgeId = m_memory->m_edges.size();
        m_edges.push_back(edgeId);
        neighbor->m_edges.push_back(edgeId);
        m_memory->m_edges.emplace_back(edgeId, m_index, neighbor->m_index);
        return edgeId;
    }
    std::vector<Node*> getNeighbors() const override
    {
        std::vector<Node*> ret{m_edges.size(), nullptr};
        std::transform(m_edges.begin(), m_edges.end(), ret.begin(), [this](auto e){ return m_memory->m_edges[e->getStart() != m_index ? e->getStart() : e->getEnd()]; });
        return ret;
    }
    std::vector<Edge*> getEdges() const override
    {
        std::vector<Edge*> ret{m_edges.size(), nullptr};
        std::transform(m_edges.begin(), m_edges.end(), ret.begin(), [this](auto e){ return m_memory->m_edges[e]; });
        return ret;
    }

protected:
    Index m_index = -1;
    std::vector<Index> m_edges;

    Memory<Data>* m_memory;
};

void foo();

}

#endif // GRAPHWITHVECTOR_H
