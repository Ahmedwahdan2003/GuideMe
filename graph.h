#ifndef GRAPH_H
#define GRAPH_H
#include<map>
#include<unordered_map>
#include<string>
using namespace std;
class Graph
{
    struct Transportation
    {
        string name;
        int cost;
        Transportation(string &s, int n):name(s),cost(n){}
    };
    struct Edge
    {
        string destination;
        vector<Transportation>options;
        Edge(string dest):destination(dest){}
        void addOption(string &name, int cost)
        {
            options.push_back(Transportation(name,cost));
        }
    };
public:
    class Node
    {
        string nodeName;
        vector<Edge>edges;
        Node(string Name){nodeName=Name;}
    public:
        void addEdge(string dest, string &optionName,int cost){
            edges.push_back(Edge(dest));
            edges.back().addOption(optionName,cost);
        }
    };
    map<string,Node>adjcencyList;
    Graph();
    void addNode(string nameofnode,Node newNode);
};
#endif // GRAPH_H
