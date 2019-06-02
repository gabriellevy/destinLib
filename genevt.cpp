#include "genevt.h"
#include <QSqlQuery>

GenEvt::GenEvt()
{

}


void GenEvt::ChargerEffetsBdd(Evt* evtDest)
{
    QSqlQuery query("SELECT * FROM d_Effet WHERE appartient_a_evt_id = " +
                    QString::number(evtDest->m_BDD_EvtId) +
                    " ORDER BY ordre");
    while (query.next())
    {
       int bd_id = query.value("id").toInt();

       Effet* effet = this->AjouterEffetVide(evtDest);
       effet->m_BDD_EffetId = bd_id;
       effet->m_ImgPath = query.value("m_CheminImg").toString();

       // récupération de la partie noeud :
       effet->AppliquerValeurDeNoeudBDD( query.value("est_un_noeud_id").toInt());

       effet->ChargerChoixBdd();
    }
}

void GenEvt::AjouterASelectionneurEvt(Evt* evt, int selectionneur_bdd_id)
{
    // vérifier si ce sélectionneur a déjà été créé depuis la bdd :
    for ( SelectionneurDEvenement* sel: SelectionneurDEvenement::s_TousLesSelectionneurs)
    {
        if ( sel->m_BddId == selectionneur_bdd_id) {
            sel->m_Evts.push_back(evt);
            return;
        }
    }

    // pas trouvé : on le crée
    QString req_str = "SELECT * FROM d_SelectionneurDEvt WHERE id = " + QString::number(selectionneur_bdd_id);
    QSqlQuery query(req_str);

    while (query.next())
    {
        int bdd_id = query.value("id").toInt();
        QString intitule = query.value("intitule").toString();

        SelectionneurDEvenement* sel = new SelectionneurDEvenement(intitule, bdd_id);
        sel->m_Evts.push_back(evt);
        SelectionneurDEvenement::s_TousLesSelectionneurs.push_back(sel);
    }
}

void GenEvt::AjouterImgFond(Evt* evt, QString fond)
{
    if ( fond != "") {
        evt->m_CheminImgFond = fond;
    }
}

Effet* GenEvt::AjouterEffet(Evt* evtDest, Effet* effet)
{
    // si l'événement a un chrono il le transmet à tous ses effets (pour annuler celà le chrono sur les effets doit être changé individuellement vers -1)
    if ( evtDest->m_MsChrono != -1 )
        effet->ChangerChrono(evtDest->m_MsChrono);
    evtDest->m_Effets.push_back(effet);

    return effet;
}

Effet* GenEvt::AjouterEffetVide(Evt* evtDest)
{
    Effet* effet = new Effet(evtDest);
    AjouterEffet(evtDest, effet);
    return effet;
}

Effet* GenEvt::AjouterEffetRetireurACarac(Evt* evtDest, QString caracId, QString valeurRetire, QString text, QString id)
{
    Effet* effet = new Effet(evtDest,
                            id,
                            text,
                            "");
    effet->AjouterRetireurACarac(caracId, valeurRetire);
    AjouterEffet(evtDest, effet);
    return effet;
}

Effet* GenEvt::AjouterEffetModificateurCarac(Evt* evtDest, QString caracId, QString nouvelleValeur, QString text, QString id)
{
    Effet* effet = new Effet(evtDest,
                            id,
                            text,
                            "");
    effet->AjouterChangeurDeCarac(caracId, nouvelleValeur);
    AjouterEffet(evtDest, effet);
    return effet;
}

Effet* GenEvt::AjouterEffetAjouteurACarac(Evt* evtDest, QString caracId, QString valeurAjoutee, QString id)
{
    Effet* effet = new Effet(evtDest, id);
    effet->AjouterAjouteurACarac(caracId, valeurAjoutee);
    AjouterEffet(evtDest, effet);
    return effet;
}

Effet* GenEvt::AjouterEffetNarration(Evt* evtDest, QString text, QString cheminImg, QString id)
{
    Effet* effet = new Effet(evtDest,
                            id,
                            text,
                            cheminImg);
    AjouterEffet(evtDest, effet);
    return effet;
}

Effet* GenEvt::AjouterEffetGlisseur(Evt* evtDest, QString text, QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id, QString cheminImg, QString id )
{
    Effet* effet = new Effet(evtDest,
                            id,
                            text,
                            cheminImg);

    effet->AjouterGlisseur(valeur_min, valeur_max, valeur_depart, carac_id);

    AjouterEffet(evtDest, effet);
    return effet;
}

Effet* GenEvt::AjouterEffetChangementPerso(Evt* evtDest, QString persoId, QString text, QString cheminImg, QString id)
{
    Effet* effet = new Effet(evtDest,
                             id,
                            text,
                             cheminImg);
    effet->m_ChangePerso = persoId;
    AjouterEffet(evtDest, effet);
    return effet;
}

Effet* GenEvt::AjouterEffetTest(Evt* evtDest, QString caracId, Comparateur comparateur, QString valeur, QString id )
{
    Effet* effet = new Effet(evtDest, id, "", "");
    effet->m_Conditions.push_back(new Condition(caracId, valeur, comparateur));
    AjouterEffet(evtDest, effet);
    return effet;
}

Evt* GenEvt::GenererEvt(QString id, QString nom)
{
    Evt* evt = new Evt(id, nom);
    return evt;
}

EvtAleatoire* GenEvt::GenererEvtAleatoire(QString id, QString nom)
{
    EvtAleatoire* evt = new EvtAleatoire(id, nom);
    return evt;
}

