#ifndef UPDATET_H
#define UPDATET_H
#include<graph.h>
#include<home.h>
#include <QDialog>

namespace Ui {
class updatet;
}

class updatet : public QDialog
{
    Q_OBJECT

public:
    explicit updatet(Graph *graph,QWidget *parent = nullptr);
    ~updatet();
    bool readFromFile(const QString& fileName, QStringList& data);

private slots:
    void on_pushButton_clicked();


private:
    Ui::updatet *ui;
    Graph *graph;
};


#endif // UPDATET_H
