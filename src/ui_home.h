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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Home
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_1;
    QTextEdit *textEdit;
    QPushButton *btn_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Home)
    {
        if (Home->objectName().isEmpty())
            Home->setObjectName("Home");
        Home->resize(800, 600);
        centralwidget = new QWidget(Home);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        btn_1 = new QPushButton(centralwidget);
        btn_1->setObjectName("btn_1");

        verticalLayout->addWidget(btn_1);

        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(textEdit);

        btn_2 = new QPushButton(centralwidget);
        btn_2->setObjectName("btn_2");

        verticalLayout->addWidget(btn_2);

        Home->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Home);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        Home->setMenuBar(menubar);
        statusbar = new QStatusBar(Home);
        statusbar->setObjectName("statusbar");
        Home->setStatusBar(statusbar);

        retranslateUi(Home);
        QObject::connect(btn_1, SIGNAL(clicked()), Home, SLOT(btn_1_handle()));
        QObject::connect(btn_2, SIGNAL(clicked()), Home, SLOT(ADDnode()));

        QMetaObject::connectSlotsByName(Home);
    } // setupUi

    void retranslateUi(QMainWindow *Home)
    {
        Home->setWindowTitle(QCoreApplication::translate("Home", "Home", nullptr));
        btn_1->setText(QCoreApplication::translate("Home", "PushButton", nullptr));
        btn_2->setText(QCoreApplication::translate("Home", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Home: public Ui_Home {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOME_H
