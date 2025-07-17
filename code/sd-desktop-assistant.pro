QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adddaydialog.cpp \
    addmemodialog.cpp \
    databasemanager.cpp \
    importantdaymanager.cpp \
    main.cpp \
    managedaydialog.cpp \
    managememodialog.cpp \
    memomanager.cpp \
    tipsupdate.cpp \
    weatherupdate.cpp \
    widget.cpp \
    widgetlayout.cpp

HEADERS += \
    adddaydialog.h \
    addmemodialog.h \
    databasemanager.h \
    importantdaymanager.h \
    managedaydialog.h \
    managememodialog.h \
    memomanager.h \
    tipsupdate.h \
    weatherupdate.h \
    widget.h \
    widgetlayout.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc \
    resources.qrc
