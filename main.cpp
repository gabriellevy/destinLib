#include <QApplication>
#include "tyranides.h"
#include <QTime>
#include <QtPlugin>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

   /* QTime maintenant = QTime::currentTime();
    qsrand(maintenant.msec());

    MainWindow* fenetre = new MainWindow();
    fenetre->show();*/

    // temp test :
    //QString cheminFichier = "D:/boulot/QtProjects/destinLib/Aventures/Warhammer 40000/Tyranides/Tyranides.json";
    //Aventure* av = new Aventure(cheminFichier, "Debut_Reproduction_Genestealer"/*, "exploration_repartition"*/ );
    Tyranides* av = new Tyranides(eh_FlotteKraken, nullptr, "DebutExploration", "reine_norne");
    av->show();

    return a.exec();
}
