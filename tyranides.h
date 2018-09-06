#ifndef TYRANIDES_H
#define TYRANIDES_H

#include <QWidget>
#include "aventure.h"

/*namespace Ui {
class Tyranides;
}*/

enum HistoireTyranide {
    eh_CulteGenestealer,
    eh_FlotteKraken
};

class Tyranides : public Aventure
{
    Q_OBJECT
protected:
    // pour les aventrues qui n'utilisent pas le json mais du code :surclasser aventure et développer cette fonction
    virtual void GenererAventure();
    // génère toutes les caracs qui peuvent être visualisées par le joueur (d'autres caracs peuvent être générées et invisibles n'importe quand dans l'aventure)
    virtual void GenererCaracs();

public:
    explicit Tyranides(HistoireTyranide themeHistoire, QWidget *parent = nullptr, QString premierEvt = "", QString premierEffet = "");
    //~Tyranides();

private:
    //Ui::Tyranides *ui_Tyranides;
    HistoireTyranide m_HistoireTyranide;
};

#endif // TYRANIDES_H
