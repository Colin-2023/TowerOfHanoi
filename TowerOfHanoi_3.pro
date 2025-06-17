QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    printparams.cpp \
    pseudo.cpp \
    sta.cpp \
    startgame.cpp \
    towermodel.cpp \
    towerpresenter.cpp

HEADERS += \
    dialog.h \
    disc.h \
    diskstate.h \
    mainwindow.h \
    printparams.h \
    pseudo.h \
    sta.h \
    startgame.h \
    towermodel.h \
    towerpresenter.h

FORMS += \
    dialog.ui \
    mainwindow.ui \
    pseudo.ui \
    startgame.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    background.qrc
