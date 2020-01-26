#include "genhistoirebdd.h"
#include "../abs/condition.h"

GenHistoireBdd::GenHistoireBdd(QString titre):GenHistoire (titre)
{

}

void GenHistoireBdd::ChargerBDD(QString cheminBDD)
{
    if ( !this->m_Db.m_db.isOpen())
        this->m_Db.Initialisation(cheminBDD);

    if ( this->m_Db.m_db.isOpen())
    {
        this->ChargerEvtsBdd();
    }
}

GenHistoireBdd::~GenHistoireBdd()
{

}

void GenHistoireBdd::ChargerEvtsBdd()
{
    QSqlQuery query("SELECT * FROM d_Evt");
    while (query.next())
    {
       int bd_id = query.value("id").toInt();

       Evt* evt = AjouterEvt("evt vide", "et sans nom");
       this->m_GenerateurEvt->AjouterImgFond(evt, query.value("m_CheminImgFond").toString());
       evt->m_BDD_EvtId = bd_id;
       QString TypeEvenement = query.value("m_TypeEvenement").toString();
       if (TypeEvenement == "TE_Base") evt->m_TypeEvenement = TypeEvt::TE_Base;
       else if (TypeEvenement == "TE_Conditionnel") evt->m_TypeEvenement = TypeEvt::TE_Conditionnel;
       if (TypeEvenement == "TE_Aleatoire") evt->m_TypeEvenement = TypeEvt::TE_Aleatoire;

       // récupération de la partie noeud :
       evt->AppliquerValeurDeNoeudBDD( query.value("est_noeud_id").toInt());

       this->m_GenerateurEvt->ChargerEffetsBdd(evt);

       int selectionneur_bdd_id = query.value("appartient_selectionneur_evt_id").toInt();
       if ( selectionneur_bdd_id > 0 )
           m_GenerateurEvt->AjouterASelectionneurEvt(
                       evt,
                       new Condition(1.0, TypeProba::p_Relative),
                       selectionneur_bdd_id);// 2ème argument au pif, pas le temps de chercher à comprendre
    }
}
