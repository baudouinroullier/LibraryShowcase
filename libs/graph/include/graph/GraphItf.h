#ifndef GRAPHITF_H
#define GRAPHITF_H

#include <vector>

namespace graph
{

class EmpyData
{
    class Node {};
    class Edge {};
};


template<class Data>
class GraphEdgeItf
{
public:
    virtual std::pair<GraphEdgeItf<Data>*, GraphEdgeItf<Data>*> getNodes() = 0;
    virtual GraphEdgeItf<Data>* getStart() = 0;
    virtual GraphEdgeItf<Data>* getEnd() = 0;
protected:
    typename Data::Edge m_data;
};

template<class Data>
class GraphNodeItf
{
public:
    GraphNodeItf();

    virtual std::vector<GraphNodeItf<Data>*> getNeighbors() = 0;
    virtual std::vector<GraphEdgeItf<Data>*> getEdges() = 0;
protected:
    typename Data::Node m_data;
};

}

#endif // GRAPHITF_H
