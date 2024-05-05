 #ifndef ADDT_H
#define ADDT_H

#include <QDialog>
#include<graph.h>
#include<home.h>

namespace Ui {
class addT;
}

class addT : public QDialog
{
    Q_OBJECT


public:

     explicit addT(Graph *graph, QWidget *parent = nullptr);
    ~addT();
   // bool readFromFile(const QString& fileName, QStringList& data);
    Node qStringToNode(const QString& qstr);
private slots:
    void on_pushButton_clicked();


private:
    Ui::addT *ui;
     Graph *graph;

};

#endif // ADDT_H
