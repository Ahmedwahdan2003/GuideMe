#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QScrollArea> // Include QScrollArea header
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <climits>

// Function to generate DOT file from adjacency list with specified size and node shape
void generateDotFile(const std::map<int, std::vector<std::pair<int, int>>>& adjacencyList, const std::string& filename, int width, int height, const std::string& nodeShape, const std::vector<int>& shortestPath) {
    std::ofstream dotFile(filename);
    dotFile << "graph mygraph {" << std::endl;
    dotFile << "  size=\"" << width << "," << height << "\";" << std::endl;
    dotFile << "  node [shape=" << nodeShape << "];" << std::endl; // Set node shape
    // Add nodes and edges to the DOT file
    for (const auto& node : adjacencyList) {
        int source = node.first;
        for (const auto& edge : node.second) {
            int target = edge.first;
            int weight = edge.second;
            dotFile << "  " << source << " -- " << target;
            if (std::find(shortestPath.begin(), shortestPath.end(), source) != shortestPath.end() && std::find(shortestPath.begin(), shortestPath.end(), target) != shortestPath.end()) {
                dotFile << " [label=\"" << weight << "\", color=red];" << std::endl;
            } else {
                dotFile << " [label=\"" << weight << "\"];" << std::endl;
            }
        }
    }
    dotFile << "}" << std::endl;
    dotFile.close();
}

// Function to generate graph image using Graphviz
void generateGraph(const std::string& dotFile, const std::string& outputImage) {
    std::string command = "dot -Tpng " + dotFile + " -o " + outputImage;
    system(command.c_str());
}

// Function to find shortest path between two nodes using Dijkstra's algorithm
std::vector<int> shortestPath(const std::map<int, std::vector<std::pair<int, int>>>& adjacencyList, int source, int target) {
    std::vector<int> path;
    std::vector<int> dist(adjacencyList.size(), INT_MAX);
    std::vector<int> prev(adjacencyList.size(), -1);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (const auto& edge : adjacencyList.at(u)) {
            int v = edge.first;
            int weight = edge.second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    int curr = target;
    while (curr != -1) {
        path.push_back(curr);
        curr = prev[curr];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Sample adjacency list representation of the graph
    std::map<int, std::vector<std::pair<int, int>>> adjacencyList;
    adjacencyList[0] = {{1, 5}, {2, 3}, {3, 7}};
    adjacencyList[1] = {{0, 5}, {2, 1}, {4, 9}};
    adjacencyList[2] = {{0, 3}, {1, 1}, {3, 4}, {5, 8}};
    adjacencyList[3] = {{0, 7}, {2, 4}, {6, 6}};
    adjacencyList[4] = {{1, 9}, {5, 2}, {7, 5}};
    adjacencyList[5] = {{2, 8}, {4, 2}, {8, 3}};
    adjacencyList[6] = {{3, 6}, {9, 1}};
    adjacencyList[7] = {{4, 5}, {8, 4}, {10, 2}};
    adjacencyList[8] = {{5, 3}, {7, 4}, {11, 7}};
    adjacencyList[9] = {{6, 1}, {12, 5}};
    adjacencyList[10] = {{7, 2}, {13, 4}};
    adjacencyList[11] = {{8, 7}, {14, 3}};
    adjacencyList[12] = {{9, 5}, {13, 6}};
    adjacencyList[13] = {{10, 4}, {12, 6}, {14, 2}};
    adjacencyList[14] = {{11, 3}, {13, 2}};

    // Find the shortest path between node 4 and node 14
    int source = 4;
    int target = 5;
    std::vector<int> path = shortestPath(adjacencyList, source, target);

    // Generate DOT file with a specific size, node shape, and highlighted path
    std::string dotFilename = "mygraph.dot";
    generateDotFile(adjacencyList, dotFilename, 800, 600, "ellipse", path); // Width: 800, Height: 600, Node Shape: ellipse

    // Generate graph image using Graphviz
    std::string outputImage = "mygraph.png";
    generateGraph(dotFilename, outputImage);

    // Create QLabel for displaying the image
    QLabel label;
    label.setPixmap(QPixmap(outputImage.c_str()));

    // Create a scroll area and set the label as its widget
    QScrollArea scrollArea;
    scrollArea.setWidget(&label);

    // Create a QVBoxLayout to manage the layout
    QVBoxLayout layout;
    layout.addWidget(&scrollArea); // Add the scroll area to the layout

    // Create a QWidget to serve as the main window
    QWidget window;
    window.setLayout(&layout); // Set the layout for the window
    window.setWindowTitle("Graph with Scroll Area"); // Set window title
    window.show(); // Show the window

    return app.exec();
}
