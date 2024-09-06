#ifndef ALGO_H
#define ALGO_H

#include "Graph.h"
#include "fmt/core.h"

namespace graph
{
template<class Data, class EdgeLengthFunction, class HeuristicFunction>
std::vector<std::pair<EdgeIdx, NodeIdx>> shortestPath(Graph<Data> graph, NodeIdx start, NodeIdx end,
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

        for (auto [edgeIdx, neighborIdx] : graph.node(current.node).links)
        {
            double tentativeScore = current.costToNode + f(graph.edge(edgeIdx).data);
            if (!closedList.contains(neighborIdx) || closedList[neighborIdx] > tentativeScore)
            {
                closedList[neighborIdx] = tentativeScore;
                cameFrom[neighborIdx] = std::make_pair(edgeIdx, current.node);
                openList.push({neighborIdx, tentativeScore, h(graph.node(neighborIdx).data, graph.node(start).data)});
            }
        }
    }
    while (!openList.empty());

    return {};
}
}


#endif // ALGO_H
