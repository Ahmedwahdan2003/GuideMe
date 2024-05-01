#include "deletet.h"
#include "ui_deletet.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QDebug>


deletet::deletet(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::deletet)
{
    ui->setupUi(this);
}

deletet::~deletet()
{
    delete ui;
}
bool deletet::readFromFile(const QString& filepath, QStringList& data) {
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



void deletet::on_pushButton_clicked()
{
    QString sourceT = ui->lineEdit->text();
    QString destinationT = ui->lineEdit_2->text();
    QString Transportation = ui->lineEdit_3->text();
    QString filepath = "C:\\Users\\User\\OneDrive\\Desktop\\New folder (4)\\GuideMe\\src\\Graph.txt";
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
            // Iterate over parts starting from index 6
            for (int r = 6; r < parts.size(); r += 2) {
                if (sourceT == source && destinationT == destination && Transportation == parts[r]) {
                    qDebug() << "Found match for source, destination, and transportation at line: " << line;
                    // Remove the transportation and its cost
                    parts.removeAt(r); // Remove transportation
                    parts.removeAt(r); // Remove its cost
                    // Update the line with the modified parts
                    line = parts.join(' ');
                    // Update the fileData with the modified line
                    fileData[i] = line;
                    foundMatch = true;
                    break;
                }
            }
        }
    }

    // Check if a match was found and update the file
    if (foundMatch) {
        // Open the file in write mode
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

        qDebug() << "Data updated in file.";
    } else {
        qDebug() << "No match found for the specified source, destination, and transportation.";
    }

    close();
}

