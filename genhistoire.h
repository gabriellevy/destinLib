#ifndef GENHISTOIRE_H
#define GENHISTOIRE_H

#include <QString>
#include "dbmanager.h"
#include "histoire.h"

/**
 * @brief Class servant à générer toute l'histoire du jeu destin. Une fois cette exécution terminée elle renvoie un objet ExecHistoire à exécutee t n'est plus utilisée.
 */
class GenHistoire
{
private:

protected:
    // gestion de la BDD :
    virtual void ChargerEvtsBdd();

public:
    GenHistoire(Hist* histoireGeneree);
    virtual ~GenHistoire();

    Hist* m_HistoireGeneree = nullptr;
    virtual Hist* GenererHistoire();
    virtual void GenererPersos();
    virtual void GenererThemes();
    virtual void GenererFonctionsCallback(); // cette fonction a de bonnes chances d'être vides. Je la laisse en abstraite à implémenter comme pense-bête


    /**
     * @brief charge le contenu de la bdd visée dans l'histoire
     * @param cheminBDD
     *
     * Il est tout à fait possible que les fonctions GenererHistoire et/ou GenererPersos
     * soient remplacées par des fonctions de ce genre si toutes les informations de l'histoire sont en bdd
     */
    virtual void ChargerBDD(QString cheminBDD);

    Evt* AjouterEvt(QString id, QString nom);
    EvtAleatoire* AjouterEvtAleatoire(QString id, QString nom);

    DbManager m_Db;
};

#endif // GENHISTOIRE_H
