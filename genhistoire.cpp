#include "genhistoire.h"

GenHistoire::GenHistoire(QWidget *parent)
{
    if ( parent != nullptr)
        m_HistoireGeneree = new ExecHistoire(parent);
}

GenHistoire::~GenHistoire()
{
}

ExecHistoire* GenHistoire::GenererHistoire()
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

void GenHistoire::ChargerBDD(QString cheminBDD)
{
    if ( !this->m_Db.m_db.isOpen())
        this->m_Db.Initialisation(cheminBDD);

    if ( this->m_Db.m_db.isOpen())
    {
        this->ChargerEvtsBdd();
    }
}

EvtAleatoire* GenHistoire::AjouterEvtAleatoire(QString id, QString nom)
{
    EvtAleatoire* evt = new EvtAleatoire(id, nom);
    m_HistoireGeneree->m_Evts.push_back(static_cast<Evt*>(evt));
    return evt;
}

Evt* GenHistoire::AjouterEvt(QString id, QString nom)
{
    Evt* evt = new Evt(id, nom);
    m_HistoireGeneree->m_Evts.push_back(evt);
    return evt;
}

void GenHistoire::ChargerEvtsBdd()
{
    QSqlQuery query("SELECT * FROM d_Evt");
    while (query.next())
    {
       int bd_id = query.value("id").toInt();

       Evt* evt = AjouterEvt("evt vide", "et sans nom");
       evt->AjouterImgFond(query.value("m_CheminImgFond").toString());
       evt->m_BDD_EvtId = bd_id;
       QString TypeEvenement = query.value("m_TypeEvenement").toString();
       if (TypeEvenement == "TE_Base") evt->m_TypeEvenement = TypeEvt::TE_Base;
       else if (TypeEvenement == "TE_Conditionnel") evt->m_TypeEvenement = TypeEvt::TE_Conditionnel;
       if (TypeEvenement == "TE_Aleatoire") evt->m_TypeEvenement = TypeEvt::TE_Aleatoire;

       // récupération de la partie noeud :
       evt->AppliquerValeurDeNoeudBDD( query.value("est_noeud_id").toInt());

       evt->ChargerEffetsBdd();

       int selectionneur_bdd_id = query.value("appartient_selectionneur_evt_id").toInt();
       if ( selectionneur_bdd_id > 0 )
           evt->AjouterASelectionneurEvt(selectionneur_bdd_id);
    }
}


