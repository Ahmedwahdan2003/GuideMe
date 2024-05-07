#include "bfswidget.h"
#include "ui_bfswidget.h"
#include "visualizegraph.h"
#include "graph.h"
#include <QMessageBox>

// {Omar Khales AKA (TWIX) :) }


BFSwidget::BFSwidget(Graph *graph, visualizeGraph *graphWid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BFSwidget)
    , graph(graph)
    , graphWid(graphWid)
{
    startNode = nullptr;
    ui->setupUi(this);

    if (!graph || !graphWid) {
        qDebug() << "Error: Graph or visualizeGraph pointer is null.";
        return;
    }
}

BFSwidget::~BFSwidget()
{
    delete ui;
}

void BFSwidget::on_pushButton_clicked() // {Omar Khales AKA (TWIX) made this}
{
    QString sourceNode = ui->lineEdit->text();
    qDebug() << sourceNode;
    // Search for the node the user entered
    for(size_t i = 0; i < graph->getNodes().size(); i++)
    {
        if(graph->getNodes()[i].getNodeName() == sourceNode)
        {
            startNode = & graph->getNodes()[i];
            graphWid-> bfspath = graph->BFS(graph->getNodes()[i]); // Pass the node to the BFS function
            break; // Found the node and path is now set , exit the loop
        }
    }

    if (startNode != nullptr) // Check if startNode is valid
    {
        // Start the animation

        graphWid-> startBFSAnimation();
        close();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Please Enter Start Node . ");
    }
}

