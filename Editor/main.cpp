#include "maineditorwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainEditorWindow w;
    w.show();

    return a.exec();
}