// visualizegraph.cpp
#include "visualizegraph.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include<QApplication>
#include<QRandomGenerator>
unsigned int visualizeGraph::NodesLeft = 0; // Initialize static member
unsigned int visualizeGraph::NodesDrawnidx = 0; // Initialize static member
bool visualizeGraph::flag = false;
std::map<QString,QPointF>visualizeGraph::nodesPostitions =std::map<QString,QPointF>() ;
std::unordered_set<std::tuple<QString, QString, QString>,TupleHash> visualizeGraph::edgesDrawn;
size_t visualizeGraph::algosindex = 0;
std::vector<Node>visualizeGraph::dfspath = std::vector<Node>();
std::vector<Node>visualizeGraph::bfspath = std::vector<Node>();
bool visualizeGraph::isDragAllowed;
QString visualizeGraph::draggedNode;
QPointF visualizeGraph::dragOffset;
visualizeGraph::visualizeGraph(QWidget *parent) : QGraphicsView(parent),animationTimer(this), graph(nullptr)
{
    setScene(new QGraphicsScene(this));
    connect(&animationTimer, &QTimer::timeout, this, &visualizeGraph::animateDFS);
    connect(&animationTimertwo, &QTimer::timeout, this, &visualizeGraph::animateBFS);
    lineItem=nullptr;
    arrowheadItem=nullptr;
    isDragAllowed=false;
}

void visualizeGraph::setGraph(Graph* graph)   //{WAHDAN}==> Dependency injection
{
    this->graph = graph;
    if (!graph)
        return;
    scene()->clear();
    NodesLeft = graph->getNodes().size();
    NodesDrawnidx=0;
    QPixmap backgroundImage("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\map8");

    QGraphicsPixmapItem* backgroundItem = scene()->addPixmap(backgroundImage);


    backgroundItem->setPos(0, 0);


    backgroundItem->setZValue(-100);


    backgroundItem->setTransformationMode(Qt::SmoothTransformation);



}
void visualizeGraph::setBFSPath(const std::vector<Node>& path)
{
    bfspath = path;
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
    nodeNameItem->setDefaultTextColor(Qt::white); // Adjust text color
    nodeNameItem->setFont(QFont("Arial", 11)); // Adjust font properties
    nodeNameItem->setPos(nodesPostitions[node.nodeName].x() - nodeNameItem->boundingRect().width() / 2,
                         nodesPostitions[node.nodeName].y() - scaledPinPixmap.height() - 20); // Adjust position
    nodeNameItem->setZValue(1); // Ensure text appears above the pin
}



