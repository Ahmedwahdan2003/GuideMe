? xml version = "1.0" encoding = "UTF-8" ? >
<ui version = "4.0">
<class>Home< / class>
<widget class = "QMainWindow" name = "Home">
<property name = "geometry">
<rect>
< x>0 < / x >
< y>0 < / y >
< width>1154 < / width >
< height>694 < / height >
< / rect>
< / property>
<property name = "windowTitle">
<string>Home< / string>
< / property>
<widget class = "QWidget" name = "centralwidget">
<widget class = "QWidget" name = "layoutWidget">
<property name = "geometry">
<rect>
< x>10 < / x >
< y>10 < / y >
< width>781 < / width >
< height>641 < / height >
< / rect>
< / property>
<layout class = "QHBoxLayout" name = "horizontalLayout">
<item>
<widget class = "QGraphicsView" name = "graphicsView" / >
< / item>
<item>
<layout class = "QVBoxLayout" name = "verticalLayout">
<item>
<widget class = "QPushButton" name = "BFS_btn">
<property name = "text">
<string>BFS< / string>
< / property>
< / widget>
< / item>
<item>
<widget class = "QPushButton" name = "DFS_btn">
<property name = "text">
<string>DFS< / string>
< / property>
< / widget>
< / item>
<item>
<widget class = "QPushButton" name = "Add_btn">
<property name = "text">
<string>Add< / string>
< / property>
< / widget>
< / item>
<item>
<widget class = "QPushButton" name = "Del_btn">
<property name = "text">
<string>Delete< / string>
< / property>
< / widget>
< / item>
<item>
<widget class = "QPushButton" name = "Update_btn">
<property name = "text">
<string>Update< / string>
< / property>
< / widget>
< / item>
< / layout>
< / item>
< / layout>
< / widget>
< / widget>
<widget class = "QStatusBar" name = "statusbar" / >
< / widget>
<resources / >
<connections>
<connection>
<sender>Add_btn< / sender>
<signal>clicked() < / signal >
<receiver>Home< / receiver>
<slot>btn_1_handle() < / slot >
<hints>
<hint type = "sourcelabel">
< x>984 < / x >
< y>417 < / y >
< / hint>
<hint type = "destinationlabel">
< x>1056 < / x >
< y>333 < / y >
< / hint>
< / hints>
< / connection>
< / connections>
<slots>
<slot>btn_1_handle() < / slot >
<slot>btn_2_handle() < / slot >
<slot>ADDnode() < / slot >
< / slots>
< / ui>