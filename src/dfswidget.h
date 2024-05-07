#ifndef DFSWIDGET_H
#define DFSWIDGET_H
#include"graph.h"
#include <QDialog>
#include"visualizegraph.h"
namespace Ui {
class DFSWidget;
}

class DFSWidget : public QDialog
{
    Q_OBJECT

public:
    explicit DFSWidget(Graph *graph, visualizeGraph *graphWid ,QWidget *parent = nullptr);
    ~DFSWidget();
private slots:
    void on_pushButton_clicked();

private:
    Ui::DFSWidget *ui;
    Graph *graph;
    visualizeGraph *graphWid;
    Node *startNode;
};

#endif // DFSWIDGET_H
