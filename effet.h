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
protected:
    explicit Effet(Evt* evt);
    explicit Effet(Evt* evt,
                   QString id,
             QString text = "",
             QString imgPath = "");

public:
    virtual ~Effet() {};
    /*explicit Effet(QJsonObject effetJson, QWidget *parent = nullptr);*/

    QList<Choix*> m_Choix;
    OrientationAffichageChoix m_OrientationAffichageChoix = OrientationAffichageChoix::oac_vertical;

    //Effet* GetElse();
    //bool AQuelqueChoseAAfficher();
    void SupprimerTousLesChoix();
    //Effet* AjouterElse(QString text);
    Glisseur* AjouterGlisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id);
    int CalculerIndex();

    Evt* m_Evt;
    Glisseur* m_Glisseur = nullptr;

    // gestion de la bdd :
    int m_BDD_EffetId = -1;

    friend class GenEvt;

private:
};

#endif // EFFET_H
