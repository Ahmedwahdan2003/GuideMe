#ifndef GRAPH_H
#define GRAPH_H
#include<map>
#include<string>
#include <vector>
#include<QString>
struct Transportation
{
    std::string name;
    int cost;
    Transportation(std::string &s, int n):name(s),cost(n){}
};

struct Node
{
    std::string nodeName;
    Node(std::string Name){nodeName=Name;}
    bool operator<(const Node& other) const {
        return nodeName < other.nodeName;
    }
};
struct Edge
{   Node parent;
    Node destination;
    std::vector<Transportation>options;
    Edge(Node prnt,Node dest,std::vector<Transportation>opts):parent(prnt),destination(dest),options(opts){}
};
class Graph
{

private: std::map<Node,std::vector<Edge>>adjcencyList;
public:
    Graph();

 bool readGraphFile(const QString&fileName);
 void printGraph();
 void addNode(const Node newNode);
 std::vector<Edge>getEdges(Node Source);

 void addEdge(Node From,Node dest,std::vector<Transportation>opts);
 void updateEdge(Edge Target);                                          //{WAHDAN}==> THESE FUNCTIONS WILL BE IMPLEMENTED BY [RAHAF-RAWAN,BASMALA1]
 void deleteEdge(Edge Target);                                          //{WAHDAN}==> A GRAPH IS COMPLETE IF EVERY NODE HAS EDGES TO EVERY OTHER NODE IN THE GRAPH
 bool isCompleteGraph();                                                //{WAHDAN}==> SEARCH ON HOW TO DO IT USING AN ALGORITHM

 void DFS(Node StartNode);
 void BFS(Node StartNode);

};
#endif // GRAPH_H
