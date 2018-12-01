#ifndef EFFET_H
#define EFFET_H

#include "noeud.h"
#include "choix.h"
#include "glisseur.h"

namespace Ui {
class Effet;
}

enum OrientationAffichageChoix
{
    oac_vertical,
    oac_horizontal
};


class Effet : public QWidget, public Noeud
{
    Q_OBJECT
    Effet* m_ElseNoeud = nullptr;

public:
    explicit Effet(QWidget *parent = nullptr);
    explicit Effet(QString id,
             QString text = "",
             QString imgPath = "",
             QWidget *parent = nullptr);
    /*explicit Effet(QJsonObject effetJson, QWidget *parent = nullptr);*/
    ~Effet();

    QList<Choix*> m_Choix;
    OrientationAffichageChoix m_OrientationAffichageChoix = OrientationAffichageChoix::oac_vertical;

    Effet* GetElse();
    void AfficherNoeud();

    void AfficherBoutonSuivant();
    bool AQuelqueChoseAAfficher();
    bool GestionTransition();

    Choix* AjouterChoixChangeurDeCarac(QString text, QString carac, QString valeur);
    Choix* AjouterChoixGoToEffet(QString text, QString go_to_effet_id, QString cheminImg = "");
    Effet* AjouterElse(QString text);
    Glisseur* AjouterGlisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id);

    virtual void FinExecutionNoeud();
    void ChargerImage(QString chemin);

private:
    Ui::Effet *ui;
    Glisseur* m_Glisseur;

public slots:
    //void ExecuterNoeudSlot(bool afficherNoeud = true, bool lancerNoeudSuivantSiRienAAfiicher = true);
    void valeurGlisseurAChange();
    void FinChrono();
};

#endif // EFFET_H
