#include "addt.h"
#include "graph.h"
#include "ui_addt.h"
//#include "graph_view.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>


addT::addT(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addT)
{
    ui->setupUi(this);
}

addT::~addT()
{
    delete ui;
}
bool addT::readFromFile(const QString& filepath, QStringList& data) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file: " << filepath;
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        data.append(line); // Add each line to the data list
    }

    file.close();
    return true;
}


void addT::on_pushButton_clicked()
{
    QString text1 = ui->lineEdit->text();
    QString text2 = ui->lineEdit_2->text();
    QString text3 = ui->lineEdit_3->text();
    QString text4 = ui->lineEdit_4->text();

    QString filepath = "C:\\Users\\User\\OneDrive\\Desktop\\New folder (4)\\GuideMe\\src\\Graph.txt"; // Replace with your file path
    QStringList fileData;
    if (!readFromFile(filepath, fileData)) {
        qDebug() << "Failed to read data from file: " << filepath;
        return;
    }

    bool foundMatch = false; // Flag to track if a match is found
    for (int i = 0; i < fileData.size(); ++i) {
        QString line = fileData[i];
        QStringList parts = line.split(' ');
        if (parts.size() >= 2) {
            QString source = parts[0];
            QString destination = parts[1];

            if (text1 == source && text2 == destination) {
                qDebug() << "Found match for text1 and text2 at line: " << line;
                // Modify the line to include text3 and text4
                line += " " + text3 + " " + text4;
                fileData[i] = line; // Update the line in the file data
                foundMatch = true;
                break; // No need to continue searching if a match is found
            }



        }
    }

    if (foundMatch) {
        Graph obj;
        int transportationInt = text4.toInt();
        Transportation transportation(text3, transportationInt);
        obj.addEdge( text1 , text2, transportation);
        // Open the file in write mode
        QFile file(filepath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing: " << filepath;
            return;
        }

        // Write updated file data to the file
        QTextStream out(&file);
        for (const QString& line : fileData) {
            out << line<< Qt::endl;
        }

        // Close the file
        file.close();

        qDebug() << "Data added to file.";
    } else {

        //QString newLine = text1 + " " + text2 + " "  +0  + " " + 0+ " "+  " "  +0  + " " + 0+ " "+text3 + " " + text4;
        QString newLine = text1 + " " + text2 + " " + "0" + " " + "0" + " " + "0" + " " + "0" + " " + text3 + " " + text4;

        fileData.append(newLine);
        Graph obj;
        int transportationInt = text4.toInt();
        Transportation transportation(text3, transportationInt);
        obj.addEdge( text1 , text2, transportation);
        QFile file(filepath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing: " << filepath;
            return;
        }

        // Write updated file data to the file
        QTextStream out(&file);
        for (const QString& line : fileData) {
            out << line << Qt::endl;
        }

        // Close the file
        file.close();

        qDebug() << "Data added to file.";

    }

    close();
}


