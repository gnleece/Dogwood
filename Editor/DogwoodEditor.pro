#-------------------------------------------------
#
# Project created by QtCreator 2014-06-28T17:10:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DogwoodEditor
TEMPLATE = app


SOURCES += main.cpp\
        maineditorwindow.cpp \
        vectoredit.cpp \
    assetwidget.cpp

HEADERS  += maineditorwindow.h \
    meshwidget.h

FORMS    += maineditorwindow.ui \
    transformwidget.ui \
    vectorwidget.ui \
    scrollwidget.ui \
    meshwidget.ui \
    colorwidget.ui \
    assetwidget.ui
