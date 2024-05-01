 #ifndef ADDT_H
#define ADDT_H

#include <QDialog>
#include<graph.h>

namespace Ui {
class addT;
}

class addT : public QDialog
{
    Q_OBJECT


public:

    explicit addT(QWidget *parent = nullptr);
    ~addT();
    bool readFromFile(const QString& fileName, QStringList& data);
    Node qStringToNode(const QString& qstr);
private slots:
    void on_pushButton_clicked();


private:
    Ui::addT *ui;
};

#endif // ADDT_H
