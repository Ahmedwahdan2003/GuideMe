#include "home.h"
#include "ui_home.h"
#include "graph.h"
#include"addt.h"
#include"updatet.h"
#include"deletet.h"
#include"bfswidget.h"
#include"dfswidget.h"
#include <QMessageBox>
#include<qfile.h>



Home::Home(QWidget *parent) : QMainWindow(parent), ui(new Ui::Home), graph(new Graph())
{
    ui->setupUi(this);

    // Allocate the Graph object
    if (graph == nullptr) {
        qDebug() << "Failed to allocate memory for Graph object";
        return;
    }

    // Read graph data from file
    if (!graph->readGraphFile("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\Graph.txt")) {
        qDebug() << "Failed to read graph data from file";
        delete graph;
        return;
    }


    graph->printGraph();




    graphWidget->setGraph(graph);
    ui->verticalLayout_2->addWidget(graphWidget);

    QString imagePath =":/resources/space_back2";
        QPixmap backgroundImage(imagePath);
        if (!backgroundImage.isNull()) {
            QPalette palette;
            palette.setBrush(this->backgroundRole(), QBrush(backgroundImage));
            this->setPalette(palette);
            this->setAutoFillBackground(true);
        } else {
            qDebug() << "Failed to load image at path: " << imagePath;
        }




}

Home::~Home()
{
    graph->writeGraphToFile("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\Graph.txt");
    delete graph;
    delete ui;

}


void Home::btn_1_handle(){
    DFSWidget *form = new DFSWidget(graph,graphWidget,this);
    form->show();

    //graphWidget->startDFSAnimation();
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
    deletet *form = new deletet(graph,graphWidget, this);
    form->show();
}


void Home::on_Update_btn_clicked()
{
    updatet *form = new updatet(graph,graphWidget,this);
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
    BFSwidget *form = new BFSwidget(graph,graphWidget,this);
    form->show();
    // graphWidget->startBFSAnimation();
}

