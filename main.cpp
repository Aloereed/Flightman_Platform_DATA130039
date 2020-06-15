#include "loginwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath(QApplication::applicationDirPath());
    QApplication::addLibraryPath("./");
    loginwindow w;
    w.show();
    return a.exec();
}
