#include "deletet.h"
#include "ui_deletet.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>
#include<graph.h>



deletet::deletet(Graph *graph,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::deletet)
     , graph(graph)
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
    graph->deleteEdge(sourceT,destinationT,Transportation);


    close();
}

