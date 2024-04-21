#ifndef GRAPH_H
#define GRAPH_H
#include<map>
#include<vector>
#include<QString>

struct Transportation {
    QString name;
    int cost;

    Transportation(QString s, int n) : name(std::move(s)), cost(n) {}

    // Getter for name
    QString getName() const {
        return name;
    }

    // Setter for name
    void setName(const QString& newName) {
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
    QString nodeName;
    float centerX;
    float centerY;
    int radius;

    Node(QString Name) : nodeName(std::move(Name)) {
        centerX=1;
        centerY=1;
        radius=20;
    }
    Node(QString name,int cenX,int cenY,int rad):nodeName(name),centerX(cenX),centerY(cenY),radius(rad){

    }
    bool operator<(const Node& other) const {
        return nodeName < other.nodeName;
    }
    bool operator==(const Node& other) const {
        return nodeName == other.nodeName;
    }
    // Getter for nodeName
    QString getNodeName() const {
        return nodeName;
    }

    // Setter for nodeName
    void setNodeName(const QString& newName) {
        nodeName = newName;
    }
};

struct Edge {
    Node parent;
    Node destination;
    std::vector<Transportation> options;
    float startX;
    float startY;
    float endX;
    float endY;
    Edge(Node prnt, Node dest, std::vector<Transportation> opts)
        : parent(std::move(prnt)), destination(std::move(dest)), options(std::move(opts)) {
    startX=1;
    startY=1;
    endX=1;
    endY=1;
    }

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
Node back();
 bool readGraphFile( const QString&fileName);
 void printGraph();
 void addNode( Node newNode);
 std::vector<Edge>getEdges(Node Source);
 std::vector<Node>getNodes();
 bool empty();
 void addEdge(Node From,Node dest,std::vector<Transportation>opts);
 void updateEdge(Edge Target);                                          //{WAHDAN}==> THESE FUNCTIONS WILL BE IMPLEMENTED BY [RAHAF-RAWAN,BASMALA1]
 void deleteEdge(Edge Target);                                          //{WAHDAN}==> A GRAPH IS COMPLETE IF EVERY NODE HAS EDGES TO EVERY OTHER NODE IN THE GRAPH
 bool isCompleteGraph();                                                //{WAHDAN}==> SEARCH ON HOW TO DO IT USING AN ALGORITHM

 std::vector<Node> DFS(Node& StartNode);
 std::vector<Node> BFS(Node& StartNode);
 std::vector<Edge>getEdges();
 static Graph& getInstance() {
     // Static local variable to ensure it's initialized only once
     static Graph instance;
     return instance;
 }
};
#endif // GRAPH_H