void visualizeGraph::drawEdge(const Node& node)
{
    QPointF nodePos = nodesPostitions[node.nodeName];
    std::vector<Edge> edges = graph->getEdges(node);

    // Map to store colors for each transportation
    std::unordered_map<QString, QColor> colorMap;

    // Iterate over edges to draw only one edge between the same two nodes
    for (const Edge& edge : edges) {
        // Check if the edge has already been drawn between the two nodes
        std::tuple<QString, QString, QString> edgeTuple(edge.parent.nodeName, edge.destination.nodeName, edge.option.getName());
        std::tuple<QString, QString, QString> edgeTuple2(edge.destination.nodeName, edge.parent.nodeName, edge.option.getName());
        if (edgesDrawn.find(edgeTuple) != edgesDrawn.end() || edgesDrawn.find(edgeTuple2) != edgesDrawn.end()) {
            continue; // Skip drawing this edge
        }
        edgesDrawn.insert(edgeTuple);
        edgesDrawn.insert(edgeTuple2);

        // Calculate destination node position
        QPointF destPos = nodesPostitions[edge.destination.nodeName];

        // Calculate midpoint of the edge for label placement
        QPointF midPoint = (nodePos + destPos) / 2.0;

        // Concatenate transportation names and costs into a single string
        QString labelText;
        for (const Edge& e : edges) {
            if (e.parent == edge.parent && e.destination == edge.destination) {
                if (!labelText.isEmpty()) {
                    labelText += ", ";
                }
                labelText += QString("%1 ($%2) ").arg(e.option.getName()).arg(e.option.getCost());

                // Assign color for each transportation if not already assigned
                if (colorMap.find(e.option.getName()) == colorMap.end()) {
                    // Generate a random color for the transportation
                    QColor color = QColor::fromRgb(QRandomGenerator::global()->bounded(256),
                                                   QRandomGenerator::global()->bounded(256),
                                                   QRandomGenerator::global()->bounded(256));
                    colorMap[e.option.getName()] = color;
                }
            }
        }

        // Draw the edge and label with respective colors
        for (const Edge& e : edges) {
            if (e.parent == edge.parent && e.destination == edge.destination) {
                QPen pen(colorMap[e.option.getName()]); // Set color based on transportation
                pen.setWidth(4);
                scene()->addLine(nodePos.x(), nodePos.y(), destPos.x(), destPos.y(), pen);
                QGraphicsTextItem* textItem = scene()->addText(labelText);
                textItem->setDefaultTextColor(Qt::white);
                textItem->setPos(midPoint);
                QFont font = textItem->font();
                font.setPointSize(11); // Set the desired font size
                textItem->setFont(font);


                break; // Break after drawing the edge with colors to avoid duplication
            }
        }
    }
}
void visualizeGraph::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if(isDragAllowed==false){
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
        isDragAllowed=true;
        edgesDrawn.clear();
    }
    }else{
        QPointF clickPos = mapToScene(event->pos());
        for (const auto& node : nodesPostitions) {
            // Calculate distance between click position and node position
            qreal dx = clickPos.x() - node.second.x();
            qreal dy = clickPos.y() - node.second.y();
            qreal distanceSquared = dx * dx + dy * dy;
            qreal radiusSquared = 50;

            // Check if the click is within the radius of the node
            if (distanceSquared <= radiusSquared) {
                // Store the clicked node and its offset from the click position
                draggedNode = node.first;
                dragOffset = QPointF(dx, dy);
                break;
            }
        }

    }
}

void visualizeGraph::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    // Check if a node is currently being dragged
    if (!draggedNode.isEmpty()) {
        // Calculate the new position of the dragged node
        QPointF newPos = mapToScene(event->pos()) - dragOffset;

        // Update the position of the dragged node in the scene


        // Update the position of the dragged node in the nodesPositions map
        QString nodeName = draggedNode;
        nodesPostitions[nodeName] = newPos;

        // Redraw the scene
        reDraw();
    }
}

void visualizeGraph::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    // Check if a node is currently being dragged
    if (!draggedNode.isEmpty()) {
        // Reset the draggedNode and dragOffset variables
        draggedNode = nullptr;
        dragOffset = QPointF();
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
            arrowheadItem->setBrush(Qt::white);
            arrowheadItem->setPos(endX, endY);

            // Draw the line segment from the center to the arrowhead
            lineItem = scene()->addLine(centerPos.x(), centerPos.y(), endX, endY);
            lineItem->setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::RoundCap));
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
void visualizeGraph::resizeEvent(QResizeEvent* event)
{
     QGraphicsView::resizeEvent(event);

    QPixmap backgroundImage("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\map8");

    QGraphicsPixmapItem* backgroundItem = scene()->addPixmap(backgroundImage);


    backgroundItem->setPos(0, 0);


    backgroundItem->setZValue(-100);


    backgroundItem->setTransformationMode(Qt::SmoothTransformation);
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
    QPixmap backgroundImage("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\map8"); // Adjust the path to your background image
    // Scale the background image to fit the size of the widget


    // Create a graphics item for the background
    QGraphicsPixmapItem* backgroundItem = scene()->addPixmap(backgroundImage);

    // Set the position of the background item to cover the whole scene
    backgroundItem->setPos(0, 0);

    // Ensure the background image is behind all other items
    backgroundItem->setZValue(-100); // Set a low Z-value

    // Optionally, set the aspect ratio mode to ensure the image is not distorted
    backgroundItem->setTransformationMode(Qt::SmoothTransformation);
}
