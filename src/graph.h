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

    Node(QString Name,float x,float y) : nodeName(std::move(Name)),centerX(x),centerY(y) {
        centerX=x;
        centerY=y;
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
    Transportation option;
    float startX;
    float startY;
    float endX;
    float endY;
    Edge(Node prnt, Node dest, Transportation opt)
        : parent(std::move(prnt)), destination(std::move(dest)), option(std::move(opt)) {
    startX=1;
    startY=1;
    endX=1;
    endY=1;
    }
    bool operator==(const Edge& other) const {
        return parent == other.parent && destination == other.destination && option.cost == other.option.cost && option.name == other.option.name;
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
    Transportation getOption() const {
        return option;
    }

    // Setter for options
    void setOptions(const Transportation& newOption) {
        option = newOption;
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
std::vector<Node> getNodes() ;
std::vector<Node>& getNodesRef() const;
const std::vector<Edge>& getEdgesRef() const;
 std::vector<Edge>getEdges();
 bool empty();
 void addEdge(Node From,Node dest,Transportation opt);
 void updateEdge(Edge Target);                                          //{WAHDAN}==> THESE FUNCTIONS WILL BE IMPLEMENTED BY [RAHAF-RAWAN,BASMALA1]
 void deleteEdge(Edge Target);                                          //{WAHDAN}==> A GRAPH IS COMPLETE IF EVERY NODE HAS EDGES TO EVERY OTHER NODE IN THE GRAPH
 bool isCompleteGraph();                                                //{WAHDAN}==> SEARCH ON HOW TO DO IT USING AN ALGORITHM

 std::vector<Node> DFS(Node& StartNode);
 std::vector<Node> BFS(Node& StartNode);
 static Graph& getInstance() {
     // Static local variable to ensure it's initialized only once
     static Graph instance;
     return instance;
 }
};
#endif // GRAPH_H
