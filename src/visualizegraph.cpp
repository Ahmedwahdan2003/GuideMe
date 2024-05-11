// visualizegraph.cpp
#include "visualizegraph.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include<QApplication>
#include<QScrollBar>
#include<QRandomGenerator>
QMap<QString, QString> visualizeGraph::nodeImagePaths = {
    {"earth",  ":/resources/earth2"},
    {"mercury",":/resources/mercury.png"},
    {"mars",   ":/resources/mars"},
    {"jubiter",":/resources/jupiter.png"},
    {"uranus", ":/resources/uranus.png"},
    {"saturn", ":/resources/saturn"},
    };
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
    directedArrowItem=nullptr;
    isDragAllowed=false;
    //nodeimg = QPixmap("C:\\Users\\RAWAN\\Desktop\\GuideMe\\GuideMe\\src\\earth2").scaled(128,128, Qt::KeepAspectRatio);
    backgroundImage = QPixmap(":/resources/space_back2");
}

void visualizeGraph::setGraph(Graph* graph)   //{WAHDAN}==> Dependency injection
{
    this->graph = graph;
    if (!graph)
        return;
    scene()->clear();
    NodesLeft = graph->getNodes().size();
    NodesDrawnidx=0;

    QGraphicsPixmapItem* backgroundItem = scene()->addPixmap(backgroundImage);


    backgroundItem->setPos(0, 0);


    backgroundItem->setZValue(-100);


    backgroundItem->setTransformationMode(Qt::SmoothTransformation);



}
void visualizeGraph::setBFSPath(const std::vector<Node>& path)
{
    bfspath = path;
}

void visualizeGraph::drawNode(const Node& node, int choice)
{
    QPixmap nodePixmap;
    // Check if the node name exists in the nodeImagePaths mapping
    if (nodeImagePaths.contains(node.nodeName)) {
        // Load the image for the node using the specified image path
         nodePixmap = QPixmap(nodeImagePaths[node.nodeName]);
    }else{
        nodePixmap = QPixmap(":/resources/earth2");
    }

        // Scale the pixmap to the desired size
        QSize scaledSize(128, 128); // Adjust the size as needed
        QPixmap scaledPixmap = nodePixmap.scaled(scaledSize, Qt::KeepAspectRatio);

        // Create a graphics item for the node
        QGraphicsPixmapItem* nodeItem = scene()->addPixmap(scaledPixmap);
        nodeItem->setOffset(-scaledPixmap.width() / 2, -scaledPixmap.height());
        nodeItem->setPos(nodesPostitions[node.nodeName]);

        // Set the node name as the tooltip
        nodeItem->setToolTip(node.nodeName);

        // Create a text item for displaying the node name
        QGraphicsTextItem* nodeNameItem = scene()->addText(node.nodeName);
        nodeNameItem->setDefaultTextColor(Qt::white);
        nodeNameItem->setFont(QFont("Arial", 11));
        nodeNameItem->setPos(nodesPostitions[node.nodeName].x() - nodeNameItem->boundingRect().width() / 2,
                             nodesPostitions[node.nodeName].y() - scaledPixmap.height() - 20);
        nodeNameItem->setZValue(1);
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
                QPen pen(Qt::white); // Set color based on transportation
                pen.setWidth(4);
                scene()->addLine(nodePos.x(), nodePos.y(), destPos.x(), destPos.y(), pen);
                QGraphicsTextItem* textItem = scene()->addText(labelText);
                textItem->setDefaultTextColor(colorMap[e.option.getName()]);
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
        drawNode(newNode,1);
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
            qreal radiusSquared = 100;

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
        // Load the image for the directed arrow
        QPixmap directedArrowPixmap(":/resources/space_ship2"); // Adjust the path to your directed arrow image
        int pinSize = 80; // Adjust the size as needed
        QPixmap scaleddirectedArrowPixmap = directedArrowPixmap.scaled(pinSize, pinSize, Qt::KeepAspectRatio);
        // Create a QGraphicsPixmapItem for the directed arrow image
        directedArrowItem = scene()->addPixmap(scaleddirectedArrowPixmap);

        // Set the position of the directed arrow item to the target position
        directedArrowItem->setPos(targetPos+ QPointF(80, -80));

        // Get the center position of the target node
        QPointF nodeCenterPos = targetPos;

        // Get the position of the current node (assuming it's the center of the scene)
        QPointF centerPos = nodeCenterPos + QPointF(60, 60);

        // Calculate the vector from the center position to the target position
        QPointF vectorToTarget = nodeCenterPos - centerPos;

        // Calculate the angle of the directed arrow
        qreal angle = atan2(vectorToTarget.y(), vectorToTarget.x()) * 180 / M_PI;

        // Set the rotation of the directed arrow item to match the angle
        directedArrowItem->setRotation(angle-50);
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in drawDirectedImageToPoint: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in drawDirectedImageToPoint";
    }
}


