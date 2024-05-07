#include "dfswidget.h"
#include "ui_dfswidget.h"
#include "visualizegraph.h"
#include "graph.h"
#include <QMessageBox>
// {Omar Khales AKA (TWIX) :)}
DFSWidget::DFSWidget(Graph *graph, visualizeGraph *graphWid, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DFSWidget)
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

DFSWidget::~DFSWidget()
{
    delete ui;
}

void DFSWidget::on_pushButton_clicked() // {Omar Khales AKA (TWIX) made this}
{
    QString sourceNode = ui->lineEdit->text();
    qDebug() << sourceNode;
    // Search for the node the user entered
    for(size_t i = 0; i < graph->getNodes().size(); i++)
    {
        if(graph->getNodes()[i].getNodeName() == sourceNode)
        {
            startNode = & graph->getNodes()[i];
            graphWid-> dfspath = graph->DFS(graph->getNodes()[i]); // Pass the node to the BFS function
            break; // Found the node and path is now set , exit the loop
        }
    }

    if (startNode != nullptr) // Check if startNode is valid
    {
        // Start the animation

        graphWid-> startDFSAnimation();
        close();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Please Enter Start Node . ");
    }
}
