#ifndef UNIVERS_H
#define UNIVERS_H

#include <QMainWindow>
#include <QFont>
#include <QVector>
#include "theme.h"
#include "perso.h"
#include "histoire.h"
#include "exechistoire.h"
#include "genhistoire.h"
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

    // utiliser le générateur de base éventuellement mais en général il faudra le surclasser je pense
    GenHistoire* m_GenHistoire = nullptr;
    // interfaces/widgets
    Hist* m_Histoire = nullptr;
    ExecHistoire* m_ExecHistoire = nullptr;
    IPerso* m_Perso;
    float m_Duree;
    Ui::Univers *ui;

    void InstallerInterface();

public:
    explicit Univers(QWidget *parent = nullptr, ModeAffichage modeAffichage = ModeAffichage::ema_Jeu);
    //explicit Univers(QString cheminAventure, ModeAffichage modeAffichage = ModeAffichage::ema_Jeu, QString firstEvt = "", QString premierEffet = "", QWidget *parent = nullptr);
    virtual ~Univers();

    void AfficherHistoire(QWidget *parent = nullptr);
    virtual Hist* ExecuterGenerateurHistoire();
    virtual void LancerHistoire(Hist* histoire, QWidget *parent = nullptr, QString premierEvt = "", QString premierEffet = "", bool BarreDeCote = true);

    bool LancerEvtEtOuEffetCourant();

    const static QFont* BASE_FONT;
    const static QFont* TITRE_FONT;
    static QString CHEMIN;
    static Univers* ME;
    ModeAffichage m_ModeAffichage = ModeAffichage::ema_Details;

    Hist* GetHistoire();
    ExecHistoire* GetExecHistoire();
    GenHistoire* GetGenHistoire();
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

    QVector<Theme*> m_Themes;

    /**
     * @brief AppliquerFond
     * @param urlImageFond : chemin vers l'image à utiliser en fond. Exemple : ':/images/fond.jpg'
     */
    void AppliquerFond(QString urlImageFond);

    /**
     * @brief AppliquerTheme
     * @param fond
     * Un de ces jours il faudra créer un objet thème à appliquer à une fenêtre, pour l'instant c'est du bricolage
     */
    void AppliquerTheme(QColor fond);

    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

public slots:
    // déclenche l'effet de base si aucun n'a été spécifiquement choisi par l'utilisateur (si n'y avait qu'un suivant potentiel)
    void DeclencherEffetSuivant();
};

#endif // UNIVERS_H
