#ifndef HISTOIRE_H
#define HISTOIRE_H

#include "evt.h"
#include "evtaleatoire.h"
#include "carac.h"
#include <QVector>
#include "theme.h"
#include "perso.h"

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
    Noeud* m_NoeudActuel = nullptr;

    //QString m_CurrentConditionnelEvtId;
    //int m_EffetConditionnelIndex;

public:
    explicit ExecHistoire(QWidget *parent = nullptr);
    ~ExecHistoire();

    QVector<Evt*> m_Evts;// événements de base (aventure elle-même)
    QVector<Evt*> m_EvtsConditionnels; // événements déclenchés automatiquement dès qu'on remplit leurs conditions
    QVector<Evt*> m_EvtsAleatoires; // événements qui peuvent être appelés par des effets particuliers nécessitant des événements aléatoires durant une certaine période

    /**
     * @brief fonctions spéciales associées à cette histoire et appellables par les noeuds au runtime
     * Elle ont un identifiant unique (égal à leur nom) qui permet de les référence en base de données par exemple
     * le premier paramètre QVector<QString> correspond à l'id des carac qu'on doit passer à la fonction en paramètre
     * le 2ème correspond à des valeurs "brutes"
     */
    QMap<QString, std::function<bool(QVector<QString>, QVector<QString>)>> m_CallbackFunctions;

    virtual QString GetTitre();
    //Evt* m_CurrentEvt = nullptr;

    bool AppelerFonctionCallback(QString fonction, QVector<QString> caracs, QVector<QString> params);

    void AppliquerTheme(Theme* theme);

    void PasserAEffetIndexSuivant();
    void PasserAEvtIndexSuivant();
    void GoToEffetId(QString idEffet);
    int DeterminerIndexEffet(QString idEffet);

    /**
     * @brief EvtActuel
     * @param forceHistoireMode : renvoie l'événement courant de l'histoire, excluant l'événement aléatoire secondaire même si on est en mode aléatoire
     * @return événement actuellement exécuté par le joueur
     */
    Evt* EvtActuel(bool forceHistoireMode = false);
    Effet* EffetActuel(bool forceHistoireMode = false);

    Noeud* DeterminerPuisLancerNoeudSuivant(Noeud* noeudActuel = nullptr, bool noeudActuelEstValide = true);
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
    /**
     * @brief GetIndexEffetConcerne
     * @return référence vers l'index d'effet normal ou aléatoire selon le mode dans lequel on se trouve à ce moment
     */
    //int& GetIndexEffetConcerne();

    // caracs actuelles du joueur
    QVector<Carac*> m_Caracs;

    QVector<Theme*> m_Themes;

    bool CetteCaracExisteDeja(QString id);
    void AppliquerCarac(SetCarac setCarac);

    DPerso* GetPersoCourant();

    /**
     * @brief GetCaracValue
     * @return valeur de cette carac pour le perso. "" si cette carac n'existe pas
     */
    QString GetCaracValue(QString caracId);

    void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

private:
    Ui::Histoire *ui;
};

#endif // HISTOIRE_H
