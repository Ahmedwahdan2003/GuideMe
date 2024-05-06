#include "deletet.h"
#include "ui_deletet.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>
#include<graph.h>
#include<home.h>
#include<QMessageBox>



deletet::deletet(Graph *graph, visualizeGraph *graphWid ,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::deletet)
    , graph(graph)
    , graphWid(graphWid)
{
    ui->setupUi(this);
}

deletet::~deletet()
{
    delete ui;
}

void deletet::on_pushButton_clicked()
{
    QString sourceT = ui->lineEdit->text();
    QString destinationT = ui->lineEdit_2->text();
    QString Transportation = ui->lineEdit_3->text();
    if (sourceT.isEmpty() || destinationT.isEmpty() || Transportation.isEmpty()) {
        QMessageBox::critical(this, "Error", "Please fill in all fields.");
        return;
    }

    // Check if both source and destination nodes are valid in the graph
    if (!graph->isValidNode(sourceT) || !graph->isValidNode(destinationT)) {
        QMessageBox::critical(this, "Error", "Invalid source or destination node.");
        return;
    }
    if (!graph->isValidEdge(sourceT,destinationT,Transportation)) {
        QMessageBox::critical(this, "Error", "Invalid transportation");
        return;
    }
    graph->deleteEdge(sourceT,destinationT,Transportation);


    close();
    graphWid->reDraw();
}

