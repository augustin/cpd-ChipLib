#include "MainWin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWin w;
    w.showMaximized();

    return a.exec();
}
