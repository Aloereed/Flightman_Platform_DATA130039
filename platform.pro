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
    addairport.cpp \
    addcom.cpp \
    addflight.cpp \
    addflight_passingairport.cpp \
    addseat.cpp \
    adduser.cpp \
    main.cpp \
    loginwindow.cpp \
    mainplatformwindow.cpp \
    modairport.cpp \
    modcom.cpp \
    modflight.cpp \
    moduser.cpp \
    querydialog.cpp \
    queryplugin.cpp \
    search_result.cpp \
    stopover.cpp

HEADERS += \
    addairport.h \
    addcom.h \
    addflight.h \
    addflight_passingairport.h \
    addseat.h \
    adduser.h \
    loginwindow.h \
    mainplatformwindow.h \
    modairport.h \
    modcom.h \
    modflight.h \
    moduser.h \
    querydialog.h \
    queryplugin.h \
    search_result.h \
    stopover.h

FORMS += \
    addairport.ui \
    addcom.ui \
    addflight.ui \
    addflight_passingairport.ui \
    addseat.ui \
    adduser.ui \
    loginwindow.ui \
    mainplatformwindow.ui \
    modairport.ui \
    modcom.ui \
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

DISTFILES += \
    platform_zh_CN.qm \
    style.qss

win32 {
COPY_DEST = $$replace(OUT_PWD, /, \\)
SRC_DEST = $$replace(PWD, /, \\)
system("copy /y $$SRC_DEST\\style.qss $$COPY_DEST\\$$compiled\\style.qss")
system("copy /y $$SRC_DEST\\platform_zh_CN.qm $$COPY_DEST\\$$compiled\\platform_zh_CN.qm")
system("copy /y  $$SRC_DEST\\style.qss $$COPY_DEST\\$$compiled\\debug\\style.qss")
system("copy /y $$SRC_DEST\\platform_zh_CN.qm $$COPY_DEST\\$$compiled\\debug\\platform_zh_CN.qm")
system("copy /y $$SRC_DEST\\style.qss $$COPY_DEST\\$$compiled\\release\\style.qss")
system("copy /y $$SRC_DEST\\platform_zh_CN.qm $$COPY_DEST\\$$compiled\\release\\platform_zh_CN.qm")
}

mac {
system("cp  $$PWD/style.qss $$OUT_PWD/$$compiled/platform.app/Contents/MacOS/style.qss")
system("cp $$PWD/platform_zh_CN.qm $$OUT_PWD/$$compiled/platform.app/Contents/MacOS/platform_zh_CN.qm")
}

