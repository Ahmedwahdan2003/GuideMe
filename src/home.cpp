    #include "home.h"
#include "ui_home.h"
#include "graph.h"
#include"addt.h"
#include"updatet.h"
#include"deletet.h"
#include <QMessageBox>



Home::Home(QWidget *parent) : QMainWindow(parent), ui(new Ui::Home)
{
    ui->setupUi(this);

    // Allocate the Graph object dynamically
    Graph* graph = new Graph();
    // Check if memory allocation was successful
    if (graph == nullptr) {
        qDebug() << "Failed to allocate memory for Graph object";
        return;
    }

    // Read graph data from file
    if (!graph->readGraphFile("C:\\Users\\User\\OneDrive\\Desktop\\New folder (4)\\GuideMe\\src\\Graph.txt")) {
        qDebug() << "Failed to read graph data from file";
        delete graph; // Free the allocated memory
        return;
    }

    // Print graph data for debugging
    graph->printGraph();

    // Pass 'this' as the parent widget


    graphWidget->setGraph(graph); // Set the graph in the widget
    ui->verticalLayout_2->addWidget(graphWidget);

    // Connect the button click to the DFS animation signal

}
Home::~Home()
{
    delete ui;
}


void Home::btn_1_handle(){
    graphWidget->startDFSAnimation();
}

void Home::btn_1_handle2(){

}

void Home::ADDnode(){
    // QString userInput = ui->textEdit->toPlainText();
    // int number = userInput.toInt();
}

void Home::on_Add_btn_clicked()
{ addT *form = new addT(this); // Assuming MyForm has a parent
    // Show the form
    form->show();

}


void Home::on_Del_btn_clicked()
{
    deletet *form = new deletet(this); // Assuming MyForm has a parent
    // Show the form
    form->show();

}


void Home::on_Update_btn_clicked()
{
    updatet *form = new updatet(this); // Assuming MyForm has a parent
    // Show the form
    form->show();
}


void Home::on_pushButton_clicked()
{

        // Get the singleton instance of Graph
        Graph& graph = Graph::getInstance();

        // Check if the graph is complete
        if (graph.isCompleteGraph()) {
            QMessageBox::information(this, "Graph Complete", "The graph is complete.");
        } else {
            QMessageBox::information(this, "Graph Not Complete", "The graph is not complete.");
        }


}


void Home::on_BFS_btn_clicked()
{
      graphWidget->startBFSAnimation();
}

