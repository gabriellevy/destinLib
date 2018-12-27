#ifndef UNIVERS_H
#define UNIVERS_H

#include <QMainWindow>
#include <QFont>
#include "perso.h"
#include "histoire.h"
#include "reglages.h"
#include <QMediaPlayer>

namespace Ui {
class Univers;
}

enum EtatPartie {
    EP_Chargement,
    EP_Deroulement,
    EP_FinPartie
};

enum ModeAffichage {
    ema_Jeu, // mode de base
    ema_Details // un genre de mode debug visual avec les titres visibles et toutes les caracs affichées
};

/**
 * @brief Fenêtre principale du jeu
 */
class Univers : public QMainWindow
{
    Q_OBJECT

protected:
    QJsonObject m_AvJson;
    EtatPartie m_EtatPartie;
    //bool ExtraireAventure(QString cheminAventure);
    //void LancerAventure(QString premierEvt = "", QString premierEffet = "");

    // interfaces/widgets
    Histoire* m_Histoire;
    IPerso* m_Perso;
    float m_Duree;
    Ui::Univers *ui;

    void InstallerInterface();

    // pour les aventrues qui n'utilisent pas le json mais du code :surclasser aventure et développer cette fonction
    virtual void GenererAventure() = 0;
    virtual void GenererCaracs() = 0;

public:
    explicit Univers(QWidget *parent = nullptr, ModeAffichage modeAffichage = ModeAffichage::ema_Jeu, bool barreDeCote = true);
    //explicit Univers(QString cheminAventure, ModeAffichage modeAffichage = ModeAffichage::ema_Jeu, QString firstEvt = "", QString premierEffet = "", QWidget *parent = nullptr);
    ~Univers();

    bool LancerEvtEtOuEffetCourant();

    const static QFont* BASE_FONT;
    const static QFont* TITRE_FONT;
    static QString CHEMIN;
    static Univers* ME;
    ModeAffichage m_ModeAffichage = ModeAffichage::ema_Details;

    Histoire* GetHistoire();
    IPerso* GetPersoInterface();
    void SetEtatPartie(EtatPartie etat);
    void SetTypeEvt(TypeEvt typeEvt);
    TypeEvt GetTypeEvtActuel();
    EtatPartie GetEtatPartie();
    void AjouterDuree(float duree);
    bool EstEnModeHistoire();

    EtatPartie ChangerEtatPartie(QString nouvelEtatPartie);

    QMediaPlayer* m_Lecteur;
    Reglages m_Reglages;

    /**
     * @brief AppliquerFond
     * @param urlImageFond : chemin vers l'image à utiliser en fond. Exemple : ':/images/fond.jpg'
     */
    void AppliquerFond(QString urlImageFond);


    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

public slots:
    // déclenche l'effet de base si aucun n'a été spécifiquement choisi par l'utilisateur (si n'y avait qu'un suivant potentiel)
    void DeclencherEffetSuivant();
};

#endif // UNIVERS_H
