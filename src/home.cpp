    #include "home.h"
    #include "ui_home.h"
    #include "visualizegraph.h"
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
        visualizeGraph* graphWidget = new visualizeGraph(this);

        graphWidget->setGraph(graph); // Set the graph in the widget
        ui->verticalLayout_2->addWidget(graphWidget);
    }
    Home::~Home()
    {
        delete ui;
    }


    void Home::btn_1_handle(){

        qDebug()<<"first function";
        // ui->textEdit->insertPlainText("hello there");
        // ui->textEdit->setText("logged in successfully");
    }

    void Home::btn_1_handle2(){

        qDebug()<<"second function";
        // ui->textEdit->setText("signed up successfully");

    }

    void Home::ADDnode(){
        // QString userInput = ui->textEdit->toPlainText();
        // int number = userInput.toInt();
    }
    // void Home::layoutGraph() {
    //     // Determine layout parameters
    //     // qDebug() << "hello from layoutgraph";
    //     int numNodes = graph->getNodes().size();
    //     int centerX = width() / 2;
    //     int centerY = height() / 2;
    //     int radius = std::min(width(), height()) / 3;

    //     // Store nodes in a local variable to avoid calling getNodes() repeatedly
    //     const std::vector<Node>& nodes = graph->getNodes();

    //     // Position nodes in a circular layout
    //     double angleIncrement = 2 * M_PI / numNodes;
    //     double angle = 0;
    //     std::vector<QPointF> nodePositions; // Store node positions for later use
    //     for (const Node& node : nodes) {
    //         // qDebug() << "hello from layoutgraph loop";
    //         // Calculate node position
    //         int x = centerX + radius * std::cos(angle)-30;
    //         int y = centerY + radius * std::sin(angle)-30;
    //         nodePositions.push_back(QPointF(x, y));

    //         // Create QGraphicsEllipseItem for nodes
    //         QGraphicsEllipseItem *nodeItem = new QGraphicsEllipseItem(
    //             x - node.radius, y - node.radius,
    //             2 * node.radius, 2 * node.radius
    //             );
    //         scene->addItem(nodeItem);

    //         // Update angle for the next node
    //         angle += angleIncrement;

    //         // Position edges connected to this node
    //         std::vector<Edge> edges = graph->getEdges(node);
    //         for (const Edge& edge : edges) {
    //             // Find the position of the destination node
    //             int destIndex = std::find(nodes.begin(), nodes.end(), edge.destination) - nodes.begin();
    //             QPointF destPos = nodePositions[destIndex];

    //             // Create a QGraphicsLineItem for the edge
    //             QGraphicsLineItem *edgeItem = new QGraphicsLineItem(x, y, destPos.x(), destPos.y());
    //             scene->addItem(edgeItem);
    //         }
    //     }
    //     // qDebug() << "hello from end of layoutgraph";
    // }

