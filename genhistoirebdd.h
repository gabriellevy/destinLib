#ifndef GENHISTOIREBDD_H
#define GENHISTOIREBDD_H

#include "genhistoire.h"
#include "dbmanager.h"

class GenHistoireBdd : public GenHistoire
{

protected:
    // gestion de la BDD :
    virtual void ChargerEvtsBdd();

public:
    GenHistoireBdd(Hist* histoireGeneree);

    DbManager m_Db;

    /**
     * @brief charge le contenu de la bdd visée dans l'histoire
     * @param cheminBDD
     *
     * Il est tout à fait possible que les fonctions GenererHistoire et/ou GenererPersos
     * soient remplacées par des fonctions de ce genre si toutes les informations de l'histoire sont en bdd
     */
    virtual void ChargerBDD(QString cheminBDD);
};

#endif // GENHISTOIREBDD_H
