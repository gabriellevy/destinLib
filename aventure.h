#ifndef AVENTURE_H
#define AVENTURE_H

#include <QMainWindow>
#include <QFont>
#include "perso.h"
#include "histoire.h"

namespace Ui {
class Aventure;
}

enum EtatPartie {
    EP_Chargement,
    EP_Deroulement,
    EP_FinPartie
};

/**
 * @brief Fenêtre principale du jeu
 */
class Aventure : public QMainWindow
{
    Q_OBJECT

protected:
    QJsonObject m_AvJson;
    EtatPartie m_EtatPartie;
    bool ExtraireAventure(QString cheminAventure);
    void LancerAventure(QString premierEvt = "", QString premierEffet = "");

    // interfaces/widgets
    Histoire* m_Histoire;
    IPerso* m_Perso;
    float m_Duree;
    Ui::Aventure *ui;

    void InstallerInterface();

    // pour les aventrues qui n'utilisent pas le json mais du code :surclasser aventure et développer cette fonction
    virtual void GenererAventure() = 0;
    virtual void GenererCaracs() = 0;

public:
    explicit Aventure(QWidget *parent = nullptr);
    explicit Aventure(QString cheminAventure, QString firstEvt = "", QString premierEffet = "", QWidget *parent = 0);
    ~Aventure();

    bool LancerEvtEtOuEffetCourant();

    const static QFont* BASE_FONT;
    const static QFont* TITRE_FONT;
    static QString CHEMIN;
    static Aventure* ME;

    Histoire* GetHistoire();
    IPerso* GetPersoInterface();
    void SetEtatPartie(EtatPartie etat);
    void SetTypeEvt(TypeEvt typeEvt);
    TypeEvt GetTypeEvtActuel();
    EtatPartie GetEtatPartie();
    void AjouterDuree(float duree);
    bool EstEnModeHistoire();

    EtatPartie ChangerEtatPartie(QString nouvelEtatPartie);

public slots:
    // déclenche l'effet de base si aucun n'a été spécifiquement choisi par l'utilisateur (si n'y avait qu'un suivant potentiel)
    void DeclencherEffetSuivant();
};

#endif // AVENTURE_H
