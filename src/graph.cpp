#include "graph.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include<stack>
#include <unordered_set>
Graph::Graph():adjcencyList() {

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

        std::string source = parts[0].toStdString();
        std::string destination = parts[1].toStdString();
                                                            //alex cairo bus 20 metro 30
        Node src(source);
        Node dist(destination);

        std::vector<Transportation> weights;
        for (int i = 2; i < parts.size(); i+=2) {
            std::string name = parts[i].toStdString();
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
        qDebug() << "Node " << pair.first.getNodeName().c_str() << " connected to: ";
        for (const auto& edge : pair.second) {
            qDebug() << edge.getDestination().getNodeName().c_str() << " Weights: ";
            for (const Transportation& weight : edge.getOptions()) {
                qDebug() << weight.getName().c_str() << weight.getCost();
            }
            qDebug() << "| ";
        }
        qDebug() << "\n";
    }
}

std::vector<Node> Graph::DFS(Node& StartNode) {
    std::stack<Node> st;
    std::vector<Node> path;
    std::unordered_set<std::string> visitedNodes;
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

