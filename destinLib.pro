#-------------------------------------------------
#
# Project created by QtCreator 2018-04-08T12:30:42
#
#-------------------------------------------------

QT += core gui sql widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DestinQt2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(gen/gen.pri)
include(abs/abs.pri)
include(exec/exec.pri)

SOURCES += \
    gestionnairecarac.cpp \
    main.cpp \
    aspectratiolabel.cpp \
    dbmanager.cpp \
    glisseur.cpp \
    reglages.cpp \
    aleatoire.cpp

HEADERS += \
    gestionnairecarac.h \
    aspectratiolabel.h \
    dbmanager.h \
    glisseur.h \
    reglages.h \
    aleatoire.h

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/release/ -ldestinwidgetsplugind
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/debug/ -ldestinwidgetsplugind
#else:unix: LIBS += -L$$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/ -ldestinwidgetsplugind
#
#INCLUDEPATH += $$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/debug
#DEPENDPATH += $$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/debug

