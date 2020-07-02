QT       += core gui
QT       += sql
QT       += network
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
    add_price.cpp \
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
    modairport.cpp \
    modcom.cpp \
    modfliarrange.cpp \
    modflight.cpp \
    moduser.cpp \
    querydialog.cpp \
    queryplugin.cpp \
    search_result.cpp \
    stopover.cpp

HEADERS += \
    add_price.h \
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
    modairport.h \
    modcom.h \
    modfliarrange.h \
    modflight.h \
    moduser.h \
    querydialog.h \
    queryplugin.h \
    search_result.h \
    stopover.h

FORMS += \
    add_price.ui \
    addairport.ui \
    addcom.ui \
    addfliarrange.ui \
    addflight.ui \
    addflight_passingairport.ui \
    addseat.ui \
    adduser.ui \
    loginwindow.ui \
    mainplatformwindow.ui \
    modairport.ui \
    modcom.ui \
    modfliarrange.ui \
    modflight.ui \
    moduser.ui \
    querydialog.ui \
    queryplugin.ui \
    search_result.ui \
    stopover.ui

TRANSLATIONS += \
    platform_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    platform_zh_CN.qm \
    style.qss

ANDROID_EXTRA_LIBS +=    \ # modify the path
        $$PWD/libmariadb.so


