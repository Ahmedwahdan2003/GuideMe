#include "opengl_graph.h"
#include <QPainter>
#include <QPaintEvent>
#include<vector>
opengl_graph::opengl_graph(QWidget *parent) :
    QOpenGLWidget(parent)
{
}

opengl_graph::~opengl_graph()
{
}
class Node {
public:
    int centerX;
    int centerY;
    int radius;
    int number;

    Node(int x, int y, int r, int num) : centerX(x), centerY(y), radius(r), number(num) {}
};

struct edge {
    int startX;
    int startY;
    int endX;
    int endY;

    edge(int x1, int y1, int x2, int y2) : startX(x1), startY(y1), endX(x2), endY(y2) {}
};

std::vector<edge> edges;
std::vector<Node> nodes;
void opengl_graph::initializeGL()
{
    initializeOpenGLFunctions();

    if (!context()->isValid()) {
        qDebug() << "OpenGL context is not valid!";
        // Handle error or return
        return ;
    }

    // Other initialization code
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void opengl_graph::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    viewportWidth = w;
    viewportHeight = h;
    // Setup orthographic projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Adjust the size of the projection matrix
    glOrtho(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void opengl_graph::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f); // Set color to black

    for (const auto& node : nodes) {
        drawCircle(node.centerX, node.centerY, node.radius, QString::number(node.number));
    }

    for (const auto& edge : edges) {
        drawLine(edge.startX, edge.startY, edge.endX, edge.endY);
    }
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qDebug() << "OpenGL error:" << error;
    }
}


void opengl_graph::drawCircle(int centerX, int centerY, int radius, const QString& text) {
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

void opengl_graph::drawLine(int startX, int startY, int endX, int endY) {
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


void opengl_graph::addNodeAfterLast(int number) {
    int newX = 0, newY = 0;                                          //{WAHDAN}   // if the vector is empty insitalize the first node position in the middle of the screen
    if (!nodes.empty()) {
        const Node& lastNode = nodes.back();                               //{WAHDAN} it will return refrence to last node (akenaha btraga3 el object nafso msh pointer)
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
    nodes.emplace_back(newX, newY, 20, number);
    const float minDistance = 50.0f; // Minimum distance between nodes
    const float pushFactor = 10.0f; // Factor to adjust positions when nodes are too close
        //{WAHDAN} Rearrange the nodes positions so that no nodes is overlaping
    for (size_t i = 0; i < nodes.size(); ++i) {                                             //{WAHDAN}  get the distance between every 2 nodes and if the disance <50
        //{WAHDAN} push them away abit from each other
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            float dx = nodes[j].centerX - nodes[i].centerX;
            float dy = nodes[j].centerY - nodes[i].centerY;
            float distance = std::sqrt(dx * dx + dy * dy);//{WAHDAN}euclidean distance

            if (distance < minDistance) {
                // Nodes are too close, adjust positions
                float adjust = (minDistance - distance) / 2.0f;
                float angle = std::atan2(dy, dx);                //{WAHDAN} angle between the two nodes
                float offsetX = std::cos(angle) * adjust * pushFactor;          //{WAHDAN} cos give us the horizontal component so we can know how much to push the node in the x axis
                float offsetY = std::sin(angle) * adjust * pushFactor;          //{WAHDAN} sin give us the vertical component so we can know how much to push the node in the y axis

                // Adjust positions of both nodes
                nodes[i].centerX -= offsetX;
                nodes[i].centerY -= offsetY;
                nodes[j].centerX += offsetX;
                nodes[j].centerY += offsetY;
            }
        }
    }
    // Draw a edge connecting the new node to the last node
    if (nodes.size() > 1) {
        const Node& lastNode = nodes[nodes.size() - 2];
        edges.emplace_back(lastNode.centerX, lastNode.centerY, newX, newY);
    }

    update(); // Trigger repaint //{WAHDAN} after rearranging and adjusting all the positions redraw the graph
}


