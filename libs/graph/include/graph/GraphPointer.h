#ifndef GRAPHPOINTER_H
#define GRAPHPOINTER_H

#include "GraphItf.h"
#include <algorithm>
#include <memory>
#include <fmt/core.h>

namespace graph
{
template<class Data> class NodeWithPointer;

template<class Data>
class EdgeWithPointer : public EdgeItf<Data, NodeWithPointer<Data>, EdgeWithPointer<Data>>
{
    using Node = typename EdgeItf<Data, NodeWithPointer<Data>, EdgeWithPointer<Data>>::Node;
    using Edge = typename EdgeItf<Data, NodeWithPointer<Data>, EdgeWithPointer<Data>>::Edge;
public:
    EdgeWithPointer(Node* start, Node* end) : m_startNode(start), m_endNode(end) {}

    std::pair<Node*, Node*> getNodes() override { return {m_startNode, m_endNode}; }
    std::pair<const Node*, const Node*> getNodes() const override { return {m_startNode, m_endNode}; }
    Node* getStart() override { return m_startNode; }
    const Node* getStart() const override { return m_startNode; }
    Node* getEnd() override { return m_endNode; }
    const Node* getEnd() const override { return m_endNode; }

protected:
    Node* m_startNode;
    Node* m_endNode;
};

template<class Data>
class NodeWithPointer : public NodeItf<Data, NodeWithPointer<Data>, EdgeWithPointer<Data>>
{
    using Node = typename NodeItf<Data, NodeWithPointer<Data>, EdgeWithPointer<Data>>::Node;
    using Edge = typename NodeItf<Data, NodeWithPointer<Data>, EdgeWithPointer<Data>>::Edge;
public:
    NodeWithPointer() {}

    Edge* connectNode(Node* neighbor) override
    {
        auto edge = std::make_shared<Edge>(this, neighbor);
        m_edges.push_back(edge);
        neighbor->m_edges.push_back(edge);
        return edge.get();
    }
    std::vector<Node*> getNeighbors() const override
    {
        std::vector<Node*> ret{m_edges.size(), nullptr};
        std::transform(m_edges.begin(), m_edges.end(), ret.begin(), [this](auto e){ return e->getStart() != this ? e->getStart() : e->getEnd(); });
        return ret;
    }
    std::vector<Edge*> getEdges() const override
    {
        std::vector<Edge*> ret{m_edges.size(), nullptr};
        std::transform(m_edges.begin(), m_edges.end(), ret.begin(), [this](auto e){ return e.get(); });
        return ret;
    }

protected:
    std::vector<std::shared_ptr<Edge>> m_edges;
};

void foo();

}

#endif // GRAPHPOINTER_H
