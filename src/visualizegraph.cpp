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
size_t visualizeGraph::algosindex = 0;
std::vector<Node>visualizeGraph::dfspath = std::vector<Node>();
std::vector<Node>visualizeGraph::bfspath = std::vector<Node>();
visualizeGraph::visualizeGraph(QWidget *parent) : QGraphicsView(parent),animationTimer(this), graph(nullptr)
{
    setScene(new QGraphicsScene(this));
    connect(&animationTimer, &QTimer::timeout, this, &visualizeGraph::animateDFS);
    connect(&animationTimertwo, &QTimer::timeout, this, &visualizeGraph::animateBFS);
    lineItem=nullptr;
    arrowheadItem=nullptr;
}

void visualizeGraph::setGraph(Graph* graph)   //{WAHDAN}==> Dependency injection
{
    this->graph = graph;
    if (!graph)
        return;
    // dfspath = graph->DFS(graph->getNodes()[0]);
    // bfspath = graph->BFS(graph->getNodes()[0]);
    // Clear the scene
    scene()->clear();
    NodesLeft = graph->getNodes().size();
    NodesDrawnidx=0;
    QPixmap backgroundImage("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\map"); // Adjust the path to your background image

    // Scale the background image to fit the size of the widget
    // QPixmap scaledBackgroundImage = backgroundImage.scaled(1100,1150, Qt::KeepAspectRatio);

    // Create a graphics item for the background
    QGraphicsPixmapItem* backgroundItem = scene()->addPixmap(backgroundImage);

    // Set the position of the background item to cover the whole scene
    backgroundItem->setPos(0, 0);

    // Ensure the background image is behind all other items
    backgroundItem->setZValue(-100); // Set a low Z-value

    // Optionally, set the aspect ratio mode to ensure the image is not distorted
    backgroundItem->setTransformationMode(Qt::SmoothTransformation);

    // Create a pixmap item and set the background image

}
void visualizeGraph::setBFSPath(const std::vector<Node>& path)
{
    bfspath = path;
    qDebug()<<"hello kitty";
}

void visualizeGraph::drawNode(const Node& node)
{
    // Load a custom pin pixmap
    QPixmap pinPixmap("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\google-maps.png"); // Adjust the path to your pin image

    // Scale the pin pixmap to desired size
    int pinSize = 32; // Adjust the size as needed
    QPixmap scaledPinPixmap = pinPixmap.scaled(pinSize, pinSize, Qt::KeepAspectRatio);

    // Create a graphics item for the pin
    QGraphicsPixmapItem* pinItem = scene()->addPixmap(scaledPinPixmap);
    pinItem->setOffset(-scaledPinPixmap.width() / 2, -scaledPinPixmap.height()); // Adjust position to center the pin
    pinItem->setPos(nodesPostitions[node.nodeName]);

    // Set the node name as the tooltip
    pinItem->setToolTip(node.nodeName);

    // Create a text item for displaying the node name
    QGraphicsTextItem* nodeNameItem = scene()->addText(node.nodeName);
    nodeNameItem->setDefaultTextColor(Qt::black); // Adjust text color
    nodeNameItem->setFont(QFont("Arial", 10)); // Adjust font properties
    nodeNameItem->setPos(nodesPostitions[node.nodeName].x() - nodeNameItem->boundingRect().width() / 2,
                         nodesPostitions[node.nodeName].y() - scaledPinPixmap.height() - 20); // Adjust position
    nodeNameItem->setZValue(1); // Ensure text appears above the pin
}



