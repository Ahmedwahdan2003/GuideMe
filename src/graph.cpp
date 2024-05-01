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

 std::vector<Node>& Graph::getNodesRef() const {
    static std::vector<Node> nodes; // Static to ensure it persists
    nodes.clear(); // Clear the vector to ensure it's up-to-date
    for (const auto& keyValuePair : adjcencyList) {
        nodes.push_back(keyValuePair.first);
    }
    return nodes;
}
const std::vector<Edge>& Graph::getEdgesRef() const {
    static std::vector<Edge> edges; // Static to ensure it persists
    edges.clear(); // Clear the vector to ensure it's up-to-date
    for (const auto& keyValuePair : adjcencyList) {
        edges.insert(edges.end(), keyValuePair.second.begin(), keyValuePair.second.end());
    }
    return edges;
}
void Graph::addEdge(Node From,Node dest,Transportation opt){
    addNode(From);
    addNode(dest);
    Edge newEdge = Edge(From,dest,opt);
    Edge newEdge2 = Edge(dest,From,opt);
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

        if (parts.size() < 3) {
            qDebug() << "Invalid line: " << line.toStdString() << "\n";
            continue;
        }

        QString source = parts[0];
        QString destination = parts[1];
        float x1 = parts[2].toFloat();
        float y1 = parts[3].toFloat();
        float x2 = parts[4].toFloat();
        float y2 = parts[5].toFloat();
        Node src(source,x1,y1);
        Node dist(destination,x2,y2);

        for(int i=6;i<parts.size()-1;i+=2){
        QString transportationName = parts[i];
        int transportationCost = parts[i+1].toInt();
        Transportation transportation(transportationName, transportationCost);
        addEdge(src, dist, transportation);
        }
    }

    file.close();
    return true;
}


// bool Graph::readGraphFile(const QString& fileName)
// {
//     QFile file(fileName);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qDebug() << "Failed to open file: " << fileName.toStdString() << "\n";
//         return false;
//     }

//     QTextStream in(&file);
//     while (!in.atEnd()) {
//         QString line = in.readLine().trimmed();
//         QStringList parts = line.split(' ');

//         if (parts.size() < 2) {
//             qDebug() << "Invalid line: " << line.toStdString() << "\n";
//             continue;
//         }

//         QString source = parts[0];
//         QString destination = parts[1];
//                                                             //alex cairo bus 20 metro 30
//         Node src(source);
//         Node dist(destination);

//         std::vector<Transportation> weights;
//         for (int i = 2; i < parts.size(); i+=2) {
//             QString name = parts[i];
//             int cost = parts[i+1].toInt();
//             weights.emplace_back(Transportation(name,cost));
//         }

//         addEdge(source, destination, weights);
//     }

//     file.close();
//     return true;
// }
void Graph::printGraph() {
    for (const auto& pair : adjcencyList) {
        qDebug() << "Node " << pair.first.getNodeName() << " connected to: ";
        for (const auto& edge : pair.second) {
            qDebug() << edge.getDestination().getNodeName() << " Weight: "
                     << edge.getOption().getName() << " " << edge.getOption().getCost();
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
bool Graph::isCompleteGraph() {
    // Get all nodes in the graph
    std::vector<Node> nodes = getNodes();

    // Iterate through each pair of nodes
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            // Check if there is an edge between node i and node j
            bool foundEdge = false;
            std::vector<Edge> edges = getEdges(nodes[i]);
            for (const Edge& edge : edges) {
                if (edge.getDestination() == nodes[j]) {
                    foundEdge = true;
                    break;
                }
            }
            // If there is no edge between node i and node j, the graph is not complete
            if (!foundEdge) {
                return false;
            }
        }
    }
    // If no missing edges were found between any pair of nodes, the graph is complete
    return true;
}
