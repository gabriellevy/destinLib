#include "genhistoire.h"
#include "../aleatoire.h"

using std::shared_ptr;
using std::make_shared;

GenHistoire::GenHistoire(QString titre)
{
    m_GenerateurEvt = make_shared<GenEvt>();
    if ( titre == "" )
        titre = "titre inconu : vous devriez surclasser la constructeur de GenHistoire pour éditer ces valeurs";
    m_HistoireGeneree = make_shared<Hist>(titre);
    ME = this;
}

GenHistoire* GenHistoire::ME = nullptr;
GenHistoire* GenHistoire::GetGenHistoire()
{
    return ME;
}

GenHistoire::~GenHistoire()
{
    /*if ( m_GenerateurEvt != nullptr)
        delete m_GenerateurEvt;*/
    m_GenerateurEvt= nullptr;
}

shared_ptr<Hist> GenHistoire::GenererHistoire()
{
    m_HistoireGeneree->m_PhaseDeroulement = PhaseDeroulement::epd_Construction;
    return m_HistoireGeneree;
}

std::shared_ptr<EvtAleatoire> GenHistoire::AjouterEvtAleatoire(QString id, QString nom)
{
    std::shared_ptr<EvtAleatoire> evt = this->m_GenerateurEvt->GenererEvtAleatoire(id, nom);
    m_HistoireGeneree->m_EvtsAleatoires.push_back(evt);
    return evt;
}

std::shared_ptr<Evt> GenHistoire::AjouterEvt(QString id, QString nom)
{
    if ( nom == "")
        nom = id;
    std::shared_ptr<Evt> evt = this->m_GenerateurEvt->GenererEvt(id, nom);
    m_HistoireGeneree->m_Evts.push_back(evt);
    return evt;
}

void GenHistoire::FinGenerationHistoire()
{

}

/**
**  fonctions racourci
**  */

std::shared_ptr<Effet> GenHistoire::AjouterEffetGoToEvt(QString idEvtDest, QString id, std::shared_ptr<Evt> evt)
{
    std::shared_ptr<Effet> effet = m_GenerateurEvt->AjouterEffetVide(evt, id);
    effet->m_GoToEvtId = idEvtDest;
    return effet;
}

std::shared_ptr<Effet> GenHistoire::AjouterEffetGoToEffet(QString idEffetDest, QString id, std::shared_ptr<Evt> evt)
{
    std::shared_ptr<Effet> effet = m_GenerateurEvt->AjouterEffetVide(evt, id);
    effet->m_GoToEffetId = idEffetDest;
    return effet;
}

std::shared_ptr<Effet> GenHistoire::AjouterEffetNarration(QString text, QString cheminImg, QString id, std::shared_ptr<Evt> evt)
{
    return m_GenerateurEvt->AjouterEffetNarration(text, cheminImg, id, evt);
}

std::shared_ptr<Choix> GenHistoire::AjouterChoixAjouteurACarac(QString texte, QString carac, QString valeur, QString go_to_effet_id, std::shared_ptr<Effet> effet)
{
    return m_GenerateurEvt->AjouterChoixAjouteurACarac(texte, carac, valeur, go_to_effet_id, effet);
}

std::shared_ptr<Choix> GenHistoire::AjouterChoixGoToEffet(QString texte, QString go_to_effet_id, QString cheminImg, std::shared_ptr<Effet> effetDest)
{
    return m_GenerateurEvt->AjouterChoixGoToEffet(texte, go_to_effet_id, cheminImg, effetDest);
}