void visualizeGraph::drawEdge(const Node& node)
{
    QPointF nodePos = nodesPostitions[node.nodeName];
    std::vector<Edge> edges = graph->getEdges(node);

    for (size_t i = 0; i < edges.size(); ++i) {
        const Edge& edge = edges[i];
        std::tuple<QString, QString, QString> edgeTuple(edge.parent.nodeName, edge.destination.nodeName, edge.option.getName());
        std::tuple<QString, QString, QString> edgeTuple2(edge.destination.nodeName, edge.parent.nodeName, edge.option.getName()); // Include transportation name // Include transportation name
        // Check if the edge has already been drawn between the two nodes
        if (edgesDrawn.find(edgeTuple) != edgesDrawn.end() || edgesDrawn.find(edgeTuple2) != edgesDrawn.end()) {
            continue; // Skip drawing this edge
        }

        // Calculate midpoint between nodes
        QPointF midPoint = (nodePos + nodesPostitions[edge.destination.nodeName]) / 2;

        // Offset the drawing position based on the index of the edge
        qreal offset = i * 30; // Adjust the offset as needed

        // Calculate control points for cubic Bezier curve
        QPointF controlPoint1(nodePos.x(), nodePos.y() + offset);
        QPointF controlPoint2(midPoint.x(), midPoint.y() + offset);

        // Draw curved line using cubic Bezier curve
        QPainterPath path;
        path.moveTo(nodePos);
        path.cubicTo(controlPoint1, controlPoint2, midPoint);
        path.lineTo(nodesPostitions[edge.destination.nodeName]); // Draw a straight line to the destination node

        // Draw the edge
        QPen pen(Qt::black);
        pen.setWidth(2);
        QGraphicsPathItem* pathItem = scene()->addPath(path, pen);
        QString tooltipText = QString::number(edge.option.getCost());
        pathItem->setToolTip(tooltipText);

        // Add the edge to the set of drawn edges
        edgesDrawn.insert(edgeTuple);
        edgesDrawn.insert(edgeTuple2);
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


        --NodesLeft;
        ++NodesDrawnidx;

    }

    // Check if all nodes have been drawn
    if (NodesLeft == 0) {

        for (const auto& node : nodes) {
            drawEdge(node);
        }
        edgesDrawn.clear();
    }
}


