#include "loginwindow.h"
#include <QFile>
#include <QSettings>
#include <QDebug>
#include <QApplication>


extern QSettings settings;
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif


bool checkPermission(const QString &permission) {
#ifdef Q_OS_ANDROID
    QtAndroid::PermissionResult r = QtAndroid::checkPermission(permission);

    if(r != QtAndroid::PermissionResult::Granted) {
        QtAndroid::requestPermissionsSync(QStringList() << permission);
        r = QtAndroid::checkPermission(permission);

        if(r == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }

#endif
    return true;

}

QByteArray readTextFile(const QString &file_path) {
    QFile input_file(file_path);
    QByteArray input_data;

    if(input_file.open(QIODevice::Text | QIODevice::Unbuffered | QIODevice::ReadOnly)) {
        input_data = input_file.readAll();
        input_file.close();
        return input_data;
    } else {
        return QByteArray();
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
#ifdef Q_OS_ANDROID
    a.setOrganizationDomain("FDU/BigData");
    a.setOrganizationName("FD4021");
    a.setApplicationName("FlightMan Platform Manager");
    a.setApplicationDisplayName("FlightMan Platform Manager");
    a.setApplicationVersion("1.0");
#endif
    QApplication::addLibraryPath(QApplication::applicationDirPath());
    QApplication::addLibraryPath("./");
    checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    QString style_sheet = readTextFile(settings.value("Platform/theme", ":/qss/Aqua.qss").toString());
    a.setStyleSheet(style_sheet);
    loginwindow w;

    w.setWindowIcon(QIcon(":/icon.png"));
    w.show();
    settings.sync();

    return a.exec();
}
