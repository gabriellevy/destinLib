#include "genevt.h"
#include <QSqlQuery>
#include "lancerde.h"
#include <functional>

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

       this->ChargerChoixBdd(effet);
    }
}

void GenEvt::AjouterASelectionneurEvt(Evt* evt, Condition* poids, int selectionneur_bdd_id)
{
    // vérifier si ce sélectionneur a déjà été créé depuis la bdd :
    for ( SelectionneurDeNoeud* sel: SelectionneurDeNoeud::s_TousLesSelectionneurs)
    {
        if ( sel->m_BddId == selectionneur_bdd_id) {
            sel->m_NoeudsProbables.push_back(new NoeudProbable(evt, poids));
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

        SelectionneurDeNoeud* sel = new SelectionneurDeNoeud(intitule, bdd_id);
        sel->m_NoeudsProbables.push_back(new NoeudProbable(evt, poids));
        SelectionneurDeNoeud::s_TousLesSelectionneurs.push_back(sel);
    }
}

void GenEvt::AjouterImgFond(Evt* evt, QString fond)
{
    if ( fond != "") {
        evt->m_CheminImgFond = fond;
    }
}

Effet* GenEvt::AjouterEffet(Effet* effet, Evt* evtDest)
{
    if ( evtDest == nullptr )
        evtDest = m_DernierEvtGenere;

    // si l'événement a un chrono il le transmet à tous ses effets (pour annuler celà le chrono sur les effets doit être changé individuellement vers -1)
    if ( evtDest->m_MsChrono != -1 )
        effet->ChangerChrono(evtDest->m_MsChrono);
    evtDest->m_Effets.push_back(effet);

    m_DernierEffetGenere = effet;
    return effet;
}

Effet* GenEvt::AjouterEffetVide(Evt* evtDest)
{
    if ( evtDest == nullptr)
        evtDest = m_DernierEvtGenere;
    Effet* effet = new Effet(evtDest);

    return AjouterEffet(effet, evtDest);
}

Effet* GenEvt::AjouterEffetSelectionneurDeNoeud(QString id, QString text, Evt* evtDest)
{
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->m_SelectionneurDeNoeud = new SelectionneurDeNoeud(id);
    return effet;
}

Effet* GenEvt::AjouterEffetRetireurACarac(QString caracId, QString valeurRetire, QString text, QString id, Evt* evtDest)
{
    if ( evtDest == nullptr)
        evtDest = m_DernierEvtGenere;
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->AjouterRetireurACarac(caracId, valeurRetire);
    return effet;
}

Effet* GenEvt::AjouterEffetModificateurCarac(QString caracId, QString nouvelleValeur, QString text, QString id, Evt* evtDest)
{
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->AjouterChangeurDeCarac(caracId, nouvelleValeur);
    return effet;
}

Noeud* GenEvt::GenererNoeudModificateurCarac(QString caracId, QString nouvelleValeur, QList<Condition*> conditions)
{
    Noeud* noeud = new Noeud();
    noeud->AjouterChangeurDeCarac(caracId, nouvelleValeur);
    noeud->m_Conditions.append( conditions );
    return noeud;
}

Effet* GenEvt::AjouterEffetAjouteurACarac(QString caracId, QString valeurAjoutee, QString id, Evt* evtDest)
{
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->AjouterAjouteurACarac(caracId, valeurAjoutee);
    return effet;
}

Effet* GenEvt::AjouterEffetNarration(QString text, QString cheminImg, QString id, Evt* evtDest)
{
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->m_ImgPath = cheminImg;
    return effet;
}



LancerDe* GenEvt::AjouterLancerDe(QString texte, int nbDes, std::function<ResExecutionLancerDe*(int)> callback,
           /*QVector<QString> params, */Effet* effetDest)
{
    if ( effetDest == nullptr)
        effetDest = m_DernierEffetGenere;

    LancerDe* lancerDe = new LancerDe(effetDest, texte, nbDes, callback);
    effetDest->m_LancerDe = lancerDe;

    return lancerDe;
}

Effet* GenEvt::AjouterEffetCallbackDisplay(std::function<void()> callbackDisplay, QString text, QString cheminImg, QString id, Evt* evtDest)
{
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->m_ImgPath = cheminImg;
    effet->m_CallbackDisplay = callbackDisplay;
    return effet;
}

Effet* GenEvt::AjouterEffetGlisseur(QString text, QString valeur_min, QString valeur_max, QString valeur_depart,
                                    QString carac_id, QString cheminImg, QString id, Evt* evtDest )
{
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->m_ImgPath = cheminImg;

    effet->AjouterGlisseur(valeur_min, valeur_max, valeur_depart, carac_id);
    return effet;
}

Effet* GenEvt::AjouterEffetChangementPerso(QString persoId, QString text, QString cheminImg, QString id, Evt* evtDest)
{
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->m_ImgPath = cheminImg;
    effet->m_ChangePerso = persoId;
    return AjouterEffet(effet, evtDest);
}

Effet* GenEvt::AjouterEffetTest(QString caracId, Comparateur comparateur, QString valeur, QString id, Evt* evtDest )
{
    Effet* effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Conditions.push_back(new Condition(caracId, valeur, comparateur));
    return effet;
}

Evt* GenEvt::GenererEvt(QString id, QString nom)
{
    Evt* evt = new Evt(id, nom);
    m_DernierEvtGenere = evt;
    return evt;
}

EvtAleatoire* GenEvt::GenererEvtAleatoire(QString id, QString nom)
{
    EvtAleatoire* evt = new EvtAleatoire(id, nom);
    m_DernierEvtGenere = evt;
    return evt;
}

Choix* GenEvt::AjouterChoixVide(Effet* effetDest)
{
    if ( effetDest == nullptr)
        effetDest = m_DernierEffetGenere;

    Choix* choix = new Choix(effetDest);
    effetDest->m_Choix.push_back(choix);
    return choix;
}


Choix* GenEvt::AjouterChoixAjouteurACarac(QString text, QString carac, QString valeur, QString go_to_effet_id, Effet* effetDest)
{
    Choix* choix = AjouterChoixVide(effetDest);
    choix->m_Texte = text;
    choix->AjouterAjouteurACarac(carac, valeur);
    choix->m_GoToEffetId = go_to_effet_id;
    return choix;
}


Choix* GenEvt::AjouterChoixChangeurDeCarac(QString text, QString carac, QString valeur, QString go_to_effet_id, Effet* effetDest)
{
    Choix* choix = AjouterChoixVide(effetDest);
    choix->m_Texte = text;
    choix->AjouterChangeurDeCarac(carac, valeur);
    choix->m_GoToEffetId = go_to_effet_id;
    return choix;
}

Choix* GenEvt::AjouterChoixGoToEffet(QString text, QString go_to_effet_id, QString cheminImg, Effet* effetDest)
{
    Choix* choix = AjouterChoixVide(effetDest);
    choix->m_Texte = text;
    choix->m_CheminImg = cheminImg;
    choix->m_GoToEffetId = go_to_effet_id;
    return choix;
}

void GenEvt::ChargerChoixBdd(Effet* effet)
{
    if ( effet == nullptr)
        effet = m_DernierEffetGenere;

    QString req = "SELECT * FROM d_Choix WHERE appartient_a_effet_id = " +
            QString::number(effet->m_BDD_EffetId) +
            " ORDER BY ordre";
    QSqlQuery query(req);
    while (query.next())
    {
       int bd_id = query.value("id").toInt();

       Choix* choix = this->AjouterChoixVide(effet);
       choix->m_BDD_ChoixId = bd_id;

       // récupération de la partie noeud :
       choix->AppliquerValeurDeNoeudBDD( query.value("est_un_noeud_id").toInt());
    }
}

Effet* GenEvt::AjouterEffetSelecteurDEvt(QVector<NoeudProbable*> noeudsDestination, QString id, QString text, Evt* evtDest)
{
    Effet* effet = this->AjouterEffetSelectionneurDeNoeud(id, text, evtDest);

    // vérification et ajout des noeuds destination
    for (int i = 0; i < noeudsDestination.length() ; ++i) {
        NoeudProbable* noeudProbable = noeudsDestination.at(i);
        /*Q_ASSERT_X( noeudProbable->m_PoidsProba->m_Proba > 0 ,
                    "Tentative d'ajouter un neud sans probabilité à un sélecteur de noeud via probabilité...",
                    "GenEvt::AjouterEffetSelecteurDEvt");
*/
        effet->m_SelectionneurDeNoeud->m_NoeudsProbables.push_back(noeudProbable);
    }

    return effet;
}

