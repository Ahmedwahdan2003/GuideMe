#ifndef HOME_H
#define HOME_H
#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui {
class Home;
}
QT_END_NAMESPACE

class Home : public QMainWindow
{
    Q_OBJECT

public:


  Home(QWidget *parent = nullptr);
    ~Home();

private:
    Ui::Home *ui;


public slots:
    void btn_1_handle();
    void btn_1_handle2();
    void ADDnode();
     // void layoutGraph();


};
#endif // HOME_H
