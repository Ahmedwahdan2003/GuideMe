#include "updatet.h"
#include "ui_updatet.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>



updatet::updatet(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::updatet)
{
    ui->setupUi(this);
}

updatet::~updatet()
{
    delete ui;
}
bool updatet::readFromFile(const QString& filepath, QStringList& data) {
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

void updatet::on_pushButton_clicked()
{

    QString source = ui->lineEdit->text();
    QString destination = ui->lineEdit_2->text();
    QString oldTransportation = ui->lineEdit_3->text();
    QString newTransportation = ui->lineEdit_4->text();
    int newCost = ui->lineEdit_5->text().toInt(); // Assuming the cost is entered as an integer

    QString filepath = "C:\\Users\\User\\OneDrive\\Desktop\\New folder (4)\\GuideMe\\src\\Graph.txt";
    QStringList fileData;
    if (!readFromFile(filepath, fileData)) {
        qDebug() << "Failed to read data from file: " << filepath;
        return;
    }

    bool updated = false;
    for (int i = 0; i < fileData.size(); ++i) {
        QString line = fileData[i];
        QStringList parts = line.split(' ');
        if (parts.size() >= 4) {
            QString currentSource = parts[0];
            QString currentDestination = parts[1];
            QString currentTransportation ;
            for (int r=6;r<fileData.size();r+=2)
        {
                if (source == currentSource && destination == currentDestination) {
                    // Find the index of the old transportation within the parts list
                    int index = parts.indexOf(oldTransportation);
                    if (index != -1 && index + 1 < parts.size()) {
                        // Update the transportation and its cost
                        parts[index] = newTransportation;
                        parts[index + 1] = QString::number(newCost);

                        // Join the modified parts back into a string
                        fileData[i] = parts.join(' ');

                        updated = true;
                        break; // No need to continue searching if a match is found
                    }  }
            }
          }
      }



    if (updated) {
        // Write updated data back to the file
        QFile file(filepath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing: " << filepath;
            return;
        }

        QTextStream out(&file);
        for (const QString& line : fileData) {
            out << line << '\n';
        }

        file.close();

        qDebug() << "Transportation updated successfully.";
    } else {
        qDebug() << "No matching source, destination, and old transportation found.";
    }
    close();
}

