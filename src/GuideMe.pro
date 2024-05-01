QT       += core gui openglwidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addt.cpp \
    deletet.cpp \
    graph.cpp \
    home.cpp \
    main.cpp \
    updatet.cpp \
    visualizegraph.cpp

HEADERS += \
    addt.h \
    deletet.h \
    graph.h \
    home.h \
    updatet.h \
    visualizegraph.h

FORMS += \
    addt.ui \
    deletet.ui \
    home.ui \
    updatet.ui


LIBS += -lopengl32


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
