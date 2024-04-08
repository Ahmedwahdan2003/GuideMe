#include "graph.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
Graph::Graph():adjcencyList() {

}

void Graph::addNode(const Node newNode)
{
    if(adjcencyList.find(newNode)==adjcencyList.end()){
        adjcencyList[newNode] = std::vector<Edge>();
    }
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

        std::string node1Name = parts[0].toStdString();
        std::string node2Name = parts[1].toStdString();
                                                            //alex cairo bus 20 metro 30
        Node node1(node1Name);
        Node node2(node2Name);

        std::vector<Transportation> weights;
        for (int i = 2; i < parts.size(); ++i) {
            std::string name = parts[i].toStdString();
            int cost = parts[i+1].toInt();
            weights.emplace_back(Transportation(name,cost));
        }

        addEdge(node1, node2, weights);
    }

    file.close();
    return true;
}
void Graph::printGraph() {
    for (const auto& pair : adjcencyList) {
        qDebug() << "Node " << pair.first.nodeName.c_str() << " connected to: ";
        for (const auto& edge : pair.second) {
            qDebug() << edge.destination.nodeName.c_str() << " Weights: ";
            for (const Transportation& weight : edge.options) {
                qDebug() << weight.name.c_str() << weight.cost;
            }
            qDebug() << "| ";
        }
        qDebug() << "\n";
    }
}