void visualizeGraph::startDFSAnimation()
{

    try {
        // qDebug() << "Starting DFS animation";

        // Set the DFS path and reset index
        algosindex = 0;

        // Start the animation timer
        animationTimer.start(800); // Adjust interval as needed

        // Draw the directed image for the first node
        if (algosindex < dfspath.size()) {
            const Node& currentNode = dfspath[algosindex];
            QPointF nodePos = nodesPostitions[currentNode.nodeName];
            drawArrowToPoint(nodePos);

        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in startDFSAnimation: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in startDFSAnimation";
    }
}

void visualizeGraph::animateDFS()
{
    try {
        // qDebug() << "Animating DFS";

        // Check if there are nodes left to draw
        if (algosindex < dfspath.size()) {
            // Get the current node from the DFS path
            const Node& currentNode = dfspath[algosindex];

            // Calculate the position of the current node dynamically
            QPointF nodePos = nodesPostitions[currentNode.nodeName];

            // Remove the previously drawn directed image, if it exists
            if (directedArrowItem != nullptr) {
                scene()->removeItem(directedArrowItem);
                delete directedArrowItem;
                directedArrowItem = nullptr;
            }

            // Draw the directed image pointing at the current node
            drawArrowToPoint(nodePos);
            // Increment index for the next node
            ++algosindex;
        } else {
            // Animation finished, stop the timer
            animationTimer.stop();

            // Remove the directed image from the scene
            if (directedArrowItem != nullptr) {
                scene()->removeItem(directedArrowItem);
                delete directedArrowItem;
                directedArrowItem = nullptr;
            }

        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in animateDFS: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in animateDFS";
    }
}
void visualizeGraph::startBFSAnimation(){
    try {
        // qDebug() << "Starting DFS animation";

        // Set the DFS path and reset index
        algosindex = 0;

        // Start the animation timer
        animationTimertwo.start(800); // Adjust interval as needed

        // Draw the directed image for the first node
        if (algosindex < bfspath.size()) {
            const Node& currentNode = bfspath[algosindex];
            QPointF nodePos = nodesPostitions[currentNode.nodeName];
            drawArrowToPoint(nodePos);
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in startDFSAnimation: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in startDFSAnimation";
    }
}
void visualizeGraph::animateBFS(){
    try {
        // qDebug() << "Animating DFS";

        // Check if there are nodes left to draw
        if (algosindex < bfspath.size()) {
            // Get the current node from the DFS path
            const Node& currentNode = bfspath[algosindex];

            // Calculate the position of the current node dynamically
            QPointF nodePos = nodesPostitions[currentNode.nodeName];

            // Remove the previously drawn directed image, if it exists
            if (directedArrowItem != nullptr) {
                scene()->removeItem(directedArrowItem);
                delete directedArrowItem;
                directedArrowItem = nullptr;
            }

            // Draw the directed image pointing at the current node
            drawArrowToPoint(nodePos);

            // Increment index for the next node
            ++algosindex;
        } else {
            // Animation finished, stop the timer
            animationTimertwo.stop();

            // Remove the directed image from the scene
            if (directedArrowItem != nullptr) {
                scene()->removeItem(directedArrowItem);
                delete directedArrowItem;
                directedArrowItem = nullptr;
            }
        }
    } catch (const std::exception& e) {
        qDebug() << "Exception caught in animateDFS: " << e.what();
    } catch (...) {
        qDebug() << "Unknown exception caught in animateDFS";
    }
}
void visualizeGraph::resizeEvent(QResizeEvent* event)
{
     QGraphicsView::resizeEvent(event);

    QPixmap backgroundImage(":/resources/space_back2");

    QGraphicsPixmapItem* backgroundItem = scene()->addPixmap(backgroundImage);


    backgroundItem->setPos(0, 0);


    backgroundItem->setZValue(-100);


    backgroundItem->setTransformationMode(Qt::SmoothTransformation);
}
void visualizeGraph::reDraw() {
    scene()->clear();
    QGraphicsPixmapItem* backgroundItem = scene()->addPixmap(backgroundImage);

    // Set the position of the background item to cover the whole scene
    backgroundItem->setPos(0, 0);

    // Ensure the background image is behind all other items
    backgroundItem->setZValue(-100); // Set a low Z-value

    // Optionally, set the aspect ratio mode to ensure the image is not distorted
    backgroundItem->setTransformationMode(Qt::SmoothTransformation);
    // Get all nodes from the graph
    std::vector<Node> nodes = graph->getNodes();

    // Iterate through each node
    for (const auto& node : nodes)
    {
        drawNode(node,1);
        drawEdge(node);
    }
    edgesDrawn.clear();

}
