    #include "home.h"
    #include "ui_home.h"
    #include "graph.h"

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
        if (!graph->readGraphFile("C:\\Users\\ahmed\\Desktop\\GUIDE_ME\\GuideMe\\src\\Graph.txt")) {
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
