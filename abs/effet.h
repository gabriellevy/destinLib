#ifndef EFFET_H
#define EFFET_H

#include "noeudnarratif.h"
#include "../exec/execnoeud.h"
#include "../glisseur.h"
#include "choix.h"
class Evt;

class Effet : public NoeudNarratif
{
    //Effet* m_ElseNoeud = nullptr;
protected:
    explicit Effet(Evt* evt);
    explicit Effet(Evt* evt,
                   QString id,
             QString text = "",
             QString imgPath = "");

public:
    virtual ~Effet() {}
    /*explicit Effet(QJsonObject effetJson, QWidget *parent = nullptr);*/

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
