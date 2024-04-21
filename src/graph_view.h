#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include <QOpenGLWidget>
#include <QWidget>
#include <QOpenGLFunctions>
#include "graph.h"

class graph_view : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    int viewportWidth;
    int viewportHeight;
    Graph* graph;
public:
    explicit graph_view(QWidget* parent = nullptr, Graph* g = nullptr);
    ~graph_view() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void drawCircle(int centerX, int centerY, int radius, const QString& text);
    void drawLine(int startX, int startY, int endX, int endY);


public:
    void addNodeAfterLast(QString number);
    std::vector<Node> applyLayoutAlgorithm();
};

#endif // GRAPH_VIEW_H
