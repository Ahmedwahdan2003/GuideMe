/********************************************************************************
** Form generated from reading UI file 'home.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOME_H
#define UI_HOME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Home
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Home)
    {
        if (Home->objectName().isEmpty())
            Home->setObjectName("Home");
        Home->resize(1122, 694);
        centralwidget = new QWidget(Home);
        centralwidget->setObjectName("centralwidget");
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(930, 600, 181, 71));
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(-1, -1, 881, 731));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(930, 500, 181, 71));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(930, 400, 181, 71));
        Home->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Home);
        statusbar->setObjectName("statusbar");
        Home->setStatusBar(statusbar);

        retranslateUi(Home);
        QObject::connect(pushButton_3, SIGNAL(clicked()), Home, SLOT(btn_1_handle()));

        QMetaObject::connectSlotsByName(Home);
    } // setupUi

    void retranslateUi(QMainWindow *Home)
    {
        Home->setWindowTitle(QCoreApplication::translate("Home", "Home", nullptr));
        pushButton->setText(QCoreApplication::translate("Home", "Update", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Home", "Delete", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Home", "Add", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Home: public Ui_Home {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOME_H
