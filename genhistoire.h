#ifndef GENHISTOIRE_H
#define GENHISTOIRE_H

#include <QString>
#include "histoire.h"
#include "genevt.h"

/**
 * @brief Class servant à générer toute l'histoire du jeu destin. Une fois cette exécution terminée elle renvoie un objet ExecHistoire à exécutee t n'est plus utilisée.
 */
class GenHistoire
{
protected:

    // fonction dans laquelle sont créées les caracs importantes du jeu
    // (obligatoire pour celles qui sont affichées, les autres peuvent être créées autrement)
    virtual void GenererCaracs() = 0;

public:
    GenHistoire(Hist* histoireGeneree);
    virtual ~GenHistoire();

    GenEvt* m_GenerateurEvt = nullptr;
    Hist* m_HistoireGeneree = nullptr;
    virtual Hist* GenererHistoire() = 0;
    virtual void GenererPersos() = 0;
    virtual void GenererFonctionsCallback() {} // cette fonction a de bonnes chances d'être vides. Je la laisse en abstraite à implémenter comme pense-bête


    Evt* AjouterEvt(QString id, QString nom = "");
    EvtAleatoire* AjouterEvtAleatoire(QString id, QString nom);
};

#endif // GENHISTOIRE_H