void visualizeGraph::updateNodeCounter()
{

}
void visualizeGraph::drawArrowToPoint(const QPointF& targetPos)
{
    try {
        // Get the center position of the target node
        QPointF nodeCenterPos = targetPos;

        // Get the position of the current node (assuming it's the center of the scene)
        QPointF centerPos = nodeCenterPos + QPointF(60, 60);

        // Calculate the vector from the center position to the target position
        QPointF vectorToTarget = nodeCenterPos - centerPos;

        // Calculate the length of the vector
        qreal vectorLength = sqrt(vectorToTarget.x() * vectorToTarget.x() + vectorToTarget.y() * vectorToTarget.y());

        // Check if the vector length is zero to avoid division by zero
        if (vectorLength != 0) {
            // Calculate the normalized vector (unit vector) pointing towards the target
            QPointF normalizedVector(vectorToTarget.x() / vectorLength, vectorToTarget.y() / vectorLength);

            // Calculate the end position of the arrow (tip at the perimeter of the node)
            qreal nodeRadius = 30;
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
        } else {
            qDebug() << "Error: Vector length is zero in drawArrowToPoint";
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in drawArrowToPoint: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in drawArrowToPoint";
    }
}


void visualizeGraph::startDFSAnimation()
{

    try{
        qDebug()<<"hello from start dfs animation";
        // Set the DFS path and reset index
        algosindex = 0;
        qDebug()<<dfspath[0].nodeName;
        qDebug()<<dfspath.size();
        // Start the animation timer
        animationTimer.start(1000); // Adjust interval as needed
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in drawArrowToPoint: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in drawArrowToPoint";
    }
}

void visualizeGraph::animateDFS()
{
    try{
        // qDebug()<<"hello from animateDfS";
        // Check if there are nodes left to draw
        if (algosindex < dfspath.size()) {
            // Get the current node from the DFS path
            const Node& currentNode = dfspath[algosindex];
            qDebug()<<algosindex<<"\n";
            // Calculate the position of the current node dynamically
            QPointF nodePos = nodesPostitions[currentNode.nodeName];
            qDebug()<<currentNode.nodeName<<" "<<nodePos.toPoint()<<"\n";
            // Remove the previously drawn arrow and line, if they exist
            if (arrowheadItem != nullptr) {
                scene()->removeItem(arrowheadItem);
                delete arrowheadItem;
                arrowheadItem = nullptr;
            }
            if (lineItem != nullptr) {
                scene()->removeItem(lineItem);
                delete lineItem;
                lineItem = nullptr;
            }

            // Draw the arrow pointing at the current node
            drawArrowToPoint(nodePos);

            // Increment index for the next node
            ++algosindex;
        } else {
            // Animation finished, stop the timer
            if (arrowheadItem!= nullptr) {
                scene()->removeItem(arrowheadItem);
                delete arrowheadItem;
                arrowheadItem = nullptr;
            }
            if (lineItem!= nullptr) {
                scene()->removeItem(lineItem);
                delete lineItem;
                lineItem = nullptr;
            }
            animationTimer.stop();
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in drawArrowToPoint: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in drawArrowToPoint";
    }
}
void visualizeGraph::startBFSAnimation(){
    try{
        // Clear any existing path and prepare for BFS animation
        NodesDrawnidx = 0;
        algosindex = 0;
        flag = true;


        // Start animation
        animationTimertwo.start(1000); // Adjust interval as needed
    }
    catch (const std::exception& e) {
        qDebug() << "Exception caught in drawArrowToPoint: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in drawArrowToPoint";
    }
}
void visualizeGraph::animateBFS(){
    try{
        // qDebug()<<"hello from animateDfS";
        // Check if there are nodes left to draw
        if (algosindex < bfspath.size()) {
            // Get the current node from the DFS path
            const Node& currentNode = bfspath[algosindex];
            qDebug()<<algosindex<<"\n";
            // Calculate the position of the current node dynamically
            QPointF nodePos = nodesPostitions[currentNode.nodeName];
            qDebug()<<currentNode.nodeName<<" "<<nodePos.toPoint()<<"\n";
            // Remove the previously drawn arrow and line, if they exist
            if (arrowheadItem != nullptr) {
                scene()->removeItem(arrowheadItem);
                delete arrowheadItem;
                arrowheadItem = nullptr;
            }
            if (lineItem != nullptr) {
                scene()->removeItem(lineItem);
                delete lineItem;
                lineItem = nullptr;
            }

            // Draw the arrow pointing at the current node
            drawArrowToPoint(nodePos);

            // Increment index for the next node
            ++algosindex;
        } else {
            // Animation finished, stop the timer
            if (arrowheadItem!= nullptr) {
                scene()->removeItem(arrowheadItem);
                delete arrowheadItem;
                arrowheadItem = nullptr;
            }
            if (lineItem!= nullptr) {
                scene()->removeItem(lineItem);
                delete lineItem;
                lineItem = nullptr;
            }
            animationTimertwo.stop();
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in drawArrowToPoint: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in drawArrowToPoint";
    }
}
void visualizeGraph::reDraw() {
    scene()->clear();
    // Get all nodes from the graph
    std::vector<Node> nodes = graph->getNodes();

    // Iterate through each node
    for (const auto& node : nodes)
    {
        drawNode(node);
        drawEdge(node);
    }
    edgesDrawn.clear();
    QPixmap backgroundImage("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\map"); // Adjust the path to your background image
    // Scale the background image to fit the size of the widget
    //QPixmap scaledBackgroundImage = backgroundImage.scaled(viewport()->size(), Qt::KeepAspectRatio);

    // Create a graphics item for the background
    QGraphicsPixmapItem* backgroundItem = scene()->addPixmap(backgroundImage);

    // Set the position of the background item to cover the whole scene
    backgroundItem->setPos(0, 0);

    // Ensure the background image is behind all other items
    backgroundItem->setZValue(-100); // Set a low Z-value

    // Optionally, set the aspect ratio mode to ensure the image is not distorted
    backgroundItem->setTransformationMode(Qt::SmoothTransformation);
}
