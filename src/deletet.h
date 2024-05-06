#ifndef DELETET_H
#define DELETET_H

#include <QDialog>
#include<graph.h>
#include <home.h>
namespace Ui {
class deletet;
}

class deletet : public QDialog
{
    Q_OBJECT

public:
    explicit deletet(Graph *graph, visualizeGraph *graphWid,QWidget *parent = nullptr);
    ~deletet();
    bool readFromFile(const QString& fileName, QStringList& data);


private slots:
    void on_pushButton_clicked();

private:
    Ui::deletet *ui;
    Graph *graph;
    visualizeGraph *graphWid;
};

#endif // DELETET_H
