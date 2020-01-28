#ifndef EFFET_H
#define EFFET_H

#include "noeudnarratif.h"
#include "../exec/execnoeud.h"
#include "../glisseur.h"
#include "choix.h"

class Evt;

class Effet : public NoeudNarratif
{
protected:

public:
    explicit Effet(std::shared_ptr<Evt> evt);
    explicit Effet(std::shared_ptr<Evt> evt,
                 QString id,
                 QString text = "",
                 QString imgPath = "");

    virtual ~Effet() {}

    void SupprimerTousLesChoix();
    Glisseur* AjouterGlisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id);
    int CalculerIndex();

    std::shared_ptr<Evt> m_Evt;
    Glisseur* m_Glisseur = nullptr;

    // gestion de la bdd :
    int m_BDD_EffetId = -1;

    friend class GenEvt;

private:
};

#endif // EFFET_H
