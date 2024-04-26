// visualizegraph.cpp
#include "visualizegraph.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include<QApplication>
unsigned int visualizeGraph::NodesLeft = 0; // Initialize static member
unsigned int visualizeGraph::NodesDrawnidx = 0; // Initialize static member
bool visualizeGraph::flag = false;
std::map<QString,QPointF>visualizeGraph::nodesPostitions =std::map<QString,QPointF>() ;
std::unordered_set<std::tuple<QString, QString, QString>,TupleHash> visualizeGraph::edgesDrawn;
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
    // for (const auto& edge : graph->getEdges()) {
    //     edgesDrawn.insert({edge.parent.getNodeName(), edge.destination.getNodeName()});
    // }
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

void visualizeGraph::drawEdge(const Node& node)
{
    // Get all edges connected to the node

    QPointF nodePos = nodesPostitions[node.nodeName];

    // Get all edges connected to the node
    std::vector<Edge> edges = graph->getEdges(node);

    // Calculate the angle increment between edges
    qreal angleIncrement = (2 * M_PI) / edges.size();

    // Start angle for the first edge
    qreal startAngle = -M_PI / 2;

    // Draw each edge with a slight offset
        for (const auto& edge : edges) {
            std::tuple<QString, QString, QString> edgeTuple(edge.parent.nodeName, edge.destination.nodeName, edge.option.getName());
            std::tuple<QString, QString, QString> edgeTuple2(edge.destination.nodeName, edge.parent.nodeName, edge.option.getName());            // Include transportation name
            if (edgesDrawn.find(edgeTuple) != edgesDrawn.end()) {
                continue; // Skip drawing this edge
            }
            edgesDrawn.insert(edgeTuple);
            if (edgesDrawn.find(edgeTuple2) != edgesDrawn.end()) {
                continue; // Skip drawing this edge
            }
            edgesDrawn.insert(edgeTuple2);

        // Find the index of the current edge within the vector of edges
        auto it = std::find(edges.begin(), edges.end(), edge);
        if (it == edges.end()) {
            // Edge not found, skip drawing
            continue;
        }
        int index = std::distance(edges.begin(), it);

        // Calculate the angle between the two nodes
        qreal angle = startAngle + (angleIncrement * index);

        // Calculate the perpendicular offset for each edge
        qreal offsetX = 20 * qSin(angle); // 20 can be adjusted for the desired spacing
        qreal offsetY = 20 * qCos(angle); // 20 can be adjusted for the desired spacing

        // Get coordinates of start and end nodes using updated positions
        QPointF startPos = nodePos + QPointF(offsetX, offsetY);
        QPointF endPos = nodesPostitions[edge.destination.nodeName];

        // Draw edge line with adjusted positions
        QGraphicsLineItem* edgeItem = scene()->addLine(
            startPos.x(), startPos.y(),
            endPos.x(), endPos.y()
            );
        // Set edge options as tooltip
        QString tooltipText = edge.option.getName() + " (" + QString::number(edge.option.getCost()) + ")";
        edgeItem->setToolTip(tooltipText);
        QPointF textPos = (startPos + endPos) / 2;
        // Calculate the angle of the edge
        // Rotate the text to align with the edge
        QTransform transform;
        transform.rotateRadians(angle);
        QFont font = QApplication::font(); // Use default font
        QFontMetrics fm(font);
        int textWidth = fm.horizontalAdvance(tooltipText); // Calculate width of the text
        int textHeight = fm.height(); // Calculate height of the text
        QPointF rotatedTextPos = transform.map(QPointF(-textWidth / 2, textHeight)); // Rotate the text position

        // Draw the text on the scene
        QGraphicsSimpleTextItem* textItem = scene()->addSimpleText(tooltipText);
        textItem->setPos(textPos - rotatedTextPos); // Set the position of the text
        textItem->setTransform(transform); // Apply the rotation transform to the text

}
}



void visualizeGraph::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

     std::vector<Node> nodes = graph->getNodes(); // Get nodes by reference
    std::vector<Edge> edges = graph->getEdges(); // Get edges by const reference

    if (NodesLeft > 0) {
        // Get the position where the user clicked
        QPointF clickPos = mapToScene(event->pos());

        // Create a new node at the position of the click
        Node newNode = nodes[NodesDrawnidx]; // Get a reference to the node
        newNode.centerX = clickPos.x();
        newNode.centerY = clickPos.y();
        nodesPostitions[newNode.nodeName] = clickPos;
        drawNode(newNode);
        qDebug() << "NodesDrawnidx : " << NodesDrawnidx << "\n";

        // Update the node counter
        --NodesLeft;
        ++NodesDrawnidx;
        // updateNodeCounter();
    }

    // Check if all nodes have been drawn
    if (NodesLeft == 0) {

        // Draw the edges using the updated node positions
        for (const auto& node : nodes) {
            drawEdge(node);
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
