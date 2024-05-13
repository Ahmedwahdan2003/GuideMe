#ifndef FDWIDGET_H
#define FDWIDGET_H
#include <QComboBox>
#include <QWidget>
#include <Home.h>
#include<graph.h>
#include <QTextEdit>
#include <set>

namespace Ui {
class FDwidget;

}

class FDwidget : public QWidget
{
    Q_OBJECT


public:
    explicit FDwidget(Graph *graph,QWidget *parent = nullptr);
    ~FDwidget();
    Node qStringToNode(const QString& qstr);



private:
    Ui::FDwidget *ui;
    QComboBox* loc_cb;
    QComboBox* des_cb;
    void calculateAndDisplayCost();
    void NodeToComboBox(QComboBox* loc_cb,QComboBox* des_cb);
    Graph *graph;
    void find_bt();
    QTextEdit* pathDisplay;
    QString generatePathInfo(const Graph* graph, const std::vector<Node>& path, double& totalCost,
                             std::unordered_map<QString, double>& transportationCosts, int pathNumber);


};

#endif // FDWIDGET_H
