#ifndef UNIVERS_H
#define UNIVERS_H

#include <QMainWindow>
#include <QFont>
#include <QVector>
#include "perso.h"
#include "histoire.h"
#include "../exec/exechistoire.h"
#include "../gen/genhistoire.h"
#include "../reglages.h"
#include <QMediaPlayer>
#include <QHash>

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
 * @brief Fenêtre principale du jeu et point de référence de base pour l'accès à tous les éléments des différentes histoires
 */
class Univers : public QMainWindow
{
    Q_OBJECT

protected:
    QJsonObject m_AvJson;

    // Un générateurt d'histoire par histoire (donc il peut y en avoir plusieurs par univers si on veut plusieurs histoires)
    QHash<QString, GenHistoire*> m_GensHistoire;

    // histoire actuellement exécutée si il y en a une
    Hist* m_Histoire = nullptr;

    // interfaces/widgets
    ExecHistoire* m_ExecHistoire = nullptr;
    IPerso* m_Perso;
    Ui::Univers *ui;

    float m_Duree;
    bool m_PersoAffiche = true; // colonne personnage affichée ou non

    void InstallerInterface();

public:
    explicit Univers(QWidget *parent = nullptr, ModeAffichage modeAffichage = ModeAffichage::ema_Jeu, bool persoAffiche = true);
    //explicit Univers(QString cheminAventure, ModeAffichage modeAffichage = ModeAffichage::ema_Jeu, QString firstEvt = "", QString premierEffet = "", QWidget *parent = nullptr);
    virtual ~Univers();

    void AfficherHistoire(QWidget *parent = nullptr);
    Hist* GenererUneHistoire(QString histoireId);
    virtual void LancerHistoire(QString idHistoire, QWidget *parent = nullptr, QString premierEvt = "", QString premierEffet = "", bool BarreDeCote = true);

    // cette fonction m'a l'air hautement dispensable => à virer si je me décide à remettre au propre le déroulement des actions
    bool LancerEvtEtOuEffetCourant();

    void NettoyageGenerateurs();

    const static QFont* BASE_FONT;
    const static QFont* TITRE_FONT;
    static QString CHEMIN;
    static Univers* ME;
    ModeAffichage m_ModeAffichage = ModeAffichage::ema_Details;

    // getters setters
    Hist* GetHistoire();
    ExecHistoire* GetExecHistoire();
    GenHistoire* GetGenHistoire(QString histoireId);
    IPerso* GetPersoInterface();
    void SetTypeEvt(TypeEvt typeEvt);
    TypeEvt GetTypeEvtActuel();

    void AjouterDuree(float duree);

    QMediaPlayer* m_Lecteur;
    Reglages m_Reglages;

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

    void RafraichirAffichage();

public slots:
    // déclenche l'effet de base si aucun n'a été spécifiquement choisi par l'utilisateur (si n'y avait qu'un suivant potentiel)
    void DeclencherEffetSuivant();
};

#endif // UNIVERS_H
