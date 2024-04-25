// visualizegraph.h
#ifndef VISUALIZEGRAPH_H
#define VISUALIZEGRAPH_H

#include <QGraphicsView>
#include "graph.h"

class visualizeGraph : public QGraphicsView
{
    Q_OBJECT
public:
    static unsigned int NodesLeft;
    static unsigned int NodesDrawnidx;
    static bool flag;
    explicit visualizeGraph(QWidget *parent = nullptr);

    // Function to set the graph to be visualized
    void setGraph(Graph* graph);
    void drawNode(const Node& node);
    void drawEdge(const Edge& edge, const std::map<QString, QPointF>& nodePositions);
    void mousePressEvent(QMouseEvent *event) override;

signals:
 void allNodesDrawn();
public slots:
void updateNodeCounter();
private:
    Graph* graph; // Pointer to the graph object
};

#endif // VISUALIZEGRAPH_H
