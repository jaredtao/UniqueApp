#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "UniqueApp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UniqueApp uapp;

    MainWindow w;
    uapp.setNewAppCallback([&](){
        qWarning() << "raise";
        w.activateWindow();
    });

    w.show();

    return a.exec();
}
