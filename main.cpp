#include "dirwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DirWindow w;
    w.show();

    return a.exec();
}
