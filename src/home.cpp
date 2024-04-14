#include "home.h"
#include "ui_home.h"
#include"graph.h"
#include<QGraphicsView>
Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);
    Graph graph;
    graph.readGraphFile("C:\\Users\\ahmed\\Desktop\\my projects\\GuideMe\\GuideMe\\src\\Graph.txt");
    graph.printGraph();
    Node node = Node("Alex");
    std::vector<Node>v = graph.DFS(node);
    if(v.empty()){
        qDebug()<<"Error in getEdges";
    }else
    for(Node& n:v){
        qDebug()<<n.getNodeName().c_str()<<" and \n";
    }

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
