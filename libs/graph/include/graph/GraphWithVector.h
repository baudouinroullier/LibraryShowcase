#ifndef GRAPHWITHVECTOR_H
#define GRAPHWITHVECTOR_H

#include <algorithm>
#include <map>
#include <set>
#include <vector>

namespace graph
{
using NodeIdx = size_t;
using EdgeIdx = size_t;


template<class Data>
class Edge
{
public:
    NodeIdx startNode = -1;
    NodeIdx endNode = -1;
    EdgeIdx index = -1;

    typename Data::Edge data{};
};

template<class Data>
class Node
{
public:
    std::vector<NodeIdx> getNeighbors() const override
    {
        std::set<NodeIdx> uniques;
        std::transform(links.begin(), links.end(), uniques.begin(), [](auto l){ return l.second; });
        std::vector<NodeIdx> ret{uniques.size()};
        std::copy(uniques.begin(), uniques.end(), ret.begin());
        return ret;
    }

    std::vector<EdgeIdx> getEdges() const override
    {
        std::vector<EdgeIdx> ret{links.size()};
        std::transform(links.begin(), links.end(), ret.begin(), [this](auto l){ return l.first; });
        return ret;
    }

    NodeIdx index = -1;
    typename Data::Node data{};

    std::map<EdgeIdx, NodeIdx> links;
};

template<class Data>
class Graph
{
public:
    Node<Data>* node(NodeIdx i) { return m_nodes[i]; }
    Edge<Data>* edge(EdgeIdx i) { return m_edges[i]; }
    const Node<Data>* node(NodeIdx i) const { return m_nodes[i]; }
    const Edge<Data>* edge(EdgeIdx i) const { return m_edges[i]; }

    std::map<NodeIdx, Node<Data>> nodes() { return m_nodes; }
    std::map<EdgeIdx, Edge<Data>> edges() { return m_edges; }
    const std::map<NodeIdx, Node<Data>> nodes() const { return m_nodes; }
    const std::map<EdgeIdx, Edge<Data>> edges() const { return m_edges; }

    NodeIdx createNode(typename Data::Node&& data)
    {
        NodeIdx i = m_nodes.size();
        m_nodes.emplace(i, {i, std::forward(data)});
        return i;
    }

    EdgeIdx linkNodes(NodeIdx node1, NodeIdx node2, typename Data::Edge&& data = {})
    {
        EdgeIdx i = m_edges.size();
        m_edges.emplace(i, {node1, node2, i, std::forward(data)});
        m_nodes[node1].links.push_back({node2, i});
        m_nodes[node2].links.push_back({node1, i});
        return i;
    }

    void deleteNode(NodeIdx index)
    {
        // delete node from neighbors
        for (auto nIdx : m_nodes[index]->getNeighbors())
            std::erase(m_nodes[nIdx].links, index);

        // delete edges
        for (auto eIdx : m_nodes[index]->getEdges())
            std::erase(m_edges, eIdx);

        // delete node
        std::erase(m_nodes, index);
    }

    void unlinkNodes(NodeIdx node1, NodeIdx node2)
    {
        for(const auto& l : node(node1)->links)
            if (l.second == node2)
                deleteEdge(l.first);
    }

    void deleteEdge(EdgeIdx index)
    {
        // delete node from neighbors
        std::erase_if(node(edge(index)->startNode)->links, [index](auto l){ return l.first == index; });
        std::erase_if(node(edge(index)->endNode)->links, [index](auto l){ return l.first == index; });

        // delete edges
        for (auto eIdx : m_nodes[index]->getEdges())
            std::erase(m_edges, eIdx);
    }

protected:
    std::map<NodeIdx, Node<Data>> m_nodes;
    std::map<NodeIdx, Edge<Data>> m_edges;
};

}

#endif // GRAPHWITHVECTOR_H
