#ifndef HOME_H
#define HOME_H
#include <QMainWindow>
 #include "visualizegraph.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Home;
}
QT_END_NAMESPACE

class Home : public QMainWindow
{
    Q_OBJECT

public:
    visualizeGraph* graphWidget = new visualizeGraph(this);
    Graph* graph;
    Home(QWidget *parent = nullptr);
    ~Home();

private:
    Ui::Home *ui;
public slots:
    void btn_1_handle();
    void btn_1_handle2();
    void ADDnode();
     // void layoutGraph();


private slots:
    void on_Add_btn_clicked();
    void on_Update_btn_clicked();
    void on_Del_btn_clicked();
    void on_pushButton_clicked();
    void on_BFS_btn_clicked();
    void on_FD_btn_clicked();

};
#endif // HOME_H
