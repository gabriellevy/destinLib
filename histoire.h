#ifndef HISTOIRE_H
#define HISTOIRE_H

#include "evt.h"
#include "evtaleatoire.h"
#include "carac.h"

namespace Ui {
class Histoire;
}

/**
 * @brief Classe qui gère le déroulement et l'affichage de toute l'histoire du jeu
 */
class Histoire : public QWidget
{
    Q_OBJECT

protected:
    QVector<Evt*> m_Evts;// événements de base (aventure elle-même)
    QVector<Evt*> m_EvtsConditionnels; // événements déclenchés automatiquement dès qu'on remplit leurs conditions
    QVector<Evt*> m_EvtsAleatoires; // événements qui peuvent être appelés par des effets particuliers nécessitant des événements aléatoires durant une certaine période

    Evt* m_DernierEvtAffiche = nullptr;
    Effet* m_DernierEffetAffiche = nullptr;

    int DeterminerIndexEvt(QString idEvt);

    // repérage d'événement courant et effet courant en mode histoire normal
    QString m_CurrentEvtId;
    int m_EffetIndex; // index de l'effet actuel dans la liste des effets de l'événement actuel

    QString m_CurrentConditionnelEvtId;
    int m_EffetConditionnelIndex;

public:
    explicit Histoire(QWidget *parent = nullptr);
    ~Histoire();

    // pour les aventrues qui n'utilisent pas le json mais du code :surclasser aventure et développer ces fonction
    virtual void GenererHistoire() = 0;
    virtual void GenererPersos() = 0;
    Evt* m_CurrentEvt = nullptr;

    //void Generer(QJsonObject aventure);
    QVector<QString> m_Themes;


    int DeterminerIndexEffet(QString idEffet);

    /**
     * @brief EvtActuel
     * @param forceHistoireMode : renvoie l'événement courant de l'histoire, excluant l'événement aléatoire secondaire même si on est en mode aléatoire
     * @return événement actuellement exécuté par le joueur
     */
    Evt* EvtActuel(bool forceHistoireMode = false);
    Effet* EffetActuel(bool forceHistoireMode = false);

    // déclenche l'effet suivant de base si aucun n'a été spécifiquement choisi par l'utilisateur (qu'il n'y avait qu'un suivant potentiel)
    Noeud* DeterminerPuisLancerEffetSuivant(Noeud* noeudActuel = nullptr);
    Noeud* GetEffetDindexSuivant(Noeud* noeudActuel);
    Noeud* TesterSiEffetEstLancableOuSonElse(Noeud* noeudActuel);
    bool AppliquerGoTo(Noeud* noeud);

    void RafraichirAffichageEvtEtOuEffet(Evt* evt, Effet* effet);

    void SetEffetIndex(int index);
    void SetCurrentEvtId(QString);
    void AjouterDureeAEffetHistoireCourant(float duree);
    /**
     * @brief GetIndexEffetConcerne
     * @return référence vers l'index d'effet normal ou aléatoire selon le mode dans lequel on se trouve à ce moment
     */
    int& GetIndexEffetConcerne();

    Evt* AjouterEvt(QString id, QString nom);
    EvtAleatoire* AjouterEvtAleatoire(QString id, QString nom);

    // caracs :
    QVector<Carac*> m_Caracs;

    bool CetteCaracExisteDeja(QString id);
    void AppliquerCarac(SetCarac setCarac);

    /**
     * @brief GetCaracValue
     * @return valeur de cette carac pour le perso. "" si cette carac n'existe pas
     */
    QString GetCaracValue(QString caracId);

private:
    Ui::Histoire *ui;
};

#endif // HISTOIRE_H
