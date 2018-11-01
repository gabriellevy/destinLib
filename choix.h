#ifndef CHOIX_H
#define CHOIX_H

#include <QPushButton>
#include "noeud.h"

namespace Ui {
class Choix;
}

/**
 * @brief The Choix class
 * dérive des noeuds tout comme les effets mais est un bouton et a comme principale particularité que l'exécution des actions n'est
 * faite que quand on appuie sur le bouton de même que la transition. Ils sont donc complètement désolidarisés de l'affichage contrairement
 * à la plupart des noeuds
 */
class Choix : public QPushButton, public Noeud
{
    Q_OBJECT

public:
    //explicit Choix(QJsonObject choixJson, QWidget *parent = nullptr);
    explicit Choix(QString text, QString cheminImg = "", QWidget *parent = nullptr);
    ~Choix();
    void AfficherNoeud();
    /**
     * @brief AQuelqueChoseAAfficher
     * @return false car les noeuds choix ne sont jamais affichés en tant que tels (seulement comme boutons en bas d'un autre noeud)
     */
    virtual bool AQuelqueChoseAAfficher();

    void LancerNoeud();
    bool GestionTransition();


private:
    Ui::Choix *ui;

public slots:
    void ExecuterNoeudSlot(/*bool afficherNoeud = true, bool lancerNoeudSuivantSiRienAAfiicher = true*/);

};

#endif // CHOIX_H
