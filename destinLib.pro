#-------------------------------------------------
#
# Project created by QtCreator 2018-04-08T12:30:42
#
#-------------------------------------------------

QT       += core gui widgets multimedia
QT += sql

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


SOURCES += \
    execchoix.cpp \
    execeffet.cpp \
    execevt.cpp \
    execnoeud.cpp \
    genevt.cpp \
        main.cpp \
    perso.cpp \
    carac.cpp \
    aspectratiolabel.cpp \
    dbmanager.cpp \
    evt.cpp \
    evtaleatoire.cpp \
    effet.cpp \
    noeud.cpp \
    setcarac.cpp \
    choix.cpp \
    condition.cpp \
    glisseur.cpp \
    tyranides.cpp \
    genestealer.cpp \
    cultegenestealer.cpp \
    reinenorne.cpp \
    invasionkraken.cpp \
    reglages.cpp \
    theme.cpp \
    univers.cpp \
    selectionneurdevenement.cpp \
    genhistoire.cpp \
    exechistoire.cpp \
    histoire.cpp

HEADERS += \
    execchoix.h \
    execeffet.h \
    execevt.h \
    execnoeud.h \
    genevt.h \
    perso.h \
    carac.h \
    aspectratiolabel.h \
    dbmanager.h \
    evt.h \
    evtaleatoire.h \
    effet.h \
    noeud.h \
    setcarac.h \
    choix.h \
    condition.h \
    glisseur.h \
    tyranides.h \
    genestealer.h \
    cultegenestealer.h \
    reinenorne.h \
    invasionkraken.h \
    reglages.h \
    theme.h \
    univers.h \
    selectionneurdevenement.h \
    genhistoire.h \
    exechistoire.h \
    histoire.h

FORMS += \
    univers.ui \
    perso.ui \
    histoire.ui \
    carac.ui \
    evt.ui \
    effet.ui \
    choix.ui

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/release/ -ldestinwidgetsplugind
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/debug/ -ldestinwidgetsplugind
#else:unix: LIBS += -L$$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/ -ldestinwidgetsplugind
#
#INCLUDEPATH += $$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/debug
#DEPENDPATH += $$PWD/../build-DestinDesigner-Desktop_Qt_5_9_3_MSVC2017_64bit-Debug/debug

RESOURCES += \
    tyranides.qrc

DISTFILES += \
    Aventures/Warhammer 40000/Tyranides/EspritDeLaRuche.jpg \
    Aventures/Warhammer 40000/Tyranides/genestealer_attaque.jpg \
    Aventures/Warhammer 40000/Tyranides/Genestealers_atterissage.jpg \
    Aventures/Warhammer 40000/Tyranides/GenestealerSpaceHulk.jpg \
    Aventures/Warhammer 40000/Tyranides/icone_logements_détruits.jpg \
    Aventures/Warhammer 40000/Tyranides/Incursions_Tyranides.jpg \
    Aventures/Warhammer 40000/Tyranides/logements_détruits.jpg \
    Aventures/Warhammer 40000/Tyranides/MagusPortrait.jpg \
    Aventures/Warhammer 40000/Tyranides/Patriarche.jpg \
    Aventures/Warhammer 40000/Tyranides/PrimusPortrait.jpg \
    Aventures/Warhammer 40000/Tyranides/TeteCitoyen.jpg \
    Aventures/Warhammer 40000/Tyranides/TeteCultiste.jpg \
    Aventures/Warhammer 40000/Tyranides/TeteGenestealer.jpg \
    Aventures/Warhammer 40000/Tyranides/TeteHybride1.jpg \
    Aventures/Warhammer 40000/Tyranides/TeteHybride2.jpg \
    Aventures/Warhammer 40000/Tyranides/TeteHybride3.jpg \
    Aventures/Warhammer 40000/Tyranides/TeteHybride4.jpg \
    Aventures/Warhammer 40000/Tyranides/TeteInséminé.jpg \
    Aventures/Warhammer 40000/Tyranides/ruche_lamos.png \
    Aventures/Warhammer 40000/Tyranides/TeteCitoyen.jpg \
    Aventures/Warhammer 40000/Tyranides/entrepot.jpg \
    Aventures/Warhammer 40000/Tyranides/ichar4.jpg \
    Aventures/Warhammer 40000/Tyranides/larnarno.jpg \
    Aventures/Warhammer 40000/Tyranides/Miral.jpg \
    Aventures/Warhammer 40000/Tyranides/Salem.jpg
