// visualizegraph.cpp
#include "visualizegraph.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

visualizeGraph::visualizeGraph(QWidget *parent) : QGraphicsView(parent), graph(nullptr)
{
    // Set the scene
    setScene(new QGraphicsScene(this));
}

void visualizeGraph::setGraph(Graph* graph)
{
    this->graph = graph;
    if (!graph)
        return;

    // Clear the scene
    scene()->clear();

    // Get nodes and edges from the graph
    std::vector<Node> nodes = graph->getNodes();
    std::vector<Edge> edges = graph->getEdges();

    // Draw nodes
    for (const auto& node : nodes) {
        QGraphicsEllipseItem* nodeItem = scene()->addEllipse(
            node.centerX - node.radius, node.centerY - node.radius,
            2 * node.radius, 2 * node.radius
            );
        nodeItem->setBrush(Qt::red);
        // Set node name as tooltip
        // nodeItem->setPlainText(node.nodeName);
    }

   // Draw edges
    for (const auto& edge : edges) {
        // Get coordinates of start and end nodes
        float startX = edge.parent.centerX;
        float startY = edge.parent.centerY;
        float endX = edge.destination.centerX;
        float endY = edge.destination.centerY;

        // Draw edge line
        QGraphicsLineItem* edgeItem = scene()->addLine(
            startX, startY, endX, endY
            );
        // Set edge options as tooltip
        edgeItem->setToolTip(edge.option.getName() + ": " + QString::number(edge.option.getCost()));
    }
}
