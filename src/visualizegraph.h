// visualizegraph.h
#ifndef VISUALIZEGRAPH_H
#define VISUALIZEGRAPH_H
#include<unordered_set>
#include <QGraphicsView>
#include "graph.h"
#include<qtimer.h>
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
    static size_t algosindex;
    static bool isDragAllowed;
    static QString draggedNode;
    static QPointF dragOffset;
    static std::vector<Node>dfspath;
    static std::vector<Node>bfspath;
    QTimer animationTimer;
    QTimer animationTimertwo;
    static bool flag;
    QPixmap backgroundImage;
    static std::map<QString,QPointF>nodesPostitions;
    static std::unordered_set<std::tuple<QString, QString, QString>,TupleHash> edgesDrawn;
    explicit visualizeGraph(QWidget *parent = nullptr);
    QGraphicsPixmapItem* directedArrowItem;
    // Function to set the graph to be visualized
    void setGraph(Graph* graph);
    void drawNode(const Node& node,int choice);
    void drawEdge(const Node& node);
    void reDraw();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
    void drawArrowToPoint(const QPointF& targetPos);
    void animateDFS();
    void animateBFS();
    void setBFSPath(const std::vector<Node>& path);
    QPixmap nodeimg;

    // Index of the next node to draw
signals:

    void allNodesDrawn();
public slots:
    void startDFSAnimation();
    void startBFSAnimation();
    void updateNodeCounter();
private:
    Graph* graph;
    static QMap<QString, QString> nodeImagePaths;// Pointer to the graph object
};

#endif // VISUALIZEGRAPH_H
