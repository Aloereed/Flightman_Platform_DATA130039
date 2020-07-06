QT       += core gui
QT       += sql
QT       += network
android{
    QT += androidextras
}
QT += core-private
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QRibbon/QRibbon.cpp \
    add_price.cpp \
    addadmin.cpp \
    addairport.cpp \
    addcom.cpp \
    addfliarrange.cpp \
    addflight.cpp \
    addflight_passingairport.cpp \
    addseat.cpp \
    adduser.cpp \
    genarran.cpp \
    main.cpp \
    loginwindow.cpp \
    mainplatformwindow.cpp \
    modadmin.cpp \
    modairport.cpp \
    modcom.cpp \
    modfliarrange.cpp \
    modflight.cpp \
    moduser.cpp \
    querydialog.cpp \
    queryplugin.cpp \
    search_result.cpp \
    show_seat_a.cpp \
    stopover.cpp

HEADERS += \
    QRibbon/QRibbon.h \
    add_price.h \
    addadmin.h \
    addairport.h \
    addcom.h \
    addfliarrange.h \
    addflight.h \
    addflight_passingairport.h \
    addseat.h \
    adduser.h \
    genarran.h \
    loginwindow.h \
    mainplatformwindow.h \
    modadmin.h \
    modairport.h \
    modcom.h \
    modfliarrange.h \
    modflight.h \
    moduser.h \
    querydialog.h \
    queryplugin.h \
    search_result.h \
    show_seat_a.h \
    stopover.h

FORMS += \
    QRibbon/qribbon.ui \
    add_price.ui \
    addadmin.ui \
    addairport.ui \
    addcom.ui \
    addfliarrange.ui \
    addflight.ui \
    addflight_passingairport.ui \
    addseat.ui \
    adduser.ui \
    loginwindow.ui \
    mainplatformwindow.ui \
    modadmin.ui \
    modairport.ui \
    modcom.ui \
    modfliarrange.ui \
    modflight.ui \
    moduser.ui \
    querydialog.ui \
    queryplugin.ui \
    search_result.ui \
    show_seat_a.ui \
    stopover.ui

TRANSLATIONS += \
    platform_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    QRibbon/QRibbon.qrc \
    platform_zh_CN.qm \
    icon.png \
    qss/Aqua.qss \
    qss/ElegantDark.qss \
    png/about.png \
    png/exit.png \
    png/logout.png \
    png/qt.png


ANDROID_EXTRA_LIBS +=    \ # modify the path
        $$PWD/libmariadb.so


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android_sources

DISTFILES += \
    QRibbon/Resource/icon/add_bottom.png \
    QRibbon/Resource/icon/add_left.png \
    QRibbon/Resource/icon/add_right.png \
    QRibbon/Resource/icon/add_top.png \
    QRibbon/Resource/icon/arrowBottom.png \
    QRibbon/Resource/icon/branch-closed.png \
    QRibbon/Resource/icon/branch-end.png \
    QRibbon/Resource/icon/branch-more.png \
    QRibbon/Resource/icon/branch-open.png \
    QRibbon/Resource/icon/branch_close.png \
    QRibbon/Resource/icon/branch_open.png \
    QRibbon/Resource/icon/calendar_nextmonth.png \
    QRibbon/Resource/icon/calendar_prevmonth.png \
    QRibbon/Resource/icon/checkbox_checked.png \
    QRibbon/Resource/icon/checkbox_checked_disable.png \
    QRibbon/Resource/icon/checkbox_parcial.png \
    QRibbon/Resource/icon/checkbox_parcial_disable.png \
    QRibbon/Resource/icon/checkbox_unchecked.png \
    QRibbon/Resource/icon/checkbox_unchecked_disable.png \
    QRibbon/Resource/icon/downarrow.png \
    QRibbon/Resource/icon/eye.png \
    QRibbon/Resource/icon/eyeClosed.png \
    QRibbon/Resource/icon/eyeClosed0.png \
    QRibbon/Resource/icon/radiobutton_checked.png \
    QRibbon/Resource/icon/radiobutton_checked_disable.png \
    QRibbon/Resource/icon/radiobutton_unchecked.png \
    QRibbon/Resource/icon/radiobutton_unchecked_disable.png \
    QRibbon/Resource/icon/unknow.png \
    QRibbon/Resource/icon/uparrow.png \
    QRibbon/Resource/icon/vline.png \
    android_sources/AndroidManifest.xml \
    android_sources/png/icon.png





