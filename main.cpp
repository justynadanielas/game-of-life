#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    MainWindow m;
//    m.testujWszystko();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
