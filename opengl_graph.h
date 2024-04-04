#ifndef opengl_graph_H
#define opengl_graph_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class opengl_graph : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

private:
    int viewportWidth;
    int viewportHeight;

public:
    explicit opengl_graph(QWidget *parent = nullptr);
    ~opengl_graph() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    // void drawNode(float x, float y,int number,const float radius);
    void drawCircle(int centerX, int centerY, int radius, const QString& text);
    void drawLine(int startX, int startY, int endX, int endY);
    // void adjustNodePositions(std::vector<Node>& nodes);
public:void addNodeAfterLast(int number);
};



#endif // opengl_graph_H
