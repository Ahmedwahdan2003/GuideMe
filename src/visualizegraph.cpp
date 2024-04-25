// visualizegraph.cpp
#include "visualizegraph.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
unsigned int visualizeGraph::NodesLeft = 0; // Initialize static member
unsigned int visualizeGraph::NodesDrawnidx = 0; // Initialize static member
bool visualizeGraph::flag = false;
visualizeGraph::visualizeGraph(QWidget *parent) : QGraphicsView(parent), graph(nullptr)
{
    // Set up the QGraphicsView
    setScene(new QGraphicsScene(this));
    // setFixedSize(800, 600);
    // setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // setFixedSize(800, 600);
    // viewport()->setFixedSize(800, 600);

}

void visualizeGraph::setGraph(Graph* graph)
{
    this->graph = graph;
    if (!graph)
        return;

    // Clear the scene
    scene()->clear();
    NodesLeft = graph->getNodes().size();
    NodesDrawnidx=0;
     // QGraphicsTextItem* counterText = scene()->addText(QString("Nodes Left: %1").arg(NodesLeft));
     // counterText->setPos(0, 0); // Adds 10 pixels padding from top and left edges

    // Get nodes and edges from the graph
    // std::vector<Node> nodes = graph->getNodes();
    // std::vector<Edge> edges = graph->getEdges();

    // // Draw nodes
    // for (const auto& node : nodes) {
    //     // Draw the ellipse representing the node
    //     QGraphicsEllipseItem* nodeItem = scene()->addEllipse(
    //         node.centerX - node.radius, node.centerY - node.radius,
    //         2 * node.radius, 2 * node.radius
    //         );
    //     nodeItem->setBrush(Qt::red);

    //     // Set the node name as the tooltip
    //     nodeItem->setToolTip(node.nodeName);

    //     // Create a text item for displaying the node name
    //     QGraphicsTextItem* nodeNameItem = scene()->addText(node.nodeName);
    //     nodeNameItem->setPos(node.centerX - nodeNameItem->boundingRect().width() / 2,
    //                          node.centerY - nodeNameItem->boundingRect().height() / 2);

   //  }


   // // Draw edges
   //  for (const auto& edge : edges) {
   //      // Get coordinates of start and end nodes
   //      float startX = edge.parent.centerX;
   //      float startY = edge.parent.centerY;
   //      float endX = edge.destination.centerX;
   //      float endY = edge.destination.centerY;

   //      // Draw edge line
   //      QGraphicsLineItem* edgeItem = scene()->addLine(
   //          startX, startY, endX, endY
   //          );

   //      // Set edge options as tooltip
   //      edgeItem->setToolTip(edge.option.getName());

   //      // Calculate the position to draw the cost
   //      QPointF textPos((startX + endX) / 2, (startY + endY) / 2);

   //      // Draw the cost on the edge
   //      QGraphicsSimpleTextItem* costText = scene()->addSimpleText(
   //          QString::number(edge.option.getCost())
   //          );
   //      costText->setPos(textPos);
   //  }
}

void visualizeGraph::drawNode(const Node& node)
{
    // Draw the ellipse representing the node
    QGraphicsEllipseItem* nodeItem = scene()->addEllipse(
        node.centerX - node.radius, node.centerY - node.radius,
        2 * node.radius, 2 * node.radius
        );
    nodeItem->setBrush(Qt::red);

    // Set the node name as the tooltip
    nodeItem->setToolTip(node.nodeName);

    // Create a text item for displaying the node name
    QGraphicsTextItem* nodeNameItem = scene()->addText(node.nodeName);
    nodeNameItem->setPos(node.centerX - nodeNameItem->boundingRect().width() / 2,
                         node.centerY - nodeNameItem->boundingRect().height() / 2);
}

void visualizeGraph::drawEdge(const Edge& edge, const std::map<QString, QPointF>& nodePositions)
{
    // Get coordinates of start and end nodes using updated positions
    QPointF startPos = nodePositions.at(edge.parent.nodeName);
    QPointF endPos = nodePositions.at(edge.destination.nodeName);

    // Draw edge line
    QGraphicsLineItem* edgeItem = scene()->addLine(
        startPos.x(), startPos.y(), endPos.x(), endPos.y()
        );

    // Set edge options as tooltip
    edgeItem->setToolTip(edge.option.getName());

    // Calculate the position to draw the cost
    QPointF textPos((startPos.x() + endPos.x()) / 2, (startPos.y() + endPos.y()) / 2);

    // Draw the cost on the edge
    QGraphicsSimpleTextItem* costText = scene()->addSimpleText(
        QString::number(edge.option.getCost())
        );
    costText->setPos(textPos);
}

void visualizeGraph::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    std::map<QString, QPointF> nodePositions;
    std::vector<Node>& nodes = graph->getNodesRef(); // Get nodes by reference
    const std::vector<Edge>& edges = graph->getEdgesRef(); // Get edges by const reference

    if (NodesLeft > 0) {
        // Get the position where the user clicked
        QPointF clickPos = mapToScene(event->pos());

        // Create a new node at the position of the click
        Node& newNode = nodes[NodesDrawnidx]; // Get a reference to the node
        nodes[NodesDrawnidx].centerX=clickPos.x();
        nodes[NodesDrawnidx].centerY=clickPos.y();
       nodePositions[nodes[NodesDrawnidx].nodeName] = QPointF(clickPos.x(),clickPos.y());
        newNode.centerX = clickPos.x();
        newNode.centerY = clickPos.y();
        drawNode(newNode);
        qDebug() << "NodesDrawnidx : " << NodesDrawnidx << "\n";

        // Update the node counter
        --NodesLeft;
        ++NodesDrawnidx;
        // updateNodeCounter();
    }

    // Check if all nodes have been drawn
    if (NodesLeft == 0) {
        // Store the updated node positions
        for (const auto& node : nodes) {

            qDebug() << node.getNodeName() << ": " << node.centerX << " " << node.centerY << "\n";
        }

        // Draw the edges using the updated node positions
        for (const auto& edge : edges) {
            drawEdge(edge, nodePositions);
        }
    }
}


void visualizeGraph::updateNodeCounter()
{
    // QList<QGraphicsItem*> items = scene()->items();
    // for (QGraphicsItem* item : items) {
    //     QGraphicsTextItem* textItem = qgraphicsitem_cast<QGraphicsTextItem*>(item);
    //     if (textItem && textItem->toPlainText().startsWith("Nodes Left:")) {
    //         // Update the text value of the counterText item
    //         textItem->setPlainText(QString("Nodes Left: %1").arg(NodesLeft));
    //         textItem->setPos(viewport()->width() / 2 - textItem->boundingRect().width() / 2, 100);
    //         return; // Exit the function once the item is found and updated
    //     }
    // }
}
