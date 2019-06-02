#ifndef EFFET_H
#define EFFET_H

#include "noeud.h"
#include "choix.h"
#include "execnoeud.h"
#include "glisseur.h"

enum OrientationAffichageChoix
{
    oac_vertical,
    oac_horizontal
};
class Evt;

class Effet : public Noeud
{
    //Effet* m_ElseNoeud = nullptr;

public:
    explicit Effet(Evt* evt);
    explicit Effet(Evt* evt,
                   QString id,
             QString text = "",
             QString imgPath = "");
    virtual ~Effet() {};
    /*explicit Effet(QJsonObject effetJson, QWidget *parent = nullptr);*/

    QList<Choix*> m_Choix;
    OrientationAffichageChoix m_OrientationAffichageChoix = OrientationAffichageChoix::oac_vertical;

    //Effet* GetElse();
    //bool AQuelqueChoseAAfficher();

    Choix* AjouterChoixVide();
    Choix* AjouterChoixChangeurDeCarac(QString text, QString carac, QString valeur);
    Choix* AjouterChoixGoToEffet(QString text, QString go_to_effet_id, QString cheminImg = "");
    void SupprimerTousLesChoix();
    //Effet* AjouterElse(QString text);
    Glisseur* AjouterGlisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id);
    void ChargerChoixBdd();
    int CalculerIndex();

    Evt* m_Evt;
    Glisseur* m_Glisseur = nullptr;

    // gestion de la bdd :
    int m_BDD_EffetId = -1;

private:
};

#endif // EFFET_H
