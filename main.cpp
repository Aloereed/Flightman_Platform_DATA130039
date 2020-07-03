#include "loginwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath(QApplication::applicationDirPath());
    QApplication::addLibraryPath("./");
    loginwindow w;
    w.setWindowIcon(QIcon(":/icon.png"));
    w.show();
    return a.exec();
}
