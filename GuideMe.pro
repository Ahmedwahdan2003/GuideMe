QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    graph.cpp \
    home.cpp \
    main.cpp

HEADERS += \
    graph.h \
    home.h

FORMS += \
    home.ui

INCLUDEPATH += "C:/Program Files/Graphviz/include"
LIBS += -lopengl32 -L"C:/Program Files/Graphviz/lib" -lgvc -lcgraph


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
