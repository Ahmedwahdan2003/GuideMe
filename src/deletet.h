#ifndef DELETET_H
#define DELETET_H

#include <QDialog>
#include<graph.h>

namespace Ui {
class deletet;
}

class deletet : public QDialog
{
    Q_OBJECT

public:
    explicit deletet(Graph *graph,QWidget *parent = nullptr);
    ~deletet();
    bool readFromFile(const QString& fileName, QStringList& data);


private slots:
    void on_pushButton_clicked();

private:
    Ui::deletet *ui;
    Graph *graph;

};

#endif // DELETET_H
