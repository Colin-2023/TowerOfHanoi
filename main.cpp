#include "mainwindow.h"
#include <QApplication>
#include <bits/stl_vector.h>
#include <iostream>
#include "diskstate.h"
#include <towermodel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}


