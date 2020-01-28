#ifndef EVT_H
#define EVT_H

#include "effet.h"
#include <QVector>


enum TypeEvt {
    TE_Base,
    TE_Conditionnel,
    TE_Aleatoire // quand l'histoire prend une pause le temps d'un ou plusieurs événements aléatoires
};

class Evt : public NoeudNarratif
{
public:
    explicit Evt(QString id, QString nom);
    virtual ~Evt() {}

    TypeEvt m_TypeEvenement;

    QVector<std::shared_ptr<Effet>> m_Effets;
    std::shared_ptr<Effet> TrouverEffet(QString id);

    QString m_CheminImgFond;

    // utilisé suelment si l'événement a été chargé à partie de la base de donénes :
    int m_BDD_EvtId = -1; // note : un événement a un id en tant qu'evt et un pour sa partie "noeud" générique

    friend class GenEvt;
};

#endif // EVT_H
