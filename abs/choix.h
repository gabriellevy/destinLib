#ifndef CHOIX_H
#define CHOIX_H

#include <QPushButton>
#include "../exec/execnoeud.h"
#include "noeudnarratif.h"

namespace Ui {
class Choix;
}

class Effet;
class ExecEffet;

/**
 * @brief dérive des noeuds tout comme les effets mais est un bouton et a comme principale particularité que l'exécution des actions n'est
 * faite que quand on appuie sur le bouton de même que la transition. Ils sont donc complètement désolidarisés de l'affichage contrairement
 * à la plupart des noeuds
 *
 * Les choix appartiennt toujours à un effet. Ils sont affichés en bas de cet effet
 */
class Choix : public NoeudNarratif
{
protected:

public:
    explicit Choix(QString text="", QString cheminImg = "");
    virtual ~Choix() {}

    /**
     * @return false car les noeuds choix ne sont jamais affichés en tant que tels (seulement comme boutons en bas d'un autre noeud)
     */
    virtual bool AQuelqueChoseAAfficher();

    // gestion de la bdd :
    int m_BDD_ChoixId = -1;

    //QString m_Text;
    QString m_CheminImg;

    friend class GenEvt;
    friend class ExecLancerDe;

};

#endif // CHOIX_H
