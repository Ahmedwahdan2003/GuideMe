#include "updatet.h"
#include "ui_updatet.h"
#include "qdebug.h"
#include <graph.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>
#include <home.h>
#include <QMessageBox>



updatet::updatet(Graph *graph,visualizeGraph *graphWid ,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::updatet)
    , graph(graph)
    , graphWid(graphWid)
{
    ui->setupUi(this);
}

updatet::~updatet()
{
    delete ui;
}

void updatet::on_pushButton_clicked()
{

    QString sourceu = ui->lineEdit->text();
    QString destinationu = ui->lineEdit_2->text();
    QString oldTransportationu = ui->lineEdit_3->text();
    QString newTransportationu = ui->lineEdit_4->text();
    int newCostu = ui->lineEdit_5->text().toInt();
    if (sourceu.isEmpty() || destinationu.isEmpty() || oldTransportationu.isEmpty()|| newTransportationu.isEmpty()) {
        QMessageBox::critical(this, "Error", "Please fill in all fields.");
        return;
    }

    // Check if both source and destination nodes are valid in the graph
    if (!graph->isValidNode(sourceu) || !graph->isValidNode(destinationu)) {
        QMessageBox::critical(this, "Error", "Invalid source or destination node.");
        return;
    }
    if (!graph->isValidEdge(sourceu,destinationu,oldTransportationu)) {
        QMessageBox::critical(this, "Error", "Invalid transportation");
        return;
    }
    if (sourceu == destinationu) {
        QMessageBox::critical(this, "Error", "Same source and destination.");
        return;
    }
    graph->updateEdge( sourceu,  destinationu, oldTransportationu, newTransportationu, newCostu) ;
    close();
    graphWid->reDraw();
}
