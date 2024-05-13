#include "fdwidget.h"
#include "ui_fdwidget.h"
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include "graph.h"
#include <QDebug>
#include <algorithm>
#include <set>
// #include <QMediaPlayer>
// #include <QVideoWidget>
#include <QFileDialog>
#include <QTextEdit>

FDwidget::FDwidget(Graph* graph, QWidget* parent)
    : QWidget(parent), ui(new Ui::FDwidget), graph(graph), pathDisplay(nullptr)
{
    ui->setupUi(this);
    NodeToComboBox(ui->loc_cb, ui->des_cb);

    pathDisplay = new QTextEdit();
    ui->patharea->setWidget(pathDisplay);

    connect(ui->find_bt, &QPushButton::clicked, this, &FDwidget::calculateAndDisplayCost);

    // QMediaPlayer *player = new QMediaPlayer(this);
    // QVideoWidget *videoWidget = new QVideoWidget(this);
    // player->setVideoOutput(videoWidget);

    // // 8ero al path bta3 al vedio ------------------------------
    // player->setSource(QUrl::fromLocalFile("H://GuideMe//src//resources//intro vedio.mp4"));
    // videoWidget->setFixedSize(1280, 720);
    // videoWidget->show();
    // player->play();

    // connect(player, &QMediaPlayer::mediaStatusChanged, this, [videoWidget](QMediaPlayer::MediaStatus status) {
    //     if (status == QMediaPlayer::EndOfMedia) {
    //         videoWidget->hide();
    //     }
    // });

    ui->find_bt->setStyleSheet(
        "QPushButton { background-color: transparent; border: none; }"
        "QPushButton:hover { background-color: rgba(255, 255, 255, 50); }"
        );
}



FDwidget::~FDwidget()
{
    delete ui;
}

void FDwidget::NodeToComboBox(QComboBox* loc_cb,QComboBox* des_cb) {
    std::vector<Node> nodes = graph->getNodes();

    if (nodes.empty()) {
        qWarning() << "Error: No nodes found in the graph.";
        return;
    }

    loc_cb->clear();

    for (const Node& node : nodes) {
        loc_cb->addItem(node.getNodeName());
        des_cb->addItem(node.getNodeName());
    }
}
void FDwidget::calculateAndDisplayCost() {
    // 1. Get Selected Nodes and Cost Limit
    QString startNodeName = ui->loc_cb->currentText();
    QString destNodeName = ui->des_cb->currentText();

    bool conversionOk;
    double costLimit = ui->cost_tb->text().toDouble(&conversionOk);
    if (!conversionOk || costLimit < 0) {
        qDebug() << "Invalid cost limit. Please enter a positive number.";
        ui->cost_tb->clear();
        return;
    }

    // 2. Check if Valid Path Exists
    std::optional<Node> startNodeOptional = graph->getNodeByName(startNodeName);
    std::optional<Node> endNodeOptional = graph->getNodeByName(destNodeName);

    if (!startNodeOptional || !endNodeOptional) {
        qDebug() << "Start or end node not found.";
        return;
    }

    // 3. Calculate Unique Paths and Filter by Cost
    std::vector<std::vector<Node>> allPaths = graph->findAllPaths(*startNodeOptional, *endNodeOptional);
    std::set<std::vector<QString>> uniquePathNodeNames; // Set to track unique node sequences

    // Create a vector to store path and cost pairs
    std::vector<std::pair<std::vector<Node>, double>> pathCostPairs;
    pathDisplay->clear(); // Clear previous content
    std::vector<std::pair<QString,double>>pathsInfo;
    // int cnt = 0;
    for (const std::vector<Node>& path : allPaths) {
        // Extract and sort node names for duplicate check
        std::vector<QString> nodeNames;
        for (const Node& node : path) {
            nodeNames.push_back(node.getNodeName());
        }
        std::sort(nodeNames.begin(), nodeNames.end());

        if (uniquePathNodeNames.find(nodeNames) == uniquePathNodeNames.end()) {
            uniquePathNodeNames.insert(nodeNames);

            std::unordered_map<QString, double> transportationCosts;
            double totalCost = 0.0;
            QString pathInfo = "Path: " + QString::number(pathCostPairs.size() + 1) + "\n";

            for (int i = 0; i < path.size() - 1; ++i) {
                Node source = path[i];
                Node destination = path[i + 1];
                pathInfo += "  from Node " + source.getNodeName() + "\n";

                for (const Edge& edge : graph->getEdges(source.getNodeName())) {
                    if (edge.getDestination() == destination) {
                        QString transportationType = edge.getOption().getName();
                        double cost = edge.getCost();
                        totalCost += cost;
                        transportationCosts[transportationType] += cost;
                        pathInfo += "  Transportation type: " + transportationType + "\n";
                        pathInfo += "  Transportation cost: " + QString::number(cost) + "\n";

                        // Break the loop once the appropriate edge is found
                        break;
                    }
                }

                // Check cost limit after each segment
                if (totalCost > costLimit) {
                    break; // Stop processing this path if cost exceeds limit
                }
            }

            pathInfo += "  to Node " + path.back().getNodeName() + "\n";

            // Filter by cost limit before displaying
            if (totalCost <= costLimit) {
                pathCostPairs.emplace_back(path, totalCost);

                // Display path and cost
                pathInfo += "Total Cost: " + QString::number(totalCost) + "\n";
                pathInfo += "Transportation Costs:\n";

                for (const auto& entry : transportationCosts) {
                    pathInfo += "- " + entry.first + ": " + QString::number(entry.second) + "\n";
                }
                pathInfo += "---------------------------\n";
                pathsInfo.emplace_back(std::make_pair(pathInfo,totalCost));
                // qDebug()<<pathsInfo[cnt].first<<"\n";
                // pathDisplay->append(pathInfo);
            }
        }
        // cnt++;
    }

    // Sort paths by total cost (ascending order)
    std::sort(pathsInfo.begin(), pathsInfo.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    QString pathlastinfo = "";
    for(size_t i = 0;i<pathsInfo.size();i++){
        pathlastinfo+=pathsInfo[i].first;
    }
    pathDisplay->append(pathlastinfo);
    // If no paths found within the cost limit
    if (pathCostPairs.empty()) {
        pathDisplay->append("No paths found within the cost limit.");
    }
}



    // Helper function to generate the path information string
QString FDwidget::generatePathInfo(const Graph* graph, const std::vector<Node>& path, double& totalCost,
                                   std::unordered_map<QString, double>& transportationCosts, int pathNumber)
{
    QString pathInfo = "Path: " + QString::number(pathNumber) + "\n";
    for (int i = 0; i < path.size() - 1; ++i) {
        Node source = path[i];
        Node destination = path[i + 1];
        pathInfo += "  from Node " + source.getNodeName() + "\n";

        for (const Edge& edge : graph->getEdges2(source.getNodeName())) {
            if (edge.getDestination() == destination) {
                QString transportationType = edge.getOption().getName();
                double cost = edge.getCost(); // Use precalculated cost
                pathInfo += "  Transportation type: " + transportationType + "\n";
                pathInfo += "  Transportation cost: " + QString::number(cost) + "\n";
                break;
            }
        }
    }
    pathInfo += "  to Node " + path.back().getNodeName() + "\n";
    return pathInfo;
}
