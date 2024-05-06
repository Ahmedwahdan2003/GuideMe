 #ifndef ADDT_H
#define ADDT_H

#include <QDialog>
#include<graph.h>
#include<home.h>
#include<visualizegraph.h>
namespace Ui {
class addT;
}

class addT : public QDialog
{
    Q_OBJECT


public:

     explicit addT(Graph *graph, visualizeGraph *graphWid, QWidget *parent = nullptr);
    ~addT();
   // bool readFromFile(const QString& fileName, QStringList& data);
    Node qStringToNode(const QString& qstr);
private slots:
    void on_pushButton_clicked();


private:
    Ui::addT *ui;
    Graph *graph;
    visualizeGraph *graphWid;
};

#endif // ADDT_H
