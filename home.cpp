#include "home.h"
#include "ui_home.h"
#include"graph.h"
Home::Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Home)
{
    ui->setupUi(this);
    Graph g;


}
Home::~Home()
{
    delete ui;
}


void Home::btn_1_handle(){

    qDebug()<<"first function";
    // ui->textEdit->insertPlainText("hello there");
    ui->textEdit->setText("logged in successfully");
}

void Home::btn_1_handle2(){

    qDebug()<<"second function";
    ui->textEdit->setText("signed up successfully");

}

void Home::ADDnode(){
    // QString userInput = ui->textEdit->toPlainText();
    // int number = userInput.toInt();
}
