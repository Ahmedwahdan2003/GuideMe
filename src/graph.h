#ifndef GRAPH_H
#define GRAPH_H
#include<map>
#include<string>
#include <vector>
#include<QString>
struct Transportation {
private:
    std::string name;
    int cost;
public:
    Transportation(std::string s, int n) : name(std::move(s)), cost(n) {}

    // Getter for name
    std::string getName() const {
        return name;
    }

    // Setter for name
    void setName(const std::string& newName) {
        name = newName;
    }

    // Getter for cost
    int getCost() const {
        return cost;
    }

    // Setter for cost
    void setCost(int newCost) {
        cost = newCost;
    }
};

struct Node {
    private:std::string nodeName;
    public:
    Node(std::string Name) : nodeName(std::move(Name)) {}

    // Getter for nodeName
    std::string getNodeName() const {
        return nodeName;
    }

    // Setter for nodeName
    void setNodeName(const std::string& newName) {
        nodeName = newName;
    }
};

struct Edge {
private:
    Node parent;
    Node destination;
    std::vector<Transportation> options;
public:
    Edge(Node prnt, Node dest, std::vector<Transportation> opts)
        : parent(std::move(prnt)), destination(std::move(dest)), options(std::move(opts)) {}

    // Getter for parent
    Node getParent() const {
        return parent;
    }

    // Setter for parent
    void setParent(const Node& newParent) {
        parent = newParent;
    }

    // Getter for destination
    Node getDestination() const {
        return destination;
    }

    // Setter for destination
    void setDestination(const Node& newDest) {
        destination = newDest;
    }

    // Getter for options
    std::vector<Transportation> getOptions() const {
        return options;
    }

    // Setter for options
    void setOptions(const std::vector<Transportation>& newOptions) {
        options = newOptions;
    }
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
 std::vector<Node>getNodes();

 void addEdge(Node From,Node dest,std::vector<Transportation>opts);
 void updateEdge(Edge Target);                                          //{WAHDAN}==> THESE FUNCTIONS WILL BE IMPLEMENTED BY [RAHAF-RAWAN,BASMALA1]
 void deleteEdge(Edge Target);                                          //{WAHDAN}==> A GRAPH IS COMPLETE IF EVERY NODE HAS EDGES TO EVERY OTHER NODE IN THE GRAPH
 bool isCompleteGraph();                                                //{WAHDAN}==> SEARCH ON HOW TO DO IT USING AN ALGORITHM

 void DFS(Node StartNode);
 void BFS(Node StartNode);

};
#endif // GRAPH_H
