#include "UndirectedGraphWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include<vector>
UndirectedGraphWidget::UndirectedGraphWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
}

UndirectedGraphWidget::~UndirectedGraphWidget()
{
}
\
    class Node {
    public:
    int centerX;
    int centerY;
    int radius;
    int number;

    Node(int x, int y, int r, int num) : centerX(x), centerY(y), radius(r), number(num) {}
};
    struct Line {
        int startX;
        int startY;
        int endX;
        int endY;

        Line(int x1, int y1, int x2, int y2) : startX(x1), startY(y1), endX(x2), endY(y2) {}
    };
    std::vector<Line> lines;

std::vector<Node> nodes;
void UndirectedGraphWidget::initializeGL()
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

void UndirectedGraphWidget::resizeGL(int w, int h)
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

void UndirectedGraphWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f); // Set color to black

    for (const auto& node : nodes) {
        drawCircle(node.centerX, node.centerY, node.radius, QString::number(node.number));
    }

    for (const auto& line : lines) {
        drawLine(line.startX, line.startY, line.endX, line.endY);
    }
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qDebug() << "OpenGL error:" << error;
    }
}

// void UndirectedGraphWidget::drawNode(float x, float y, int number, float radius)
// {
//     // Draw a circle representing the node
//     const int segments = 50;
//     glBegin(GL_POLYGON);
//     for (int i = 0; i < segments; ++i) {
//         float angle = 2.0f * M_PI * i / segments;
//         float xPos = x + radius * cos(angle);
//         float yPos = y + radius * sin(angle);
//         glVertex2f(xPos, yPos);
//     }
//     glEnd();

//     // Render number inside the node using QPainter
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);
//     painter.setPen(Qt::black);
//     QFont font("Arial", 12);
//     painter.setFont(font);

//     // Calculate bounding rectangle for text
//     QRectF textRect(x - radius, y - radius, 2 * radius, 2 * radius);

//     // Center text within the bounding rectangle
//     painter.drawText(textRect, Qt::AlignCenter, QString::number(number));
// }

void UndirectedGraphWidget::drawCircle(int centerX, int centerY, int radius, const QString& text) {
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

void UndirectedGraphWidget::drawLine(int startX, int startY, int endX, int endY) {
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

    // Adjust text position to center it within the line
    int textX = widgetMidX - textWidth / 2;
    int textY = height() - (widgetMidY + textHeight / 2); // Adjust Y-coordinate for QPainter's coordinate system

    // Draw text
    painter.drawText(textX, textY, weightText);

}


void UndirectedGraphWidget::addNodeAfterLast(int number) {
    int newX = 0, newY = 0;
    if (!nodes.empty()) {
        const Node& lastNode = nodes.back();
        newX = lastNode.centerX + 100; // Adjust as needed for spacing
        newY = lastNode.centerY + 100; // Adjust as needed for spacing
    }

    // Update scene state
    nodes.emplace_back(newX, newY, 20, number);

    // Draw a line connecting the new node to the last node
    if (nodes.size() > 1) {
        const Node& lastNode = nodes[nodes.size() - 2];
        lines.emplace_back(lastNode.centerX, lastNode.centerY, newX, newY);
    }

    update(); // Trigger repaint

    // Node newNode(newX, newY, 20, number);
    // nodes.push_back(newNode);
}

