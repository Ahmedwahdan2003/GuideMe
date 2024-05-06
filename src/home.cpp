#include "home.h"
#include "ui_home.h"
#include "graph.h"
#include"addt.h"
#include"updatet.h"
#include"deletet.h"
#include <QMessageBox>



Home::Home(QWidget *parent) : QMainWindow(parent), ui(new Ui::Home), graph(new Graph())
{
    ui->setupUi(this);

    // Allocate the Graph object
    if (graph == nullptr) {
        qDebug() << "Failed to allocate memory for Graph object";
        return;
    }

    // Read graph data from file
    if (!graph->readGraphFile("E:\\Year Two\\Semester 4\\Data Structures\\GuideMe\\src\\Graph.txt")) {
        qDebug() << "Failed to read graph data from file";
        delete graph;
        return;
    }


    graph->printGraph();




    graphWidget->setGraph(graph);
    ui->verticalLayout_2->addWidget(graphWidget);

}

Home::~Home()
{
    graph->writeGraphToFile("E:\\Year Two\\Semester 4\\Data Structures\\GuideMe\\src\\Graph.txt");
    delete graph;
    delete ui;

}


void Home::btn_1_handle(){
    graphWidget->startDFSAnimation();
}

void Home::btn_1_handle2(){

}

void Home::ADDnode(){
}

void Home::on_Add_btn_clicked()
{

        addT *addTransportation = new addT(graph,graphWidget, this);
        addTransportation->show();

}



void Home::on_Del_btn_clicked()
{
    deletet *form = new deletet(graph, this);
    form->show();
}


void Home::on_Update_btn_clicked()
{
    updatet *form = new updatet(graph,this);
   form->show();
}


void Home::on_pushButton_clicked()
{

        // Check if the graph is complete
        if (graph->isCompleteGraph()) {
            QMessageBox::information(this, "Graph Complete", "The graph is complete.");
        } else {
            QMessageBox::information(this, "Graph Not Complete", "The graph is not complete.");
        }
        graph->printGraph();


}


void Home::on_BFS_btn_clicked()
{
      graphWidget->startBFSAnimation();
}

