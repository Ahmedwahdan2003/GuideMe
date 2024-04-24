#include "visualizegraph.h"
#include <QPainter>

visualizeGraph::visualizeGraph(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
    // Initialize any member variables if needed
}


void visualizeGraph::setGraph(Graph *mygraph)
{
    graph = mygraph;
    update(); // Trigger repaint
}

void visualizeGraph::visualizeDFS(std::vector<Node> &path)
{
    // Implement DFS visualization
    // Update rendering to highlight nodes and edges based on the traversal path
    update(); // Trigger repaint
}

void visualizeGraph::visualizeBFS( std::vector<Node> &path)
{
    // Implement BFS visualization
    // Update rendering to highlight nodes and edges based on the traversal path
    update(); // Trigger repaint
}

void visualizeGraph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Implement rendering logic to draw nodes, edges, and traversal animations
    // Use QPainter to draw shapes and lines based on the graph data

    // Example: Draw a simple circle for each node
    painter->setPen(Qt::black);
    painter->setBrush(Qt::blue);
    for (const Node &node : graph->getNodes()) {
        painter->drawEllipse(node.centerX - node.radius, node.centerY - node.radius, 2 * node.radius, 2 * node.radius);
    }

    // Example: Draw lines for each edge
    painter->setPen(Qt::black);
    for (const Node &node : graph->getNodes()) {
        for (const Edge &edge : graph->getEdges(node)) {
            painter->drawLine(edge.startX, edge.startY, edge.endX, edge.endY);
        }
    }
}
