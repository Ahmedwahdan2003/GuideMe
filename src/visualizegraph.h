// visualizegraph.h
#ifndef VISUALIZEGRAPH_H
#define VISUALIZEGRAPH_H
#include<unordered_set>
#include <QGraphicsView>
#include "graph.h"
#include<qtimer.h>
struct PairHash {
    std::size_t operator()(const std::pair<QString, QString>& pair) const {
        // Combine hashes of both QStrings in the pair
        return qHash(pair.first) ^ (qHash(pair.second) << 1);
    }
};
struct TupleHash {
    std::size_t operator()(const std::tuple<QString, QString, QString>& tuple) const {
        // Hash each element of the tuple individually and combine the hashes
        return qHash(std::get<0>(tuple)) ^
               qHash(std::get<2>(tuple));
    }
};
class visualizeGraph : public QGraphicsView
{
    Q_OBJECT
public:
    static unsigned int NodesLeft;
    static unsigned int NodesDrawnidx;
    static bool flag;
    static std::map<QString,QPointF>nodesPostitions;
   static std::unordered_set<std::tuple<QString, QString, QString>,TupleHash> edgesDrawn;
    explicit visualizeGraph(QWidget *parent = nullptr);
    QGraphicsPolygonItem* arrowheadItem;
    QGraphicsLineItem* lineItem;
    // Function to set the graph to be visualized
    void setGraph(Graph* graph);
    void drawNode(const Node& node);
    void drawEdge(const Node& node);
    void mousePressEvent(QMouseEvent *event) override;
    void drawArrowToPoint(const QPointF& targetPos);
    void animateDFS();

    // Index of the next node to draw
signals:

 void allNodesDrawn();
public slots:
void startDFSAnimation();
void updateNodeCounter();
private:
    Graph* graph; // Pointer to the graph object
};

#endif // VISUALIZEGRAPH_H
