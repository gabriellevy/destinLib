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


