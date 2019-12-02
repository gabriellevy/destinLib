#ifndef EXECHISTOIRE_H
#define EXECHISTOIRE_H

#include "perso.h"

class ExecNoeud;
class ExecEffet;
class ExecEvt;
class ExecLancerDe;
class Effet;
class Hist;
class Noeud;
class Evt;

namespace Ui {
class Histoire;
}

/**
 * @brief Classe qui gère le déroulement et l'affichage de toute l'histoire du jeu
 */
class ExecHistoire : public QWidget
{
    Q_OBJECT

protected:

    Evt* m_DernierEvtAffiche = nullptr;
    Effet* m_DernierEffetAffiche = nullptr;

    int DeterminerIndexEvt(QString idEvt);
    int CalculerIndex(Evt* evtATrouver);

    // repérage d'événement courant et effet courant en mode histoire normal
    //QString m_CurrentEvtId;
    //int m_EffetIndex; // index de l'effet actuel dans la liste des effets de l'événement actuel
    ExecNoeud* m_ExecNoeudActuel = nullptr; // 'curseur' pointant sur le noeud précisément exécuté à ce moment, que ce soit un événement, effet, choix
    ExecEvt* m_ExecEvtActuel = nullptr; // evt actuel (il doit toujours y en avoir un à partir de quand le destin est lancé


    //QString m_CurrentConditionnelEvtId;
    //int m_EffetConditionnelIndex;

public:
    explicit ExecHistoire(Hist* histoire, QWidget *parent = nullptr);
    ~ExecHistoire();

    Hist* m_Histoire;

    virtual QString GetTitre();
    //Evt* m_CurrentEvt = nullptr;

    bool AppelerFonctionCallback(QString fonction, QVector<QString> caracs, QVector<QString> params);

    void PasserAEffetIndexSuivant();
    void PasserAEvtIndexSuivant();
    void GoToEffetId(QString idEffet);
    int DeterminerIndexEffet(QString idEffet);

    /**
     * @brief EvtActuel
     * @param forceHistoireMode : renvoie l'événement courant de l'histoire, excluant l'événement aléatoire secondaire même si on est en mode aléatoire
     * @return événement actuellement exécuté par le joueur
     */
    ExecEvt* GetExecEvtActuel(bool forceHistoireMode = false);
    static ExecEffet* GetExecEffetActuel(bool forceHistoireMode = false);
    ExecLancerDe* GetExecLancerDeActuel();
    Evt* EvtActuel(bool forceHistoireMode = false);
    Effet* EffetActuel(bool forceHistoireMode = false);
    ExecEvt* SetExecEvtActuel(Evt* evt);

    ExecNoeud* DeterminerPuisLancerNoeudSuivant(ExecNoeud* noeudActuel = nullptr, bool noeudActuelEstValide = true);
    Noeud* GetEffetDindexSuivant(Noeud* noeudActuel);
    Noeud* TesterSiEffetEstLancableOuSonElse(Noeud* noeudActuel);
    // si les événements sont issues de la bdd ils ont un id qui permet de les extraire :
    Evt* GetEvtSelonBddId(int id);
    Evt* GetEvtSelonId(QString idATrouver);
    bool AppliquerGoTo(Noeud* noeud);

    void RafraichirAffichageEvtEtOuEffet(Evt* evt, Effet* effet);

    void SetEffetIndex(int index);
    /**
     * @brief trouve l'événement correspondant aà l'id en paramètre et en fait le noeud actuel
     */
    void SetCurrentEvtId(QString);
    void AjouterDureeAEffetHistoireCourant(float duree);

    void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    static Effet* GetEffetActuel();

private:
    Ui::Histoire *ui;
};

#endif // EXECHISTOIRE_H
