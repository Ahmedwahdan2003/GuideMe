// visualizegraph.h
#ifndef VISUALIZEGRAPH_H
#define VISUALIZEGRAPH_H

#include <QGraphicsView>
#include "graph.h"

class visualizeGraph : public QGraphicsView
{
    Q_OBJECT
public:
    explicit visualizeGraph(QWidget *parent = nullptr);

    // Function to set the graph to be visualized
    void setGraph(Graph* graph);

signals:

public slots:

private:
    Graph* graph; // Pointer to the graph object
};

#endif // VISUALIZEGRAPH_H
