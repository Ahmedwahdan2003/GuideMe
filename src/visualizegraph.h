#ifndef VISUALIZEGRAPH_H
#define VISUALIZEGRAPH_H
#include "graph.h"
#include <QGraphicsWidget>

class visualizeGraph : public QGraphicsWidget

{
    Q_OBJECT
public:
    explicit visualizeGraph(QGraphicsItem *parent = nullptr);


    void setGraph(Graph *mygraph);

    void visualizeDFS(std::vector<Node> &path);
    void visualizeBFS(std::vector<Node> &path);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Graph* graph;
};

#endif // VISUALIZEGRAPH_H
