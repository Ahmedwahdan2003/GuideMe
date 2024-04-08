#include "graph.h"

Graph::Graph() {}
void Graph::addNode(string name,Node N)
{
    adjcencyList.emplace(name, N);

}
