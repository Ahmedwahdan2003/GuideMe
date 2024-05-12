#include "graph.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include<stack>
#include<queue>
#include <unordered_set>
#include <algorithm>

Graph::Graph(){

}
Node Graph::back(){
    auto lastElement = adjcencyList.cbegin();
    Node lastNode = lastElement->first;
    return lastNode;
}
bool Graph::empty(){
    return adjcencyList.empty();
}

void Graph::addNode(const Node& newNode) {
    // Check if the node already exists in the graph
    if (adjcencyList.find(newNode) == adjcencyList.end()) {
        // If not, add it to the adjacency list with an empty vector of edges
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
        const std::vector<Edge>& edges = adjacencyPair.second;

        // Append the edges to the 'allEdges' vector
        allEdges.insert(allEdges.end(), edges.begin(), edges.end());
    }

    return allEdges;
}

 std::vector<Node>& Graph::getNodesRef() const {
    static std::vector<Node> nodes;
    nodes.clear();
    for (const auto& keyValuePair : adjcencyList) {
        nodes.push_back(keyValuePair.first);
    }
    return nodes;
}
const std::vector<Edge>& Graph::getEdgesRef() const {
    static std::vector<Edge> edges;
    edges.clear();
    for (const auto& keyValuePair : adjcencyList) {
        edges.insert(edges.end(), keyValuePair.second.begin(), keyValuePair.second.end());
    }
    return edges;
}
bool Graph::isValidNode(const QString& nodeName) const {
    // Check if the node exists in the adjacency list
    return adjcencyList.find(nodeName) != adjcencyList.end();
}

bool Graph::isValidEdge(const QString& sourceNode, const QString& destinationNode, const QString& transportationName) const {
    auto it = adjcencyList.find(sourceNode);

    // Iterate through the edges of the source node to find the destination node and transportation
    const std::vector<Edge>& edges = it->second;
    for (const Edge& edge : edges) {
        if (edge.getDestination().getNodeName() == destinationNode && edge.getOption().getName() == transportationName) {
            return true; // Edge found
        }
    }

    // Edge not found
    qDebug() << "Edge with transportation" << transportationName << "from" << sourceNode << "to" << destinationNode << "does not exist.";
    return false;
}
void Graph::addEdge(Node From,Node dest,Transportation opt){

    addNode(From);
    addNode(dest);
    Edge newEdge = Edge(From,dest,opt);
    Edge newEdge2 = Edge(dest,From,opt);
    adjcencyList[From].emplace_back(newEdge);
    adjcencyList[dest].emplace_back(newEdge2);
}





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
    // Check if the graph is empty or has only one node
    if (adjcencyList.empty() || adjcencyList.size() == 1) {
        qDebug() << "The graph is not complete because it is empty or has only one node.";
        return false;
    }

    // Iterate through each node in the adjacency list
    for (const auto& pair1 : adjcencyList) {
        const Node& sourceNode = pair1.first;

        // store connected nodes
        std::unordered_set<QString> getConnectedNodes;

        // Check if the current node is connected to every other node
        for (const auto& pair2 : adjcencyList) {
            const Node& destNode = pair2.first;

            if (sourceNode == destNode) continue;

            bool connected = false;
            for (const auto& edge : adjcencyList.at(sourceNode)) {
                if (edge.getDestination() == destNode) {
                    connected = true;
                    break;
                }
            }
            if (!connected) {
                qDebug() << "The graph is not complete because node" << sourceNode.getNodeName() << "is not connected to node" << destNode.getNodeName() << ".";
                return false;
            }
        }
    }

    qDebug() << "The graph is complete.";
    return true;
}

 const std::unordered_set<QString> Graph::getConnectedNodes(const Node& node) const {
     const auto& edges = adjcencyList.at(node);
     std::unordered_set<QString> connectedNodes;
     for (const auto& edge : edges) {
         connectedNodes.insert(edge.getDestination().getNodeName());
     }
     return connectedNodes;
 }





void Graph::updateEdge(const Node& source, const Node& destination, const QString& oldTransportation, const QString& newTransportation, int newCost) {

    deleteEdge(source,destination,oldTransportation );
    addEdge(source, destination, Transportation(newTransportation, newCost));
}

void Graph::deleteEdge(const Node& source, const Node& destination, const QString& transportationName) {
    if (adjcencyList.find(source) == adjcencyList.end()) {
        qDebug() << "Source node does not exist in the graph.";
        return;
    }
    auto& edges = adjcencyList[source];
    auto it = std::find_if(edges.begin(), edges.end(), [&](const Edge& edge) {
        return edge.getDestination() == destination && edge.getOption().getName() == transportationName;
    });

    if (it != edges.end()) {
        // Remove the edge from the source node's adjacency list
        edges.erase(it);
        qDebug() << "Edge deleted between " << source.getNodeName() << " and " << destination.getNodeName()
                 << " with transportation: " << transportationName;

        // remove the edge from the destination node's adjacency list
        auto& destinationEdges = adjcencyList[destination];
        auto destIt = std::find_if(destinationEdges.begin(), destinationEdges.end(), [&](const Edge& edge) {
            return edge.getDestination() == source && edge.getOption().getName() == transportationName;
        });

        if (destIt != destinationEdges.end()) {
            destinationEdges.erase(destIt);
        }
        qDebug() << "Edge deleted between " << destination.getNodeName() << " and " << source.getNodeName()
                 << " with transportation: " << transportationName;
    } else {
        qDebug() << "Edge does not exist between " << source.getNodeName() << " and " << destination.getNodeName()
                 << " with transportation: " << transportationName;
    }
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
void Graph::writeGraphToFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing: " << fileName.toStdString() << "\n";
        return;
    }

    QTextStream out(&file);

    struct PairHash {
        std::size_t operator()(const std::pair<QString, QString>& pair) const {
            return qHash(pair.first) ^ qHash(pair.second);
        }
    };

    std::unordered_set<std::pair<QString, QString>, PairHash> visitedEdges;

    // Iterate through each node in the graph
    for (const auto& pair : adjcencyList) {
        const Node& sourceNode = pair.first;

        // Iterate through each edge of the source node
        for (const auto& edge : pair.second) {
            const Node& destNode = edge.getDestination();

            // Check if the edge or its reverse has been visited before
            if (visitedEdges.find(std::make_pair(sourceNode.getNodeName(), destNode.getNodeName())) == visitedEdges.end() &&
                visitedEdges.find(std::make_pair(destNode.getNodeName(), sourceNode.getNodeName())) == visitedEdges.end()) {

                out << sourceNode.getNodeName() << " " << destNode.getNodeName() << " 0 0 0 0 ";
                const std::vector<Edge>& edges = adjcencyList[destNode];

                for (const auto& connectedEdge : edges) {
                    if (connectedEdge.getDestination() == sourceNode) {
                        const Transportation& transportation = connectedEdge.getOption();
                        out << transportation.getName() << " " << transportation.getCost() << " ";
                        visitedEdges.insert(std::make_pair(sourceNode.getNodeName(), destNode.getNodeName()));
                    }
                }
                out << "\n";
            }
        }
    }
    file.close();
}


