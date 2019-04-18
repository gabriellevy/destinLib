#ifndef HISTOIRE_H
#define HISTOIRE_H

#include "evt.h"
#include "evtaleatoire.h"
#include "carac.h"
#include <QVector>
#include "theme.h"
#include "dbmanager.h"
#include "perso.h"

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
    /**
     * @brief fonctions spéciales associées à cette histoire et appellables par les noeuds au runtime
     * Elle ont un identifiant unique (égal à leur nom) qui permet de les référence en base de données par exemple
     * le premier paramètre QVector<QString> correspond à l'id des carac qu'on doit passer à la fonction en paramètre
     * le 2ème correspond à des valeurs "brutes"
     */
    QMap<QString, std::function<bool(QVector<QString>, QVector<QString>)>> m_CallbackFunctions;

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

    // gestion de la BDD :
    virtual void ChargerEvtsBdd();

public:
    explicit Histoire(QWidget *parent = nullptr);
    ~Histoire();

    // pour les aventrues qui n'utilisent pas le json mais du code :surclasser aventure et développer ces fonction

    virtual void GenererHistoire() = 0;
    virtual void GenererPersos() = 0;
    virtual void GenererThemes() = 0;
    virtual void GenererFonctionsCallback() = 0; // cette fonction a de bonnes chances d'être vides. Je la laisse en abstraite à implémenter comme pense-bête
    virtual QString GetTitre() = 0;
    Evt* m_CurrentEvt = nullptr;

    /**
     * @brief charge le contenu de la bdd visée dans l'histoire
     * @param cheminBDD
     *
     * Il est tout à fait possible que les fonctions GenererHistoire et/ou GenererPersos
     * soient remplacées par des fonctions de ce genre si toutes les informations de l'histoire sont en bdd
     */
    virtual void ChargerBDD(QString cheminBDD);

    bool AppelerFonctionCallback(QString fonction, QVector<QString> caracs, QVector<QString> params);

    void AppliquerTheme(Theme* theme);

    //void Generer(QJsonObject aventure);
    //QVector<QString> m_Themes;


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
    // si les événements sont issues de la bdd ils ont un id qui permet de les extraire :
    Evt* GetEvtSelonBddId(int id);
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

    // caracs actuelles du joueur
    QVector<Carac*> m_Caracs;

    QVector<Theme*> m_Themes;

    DbManager m_Db;

    bool CetteCaracExisteDeja(QString id);
    void AppliquerCarac(SetCarac setCarac);

    DPerso* GetPersoCourant();

    /**
     * @brief GetCaracValue
     * @return valeur de cette carac pour le perso. "" si cette carac n'existe pas
     */
    QString GetCaracValue(QString caracId);

    void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    void AnnulerResultatsDeTests(Noeud* saufCeluiLa);

private:
    Ui::Histoire *ui;
};

#endif // HISTOIRE_H
