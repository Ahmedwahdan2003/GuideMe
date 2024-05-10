#include "addt.h"
#include "ui_addt.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>
#include <home.h>
#include<QMessageBox>

addT::addT(Graph *graph, visualizeGraph *graphWid , QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addT)
    , graph(graph)
    , graphWid(graphWid)
{
    ui->setupUi(this);
}

addT::~addT()
{
    delete ui;
}
void addT::on_pushButton_clicked()
{
    if (graph) {
        QString sourceA = ui->lineEdit->text();
        QString destA = ui->lineEdit_2->text();
        QString transA = ui->lineEdit_3->text();
        QString costA = ui->lineEdit_4->text();

        if (sourceA.isEmpty() || destA.isEmpty() || transA.isEmpty() || costA.isEmpty()) {
            QMessageBox::critical(this, "Error", "Please fill in all fields.");
            return;
        }

        // Check if both source and destination nodes are valid in the graph
        // if (!graph->isValidNode(sourceA) || !graph->isValidNode(destA)) {
        //     QMessageBox::critical(this, "Error", "Invalid source or destination node.");
        //     return;
        // }
        if (sourceA == destA) {
            QMessageBox::critical(this, "Error", "Same source and destination.");
            return;
        }
        // Add edge to the graph
            int transportationInt = costA.toInt();
            Transportation transportation(transA, transportationInt);

            graph->addEdge(sourceA, destA, transportation);
        close();
        graphWid->reDraw();


    } else {
        qDebug() << "Error: Graph object is nullptr.";

    }
}
