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


template<class Data, class CRTPNode, class CRTPEdge>
class EdgeItf
{
public:
    using Node = CRTPNode;
    using Edge = CRTPEdge;

    virtual std::pair<Node*, Node*> getNodes() = 0;
    virtual std::pair<const Node*, const Node*> getNodes() const = 0;
    virtual Node* getStart() = 0;
    virtual const Node* getStart() const = 0;
    virtual Node* getEnd() = 0;
    virtual const Node* getEnd() const = 0;

    const typename Data::Edge& data() const { return m_data; }
    typename Data::Edge& data() { return m_data; }
protected:
    typename Data::Edge m_data{};
};

template<class Data, class CRTPNode, class CRTPEdge>
class NodeItf
{
public:
    using Node = CRTPNode;
    using Edge = CRTPEdge;

    virtual Edge* connectNode(Node* neighbor) = 0;
    virtual std::vector<Node*> getNeighbors() const = 0;
    virtual std::vector<Edge*> getEdges() const = 0;

    const typename Data::Node& data() const { return m_data; }
    typename Data::Node& data() { return m_data; }
protected:
    typename Data::Node m_data{};
};

}

#endif // GRAPHITF_H
