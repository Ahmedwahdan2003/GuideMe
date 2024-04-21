#include "graph_view.h"
#include <QPainter>
#include <QPaintEvent>
#include <random>
// #include<vector>
graph_view::~graph_view(){

}
graph_view::graph_view(QWidget *parent, Graph *g): QOpenGLWidget(parent), graph(g)
{
    // Check if the graph pointer is not null before accessing its data
    // if (graph && !graph->getNodes().empty()) {
    //     qDebug() << "First node name: " << graph->getNodes()[0].nodeName;
    // }
}

void graph_view::initializeGL()
{
    initializeOpenGLFunctions();

    if (!context()->isValid()) {
        qDebug() << "OpenGL context is not valid!";
        return;
    }

    //qDebug() << "Initializing OpenGL";

    // Access the graph object here if needed
    // if (graph && !graph->getNodes().empty()) {
    //     qDebug() << "First node name: " << graph->getNodes()[0].nodeName;
    // }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void graph_view::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    viewportWidth = w;
    viewportHeight = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void graph_view::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f);
    std::vector<Node>nodes = applyLayoutAlgorithm();
    for (Node& node : nodes) {
        drawCircle(node.centerX, node.centerY, node.radius, node.getNodeName());
        for(Edge& edge: graph->getEdges(node)){
            drawLine(edge.startX, edge.startY, edge.endX, edge.endY);
        }
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qDebug() << "OpenGL error:" << error;
    }
}


void graph_view::drawCircle(int centerX, int centerY, int radius, const QString& text) {
    // qDebug()<<"hey draw cirlce";
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; ++i) {
        glColor3f(0.0f, 0.0f, 0.0f);
        float angle = i * 3.14159 / 180;
        glVertex2f(centerX + radius * std::cos(angle), centerY + radius * std::sin(angle));
    }
    glEnd();

    // Draw text in the middle of the circle using QPainter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font = painter.font();
    font.setPointSize(12); // Adjust font size as needed
    painter.setFont(font);
    painter.setPen(Qt::red);
    QRect textRect = painter.fontMetrics().boundingRect(text);
    int textWidth = textRect.width();
    int textHeight = textRect.height();

    // Convert OpenGL coordinates to widget coordinates
    int widgetCenterX = viewportWidth / 2;
    int widgetCenterY = viewportHeight / 2;
    int widgetX = centerX + widgetCenterX;
    int widgetY = widgetCenterY - centerY;

    // Adjust text position to center within circle
    int textX = widgetX - textWidth / 2;
    int textY = widgetY - textHeight / 2;

    // Check if text is within viewport bounds
    if (textX < 0) {
        textX = 0;
    } else if (textX + textWidth > viewportWidth) {
        textX = viewportWidth - textWidth;
    }
    if (textY < 0) {
        textY = 0;
    } else if (textY + textHeight > viewportHeight) {
        textY = viewportHeight - textHeight;
    }

    // Draw text
    painter.drawText(textX, textY+15, text);
    // update();
}

void graph_view::drawLine(int startX, int startY, int endX, int endY) {
    //  qDebug()<<"hey drawline";
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(startX, startY);
    glVertex2f(endX, endY);
    glEnd();
    QString weightText = QString::number(20);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font = painter.font();
    font.setPointSize(15); // Adjust font size as needed
    painter.setFont(font);
    painter.setPen(Qt::blue); // Set color for the weight text
    QRect textRect = painter.fontMetrics().boundingRect(weightText);
    int textWidth = textRect.width();
    int textHeight = textRect.height();

    // Convert OpenGL coordinates to widget coordinates for midpoint
    int widgetMidX = (startX + endX + width()) / 2;
    int widgetMidY = (startY + endY + height()) / 2;

    // Adjust text position to center it within the edge
    int textX = widgetMidX - textWidth / 2;
    int textY = height() - (widgetMidY + textHeight / 2); // Adjust Y-coordinate for QPainter's coordinate system

    // Draw text
    painter.drawText(textX, textY, weightText);

}


