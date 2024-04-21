#include "graph.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include<stack>
#include<queue>
#include <unordered_set>
Graph::Graph(){

}
Node Graph::back(){
    auto lastElement = adjcencyList.rbegin();
    Node lastNode = lastElement->first;
    return lastNode;
}
bool Graph::empty(){
    return adjcencyList.empty();
}
void Graph::addNode(const Node newNode)
{
    if(adjcencyList.find(newNode)==adjcencyList.end()){
        adjcencyList[newNode] = std::vector<Edge>();
    }
}

std::vector<Edge> Graph::getEdges(Node Source)
{
    if(adjcencyList.find(Source)!=adjcencyList.end())
    {
    return adjcencyList[Source];

    }else
    {
        return std::vector<Edge>();
    }

}

std::vector<Node> Graph::getNodes()
{
    std::vector<Node>result;
    for (const auto& Key : adjcencyList) {
        result.push_back(Key.first);
    }
    return result;
}



void Graph::addEdge(Node From,Node dest,std::vector<Transportation>opts){
    addNode(From);
    addNode(dest);
    Edge newEdge = Edge(From,dest,opts);
    Edge newEdge2 = Edge(dest,From,opts);
    adjcencyList[From].emplace_back(newEdge);
    adjcencyList[dest].emplace_back(newEdge2);
}


bool Graph::readGraphFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file: " << fileName.toStdString() << "\n";
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(' ');

        if (parts.size() < 2) {
            qDebug() << "Invalid line: " << line.toStdString() << "\n";
            continue;
        }

        QString source = parts[0];
        QString destination = parts[1];
                                                            //alex cairo bus 20 metro 30
        Node src(source);
        Node dist(destination);

        std::vector<Transportation> weights;
        for (int i = 2; i < parts.size(); i+=2) {
            QString name = parts[i];
            int cost = parts[i+1].toInt();
            weights.emplace_back(Transportation(name,cost));
        }

        addEdge(source, destination, weights);
    }

    file.close();
    return true;
}
void Graph::printGraph() {
    for ( auto& pair : adjcencyList) {
        qDebug() << "Node " << pair.first.getNodeName() << " connected to: ";
        for (const auto& edge : pair.second) {
            qDebug() << edge.getDestination().getNodeName() << " Weights: ";
            for (const Transportation& weight : edge.getOptions()) {
                qDebug() << weight.getName() << weight.getCost();
            }
            qDebug() << "| ";
        }
        qDebug() << "\n";
    }
}

std::vector<Node> Graph::DFS(Node& StartNode) {
    std::stack<Node> st;
    std::vector<Node> path;
    std::unordered_set<QString> visitedNodes;
    st.push(StartNode);

    while (!st.empty()) {
        Node node = st.top();
        st.pop();

            //{WAHDAN}==> Check if the node has already been visited
        if (visitedNodes.find(node.getNodeName()) != visitedNodes.end()) {
            continue; // Skip this node
        }

        //{WAHDAN}==> Mark the node as visited
        visitedNodes.insert(node.getNodeName());

        //{WAHDAN}==> Add the node to the path
        path.push_back(node);

        //{WAHDAN}==> Get the edges connected to the current node
        std::vector<Edge> edges = getEdges(node);


        for (const Edge& edge : edges) {
            Node destNode = edge.getDestination();

            if (visitedNodes.find(destNode.getNodeName()) == visitedNodes.end()) {      //{WAHDAN}==>check if one the neighbors is already visited before or no
                st.push(destNode);
            }
        }
    }

    return path;
}
std::vector<Node> Graph::BFS(Node& StartNode)
{
    std::queue<Node> q;
    std::unordered_set<QString> visitedNodes;
    std::vector<Node> path;
    q.push(StartNode);


    while(!q.empty())
    {
        Node node = q.front();
        q.pop();


            // {Omar Khales} -> check if node is already visited


        if (visitedNodes.find(node.getNodeName()) != visitedNodes.end()) {
            continue; // Skip this node
        }


        // {Omar Khales} -> mark node as visited
        visitedNodes.insert(node.getNodeName());

        // {Omar Khales} -> adding node to the path
        path.push_back(node);


        // {Omar Khales} -> getting neighbours of node using its edges

        std::vector<Edge>edges = getEdges(node);

        // {Omar Khales} -> iterating over edges of the node

        for(const Edge& edge :edges){
            Node destNode = edge.getDestination();

            if(visitedNodes.find(destNode.getNodeName())==visitedNodes.end())
                q.push(destNode); // {Omar Khales} -> adding unvisited neighbours
        }
    }

    return path;
}
std::vector<Edge> Graph::getEdges(){

    std::vector<Edge> allEdges;

    // Iterate through each node in the adjacency list
    for (const auto& adjacencyPair : adjcencyList) {
        // Retrieve the vector of edges associated with the current node
        const std::vector<Edge>& edges = adjacencyPair.second;

        // Append the edges to the 'allEdges' vector
        allEdges.insert(allEdges.end(), edges.begin(), edges.end());
    }

    return allEdges;
}
