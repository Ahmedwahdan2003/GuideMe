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
QTimer* animationTimer; // Timer for animation
unsigned int dfsIndex;
visualizeGraph::visualizeGraph(QWidget *parent) : QGraphicsView(parent), graph(nullptr)
{
    // Set up the QGraphicsView
    setScene(new QGraphicsScene(this));

    // setFixedSize(800, 600);
    // setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // setFixedSize(800, 600);
    // viewport()->setFixedSize(800, 600);

}

void visualizeGraph::setGraph(Graph* graph)   //{WAHDAN}==> Dependency injection
{
    this->graph = graph;
    if (!graph)
        return;

    // Clear the scene
    scene()->clear();
    NodesLeft = graph->getNodes().size();
    NodesDrawnidx=0;
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
    std::vector<Edge> edges = graph->getEdges(node);

    // Draw each edge with a slight offset
    qreal angleIncrement = (2 * M_PI) / edges.size();
    qreal startAngle = -M_PI / 2;
    qreal t = 0.1;
    int counter = 1;
    qreal spacing = 10;
    float edgeoffset = 1;
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
        edgeoffset+=1;
        // Calculate the angle between the two nodes
        qreal angle = startAngle;
        startAngle += angleIncrement;

        // Calculate the start and end positions for the line
        QPointF startPos = nodePos + QPointF(node.radius * qCos(angle), node.radius * qSin(angle));
        QPointF endPos = nodesPostitions[edge.destination.nodeName] + QPointF(node.radius * qCos(angle), node.radius * qSin(angle)); // Adjust end position to node perimeter

        // Draw straight line
        QGraphicsLineItem* lineItem = scene()->addLine(startPos.x(), startPos.y()+edgeoffset, endPos.x(), endPos.y());

        // Add the path to the scene
        QString tooltipText = QString::number(edge.option.getCost());
        lineItem->setToolTip(tooltipText);

        // Calculate position for text along the line
        if (counter % 3 == 0)
        {
            t += 0.2;
            edgeoffset=1;
        }
        QPointF textPos = startPos * (1 - t) + endPos * t;

        // Set edge transportation name on the line
        QGraphicsSimpleTextItem* textItem = scene()->addSimpleText(edge.option.getName());
        qreal nameOffsetX = spacing * qSin(angle); // Adjust this value for desired spacing
        qreal nameOffsetY = spacing * qCos(angle); // Adjust this value for desired spacing
        textItem->setPos(textPos.x() + nameOffsetX, textPos.y() + nameOffsetY);
        counter++;
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
// Slot for animating DFS
void visualizeGraph::drawArrowToPoint(const QPointF& targetPos)
{
    // Get the center position of the target node
    QPointF nodeCenterPos = targetPos;

    // Get the position of the current node (assuming it's the center of the scene)
    QPointF centerPos = nodeCenterPos+QPointF(60,60);

    // Calculate the vector from the center position to the target position
    QPointF vectorToTarget = nodeCenterPos - centerPos;

    // Calculate the length of the vector
    qreal vectorLength = sqrt(vectorToTarget.x() * vectorToTarget.x() + vectorToTarget.y() * vectorToTarget.y());

    // Calculate the normalized vector (unit vector) pointing towards the target
    QPointF normalizedVector(vectorToTarget.x() / vectorLength, vectorToTarget.y() / vectorLength);

    // Calculate the end position of the arrow (tip at the perimeter of the node)
    qreal nodeRadius = 20; // Assuming a fixed node radius, adjust as needed
    qreal endX = nodeCenterPos.x() - normalizedVector.x() * nodeRadius;
    qreal endY = nodeCenterPos.y() - normalizedVector.y() * nodeRadius;

    // Calculate the angle of the arrow
    qreal angle = atan2(nodeCenterPos.y() - endY, nodeCenterPos.x() - endX) * 180 / M_PI;

    // Create a polygon representing the arrowhead
    QPolygonF arrowhead;
    arrowhead << QPointF(0, 0) << QPointF(-10, 5) << QPointF(-10, -5);

    // Rotate the arrowhead polygon to match the angle of the arrow
    QTransform transform;
    transform.rotate(angle);
    arrowhead = transform.map(arrowhead);

    // Draw the arrowhead
    arrowheadItem = scene()->addPolygon(arrowhead);
    arrowheadItem->setBrush(Qt::black);
    arrowheadItem->setPos(endX, endY);

    // Draw the line segment from the center to the arrowhead
    lineItem = scene()->addLine(centerPos.x(), centerPos.y(), endX, endY);
    lineItem->setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap));
}




void visualizeGraph::startDFSAnimation()
{
    // Reset the animation timer
    if (!animationTimer) {
        animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &visualizeGraph::animateDFS);
    }

    // Set the DFS path and reset index
    dfsIndex = 0;
    std::vector<Node>dfsPath = graph->DFS(graph->getNodes()[0]);

    // Start the animation timer
    animationTimer->start(1000); // Adjust interval as needed
}

void visualizeGraph::animateDFS()
{
    std::vector<Node> dfsPath = graph->DFS(graph->getNodes()[0]);
    // Check if there are nodes left to draw
    if (dfsIndex < dfsPath.size()) {
        // Get the current node from the DFS path
        const Node& currentNode = dfsPath[dfsIndex];

        // Calculate the position of the current node dynamically
        QPointF nodePos = nodesPostitions[currentNode.nodeName];

        // Remove the previously drawn arrow and line, if they exist
        if (arrowheadItem) {
            scene()->removeItem(arrowheadItem);
            delete arrowheadItem;
            arrowheadItem = nullptr;
        }
        if (lineItem) {
            scene()->removeItem(lineItem);
            delete lineItem;
            lineItem = nullptr;
        }

        // Draw the arrow pointing at the current node
        drawArrowToPoint(nodePos);

        // Increment index for the next node
        ++dfsIndex;
    } else {
        // Animation finished, stop the timer
        if (arrowheadItem) {
            scene()->removeItem(arrowheadItem);
            delete arrowheadItem;
            arrowheadItem = nullptr;
        }
        if (lineItem) {
            scene()->removeItem(lineItem);
            delete lineItem;
            lineItem = nullptr;
        }
        animationTimer->stop();
    }
}


