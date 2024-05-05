#include "updatet.h"
#include "ui_updatet.h"
#include "qdebug.h"
#include<graph.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>



updatet::updatet(Graph *graph,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::updatet)
    , graph(graph)
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
    graph->updateEdge( sourceu,  destinationu, oldTransportationu, newTransportationu, newCostu) ;
    close();
}
