#include "genhistoire.h"

GenHistoire::GenHistoire(Hist* histoireGeneree):m_HistoireGeneree(histoireGeneree)
{
    m_GenerateurEvt = new GenEvt();
}

GenHistoire::~GenHistoire()
{
}

Hist* GenHistoire::GenererHistoire()
{
    return m_HistoireGeneree;
}
void GenHistoire::GenererPersos()
{

}
void GenHistoire::GenererThemes()
{

}
void GenHistoire::GenererFonctionsCallback()
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
    Evt* evt = this->m_GenerateurEvt->GenererEvt(id, nom);
    m_HistoireGeneree->m_Evts.push_back(evt);
    return evt;
}