void graph_view::addNodeAfterLast(QString number) {
    int newX = 0, newY = 0;                                          //{WAHDAN}   // if the vector is empty insitalize the first node position in the middle of the screen
    if (!(graph->empty())) {
        const Node& lastNode = graph->back();                               //{WAHDAN} it will return refrence to last node (akenaha btraga3 el object nafso msh pointer)
        newX = lastNode.centerX + 100; // Adjust as needed for spacing      //{WAHDAN} size of the new Node
        newY = lastNode.centerY + 100; // Adjust as needed for spacing
    }

    // Check if the new node's position exceeds the viewport boundaries
    if (newX < -viewportWidth / 2) {                                        //{WAHDAN} (if it reaches the top it goes down to more than half the screen) and the same for every direction
        newX = (viewportWidth / 2.4)-100;
    } else if (newX > viewportWidth / 2) {
        newX = (-viewportWidth / 2.4)+100;
    }
    if (newY < -viewportHeight / 2) {
        newY = (viewportHeight / 2.4)-100;
    } else if (newY > viewportHeight / 2) {
        newY = (-viewportHeight / 2.4)+100;
    }

    // Update scene state
    Node newNode = Node(number,newX,newY,20);
    graph->addNode(newNode);
    const float minDistance = 50.0f; // Minimum distance between nodes
    const float pushFactor = 10.0f; // Factor to adjust positions when nodes are too close
        //{WAHDAN} Rearrange the nodes positions so that no nodes is overlaping
    // Adjust node positions to prevent overlap
    for (size_t i = 0; i < graph->getNodes().size(); ++i) {
        for (size_t j = i + 1; j < graph->getNodes().size(); ++j) {
            Node node1 = graph->getNodes()[i];
            Node node2 = graph->getNodes()[j];

            float dx = node2.centerX - node1.centerX;
            float dy = node2.centerY - node1.centerY;
            float distance = std::sqrt(dx * dx + dy * dy); // Euclidean distance

            if (distance < minDistance) {
                // Nodes are too close, adjust positions
                float adjust = (minDistance - distance) / 2.0f;
                float angle = std::atan2(dy, dx); // Angle between the two nodes
                float offsetX = std::cos(angle) * adjust * pushFactor;
                float offsetY = std::sin(angle) * adjust * pushFactor;

                // Adjust positions of both nodes
                node1.centerX -= offsetX;
                node1.centerY -= offsetY;
                node2.centerX += offsetX;
                node2.centerY += offsetY;
            }
        }
    }
    // Draw a edge connecting the new node to the last node
    if (graph->getNodes().size() > 1) {
        // Get the last node in the graph
        const Node lastNode = graph->getNodes().back();

        // Get the second-to-last node in the graph
        const std::vector<Node> nodes = graph->getNodes();
        const Node secondToLastNode = nodes[nodes.size() - 2];

        // Draw the edge between the second-to-last node and the new node
        drawLine(secondToLastNode.centerX, secondToLastNode.centerY, newX, newY);
    }

    update(); // Trigger repaint //{WAHDAN} after rearranging and adjusting all the positions redraw the graph
}
std::vector<Node> graph_view::applyLayoutAlgorithm() {
    const float k = 2.0f;
    const float c = 3.0f;
    const float damping = 5.0f;
    const int iterations = 100;

        // Initialize node positions randomly within a specified range
        std::vector<Node> nodes = graph->getNodes();

        // Initialize velocities for each node
        std::vector<QPointF> velocities(nodes.size(), QPointF(0.0f, 0.0f));
        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distX(-100.0f, 100.0f); // Adjust the range as needed
        std::uniform_real_distribution<float> distY(-100.0f, 100.0f); // Adjust the range as needed

        // Assign random positions to nodes
        for (size_t i = 0; i < nodes.size(); ++i) {
            nodes[i].centerX = distX(gen);
            nodes[i].centerY = distY(gen);
        }

    for (int iter = 0; iter < iterations; ++iter) {
        for (size_t i = 0; i < nodes.size(); ++i) {
            for (size_t j = i + 1; j < nodes.size(); ++j) {
                QPointF d = QPointF(nodes[j].centerX, nodes[j].centerY) - QPointF(nodes[i].centerX, nodes[i].centerY);
                float distance = std::sqrt(d.x() * d.x() + d.y() * d.y());
                qDebug()<<"distance: "<<distance<<"\n";
                QPointF force = c * (d / ((distance != 0 ? distance : 1)+20)) / ((distance * distance) + 20);
                qDebug()<<"force: "<<force<<"\n";
                velocities[i] -= force;
                velocities[j] += force;
            }
        }

        for (const Edge& edge : graph->getEdges()) {
            QPointF d = QPointF(nodes[edge.destination.centerX].centerX, nodes[edge.destination.centerY].centerY) - QPointF(nodes[edge.parent.centerX].centerX, nodes[edge.parent.centerY].centerY);
            float distance = std::sqrt(d.x() * d.x() + d.y() * d.y());

            QPointF force = k * (distance - 20) * (d / (distance+20));
            velocities[edge.parent.centerX] += force;
            velocities[edge.parent.centerY] += force;
            velocities[edge.destination.centerX] -= force;
            velocities[edge.destination.centerY] -= force;
        }

        for (size_t i = 0; i < nodes.size(); ++i) {
            QPointF newPos = QPointF(nodes[i].centerX, nodes[i].centerY) + velocities[i];
            qDebug()<<nodes[i].nodeName<<" "<<nodes[i].centerX<<" "<<nodes[i].centerY<<" "<<velocities[i]<<"\n";
            nodes[i].centerX = newPos.x();
            nodes[i].centerY = newPos.y();

            velocities[i] *= damping;
        }
    }
    return nodes;
    //update();
}

