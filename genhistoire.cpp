#include "genhistoire.h"
#include "aleatoire.h"

GenHistoire::GenHistoire()
{
    m_GenerateurEvt = new GenEvt();
    m_HistoireGeneree = new Hist("titre inconu : vous devriez surclasser la constructeur de GenHistoire pour éditer ces valeurs");
}

GenHistoire::~GenHistoire()
{
}


EvtAleatoire* GenHistoire::AjouterEvtAleatoire(QString id, QString nom)
{
    EvtAleatoire* evt = this->m_GenerateurEvt->GenererEvtAleatoire(id, nom);
    m_HistoireGeneree->m_EvtsAleatoires.push_back(evt);
    return evt;
}

Evt* GenHistoire::AjouterEvt(QString id, QString nom)
{
    if ( nom == "")
        nom = id;
    Evt* evt = this->m_GenerateurEvt->GenererEvt(id, nom);
    m_HistoireGeneree->m_Evts.push_back(evt);
    return evt;
}

void GenHistoire::FinGenerationHistoire()
{
    delete m_GenerateurEvt;
}

// fonctions racourci :
Effet* GenHistoire::AjouterEffetNarration(QString text, QString cheminImg, QString id, Evt* evt)
{
    return m_GenerateurEvt->AjouterEffetNarration(text, cheminImg, id, evt);
}

Choix* GenHistoire::AjouterChoixAjouteurACarac(QString texte, QString carac, QString valeur, QString go_to_effet_id, Effet* effet)
{
    return m_GenerateurEvt->AjouterChoixAjouteurACarac(texte, carac, valeur, go_to_effet_id, effet);
}

Choix* GenHistoire::AjouterChoixGoToEffet(QString texte, QString go_to_effet_id, QString cheminImg, Effet* effetDest)
{
    return m_GenerateurEvt->AjouterChoixGoToEffet(texte, go_to_effet_id, cheminImg, effetDest);
}

