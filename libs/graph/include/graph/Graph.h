#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <map>
#include <queue>
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
    std::vector<NodeIdx> getNeighbors() const
    {
        std::set<NodeIdx> uniques;
        std::transform(links.begin(), links.end(), uniques.begin(), [](auto l){ return l.second; });
        std::vector<NodeIdx> ret{uniques.size()};
        std::copy(uniques.begin(), uniques.end(), ret.begin());
        return ret;
    }

    std::vector<EdgeIdx> getEdges() const
    {
        std::vector<EdgeIdx> ret{links.size()};
        std::transform(links.begin(), links.end(), ret.begin(), [this](auto l){ return l.first; });
        return ret;
    }

    NodeIdx index = -1;
    typename Data::Node data{};

    std::map<EdgeIdx, NodeIdx> links {};
};

template<class Data>
class Graph
{
public:
    Node<Data>& node(NodeIdx i) { return m_nodes[i]; }
    Edge<Data>& edge(EdgeIdx i) { return m_edges[i]; }
    const Node<Data>& node(NodeIdx i) const { return m_nodes.at(i); }
    const Edge<Data>& edge(EdgeIdx i) const { return m_edges.at(i); }

    std::map<NodeIdx, Node<Data>>& nodes() { return m_nodes; }
    std::map<EdgeIdx, Edge<Data>>& edges() { return m_edges; }
    const std::map<NodeIdx, Node<Data>>& nodes() const { return m_nodes; }
    const std::map<EdgeIdx, Edge<Data>>& edges() const { return m_edges; }

    NodeIdx createNode(typename Data::Node&& data)
    {
        NodeIdx i = m_curNodeIdx++;
        m_nodes[i] = Node<Data>{i, std::forward<typename Data::Node>(data)};
        return i;
    }

    EdgeIdx linkNodes(NodeIdx node1, NodeIdx node2, typename Data::Edge&& data = {})
    {
        EdgeIdx i = m_curEdgeIdx++;
        m_edges[i] = Edge<Data>{node1, node2, i, std::forward<typename Data::Edge>(data)};
        m_nodes[node1].links[i] = node2;
        m_nodes[node2].links[i] = node1;
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

    template<class EdgeLengthFunction, class HeuristicFunction>
    std::vector<std::pair<EdgeIdx, NodeIdx>> shortestPath(NodeIdx start, NodeIdx end,
        EdgeLengthFunction f = [](typename Data::Edge){ return 1; },
        HeuristicFunction h = [](typename Data::Node, typename Data::Node){ return 0; })
    {
        struct RankedNode
        {
            NodeIdx node;
            double costToNode;
            double heuristicToEnd;
            double totalCost() const { return heuristicToEnd + costToNode; }
        };

        auto cmp = [](auto rn1, auto rn2){ return rn1.totalCost() > rn2.totalCost(); };
        std::priority_queue<RankedNode, std::vector<RankedNode>, decltype(cmp)> openList{cmp};
        std::map<NodeIdx, double> closedList;
        std::map<NodeIdx, std::pair<EdgeIdx, NodeIdx>> cameFrom;

        openList.push({end, 0, 0});

        do
        {
            RankedNode current = openList.top();
            openList.pop();

            if (current.node == start)
            {
                std::vector<std::pair<EdgeIdx, NodeIdx>> path;
                path.reserve(cameFrom.size()/2);
                path.push_back(std::make_pair(-1, start));
                while (current.node != end)
                {
                    path.push_back(cameFrom[current.node]);
                    current.node = path.back().second;
                }
                return path;
            }

            for (auto [edgeIdx, neighborIdx] : node(current.node).links)
            {
                double tentativeScore = current.costToNode + f(edge(edgeIdx).data);
                if (!closedList.contains(neighborIdx) || closedList[neighborIdx] > tentativeScore)
                {
                    closedList[neighborIdx] = tentativeScore;
                    cameFrom[neighborIdx] = std::make_pair(edgeIdx, current.node);
                    openList.push({neighborIdx, tentativeScore, h(node(neighborIdx).data, node(start).data)});
                }
            }
        }
        while (!openList.empty());

        return {};
    }

protected:
    std::map<NodeIdx, Node<Data>> m_nodes;
    std::map<NodeIdx, Edge<Data>> m_edges;
    NodeIdx m_curNodeIdx = 0;
    EdgeIdx m_curEdgeIdx = 0;
};

}

#endif // GRAPH_H
