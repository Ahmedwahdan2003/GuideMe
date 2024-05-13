QT += core gui openglwidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Your other configurations

SOURCES += \
    addt.cpp \
    bfswidget.cpp \
    deletet.cpp \
    dfswidget.cpp \
    fdwidget.cpp \
    graph.cpp \
    home.cpp \
    main.cpp \
    updatet.cpp \
    visualizegraph.cpp

HEADERS += \
    addt.h \
    bfswidget.h \
    deletet.h \
    dfswidget.h \
    fdwidget.h \
    graph.h \
    home.h \
    updatet.h \
    visualizegraph.h

FORMS += \
    addt.ui \
    bfswidget.ui \
    deletet.ui \
    dfswidget.ui \
    fdwidget.ui \
    home.ui \
    updatet.ui

LIBS += -lopengl32

RESOURCES += resources
