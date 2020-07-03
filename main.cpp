#include "loginwindow.h"
#include <QFile>
#include <QSettings>
#include <QApplication>
extern QSettings settings;
QByteArray readTextFile(const QString &file_path) {
  QFile input_file(file_path);
  QByteArray input_data;

  if (input_file.open(QIODevice::Text | QIODevice::Unbuffered | QIODevice::ReadOnly)) {
    input_data = input_file.readAll();
    input_file.close();
    return input_data;
  }
  else {
    return QByteArray();
  }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath(QApplication::applicationDirPath());
    QApplication::addLibraryPath("./");
    QString style_sheet = readTextFile(settings.value("theme",":/qss/ElegantDark.qss");
    a.setStyleSheet(style_sheet);
    loginwindow w;

    w.setWindowIcon(QIcon(":/icon.png"));
    w.show();

    return a.exec();
}
