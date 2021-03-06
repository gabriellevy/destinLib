#include "genevt.h"
#include <QSqlQuery>
#include <functional>
#include "../abs/lancerde.h"
#include "../abs/noeudnarratif.h"
#include "../abs/evtaleatoire.h"

using std::shared_ptr;
using std::make_shared;

GenEvt::GenEvt() {}


void GenEvt::ChargerEffetsBdd(std::shared_ptr<Evt> evtDest)
{
    QSqlQuery query("SELECT * FROM d_Effet WHERE appartient_a_evt_id = " +
                    QString::number(evtDest->m_BDD_EvtId) +
                    " ORDER BY ordre");
    while (query.next())
    {
       int bd_id = query.value("id").toInt();

       std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest);
       effet->m_BDD_EffetId = bd_id;
       effet->m_ImgPath = query.value("m_CheminImg").toString();

       // récupération de la partie noeud :
       effet->AppliquerValeurDeNoeudBDD( query.value("est_un_noeud_id").toInt());

       this->ChargerChoixBdd(effet);
    }
}

void GenEvt::AjouterASelectionneurEvt(shared_ptr<Evt> evt, shared_ptr<Condition> poids, int selectionneur_bdd_id)
{
    // vérifier si ce sélectionneur a déjà été créé depuis la bdd :
    for ( shared_ptr<SelectionneurDeNoeud> sel: SelectionneurDeNoeud::s_TousLesSelectionneurs)
    {
        if ( sel->m_BddId == selectionneur_bdd_id) {
            sel->m_NoeudsProbables.push_back(std::make_shared<NoeudProbable>(evt, poids));
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

        shared_ptr<SelectionneurDeNoeud> sel = make_shared<SelectionneurDeNoeud>(intitule, bdd_id);
        sel->m_NoeudsProbables.push_back(std::make_shared<NoeudProbable>(evt, poids));
        SelectionneurDeNoeud::s_TousLesSelectionneurs.push_back(sel);
    }
}

void GenEvt::AjouterImgFond(std::shared_ptr<Evt> evt, QString fond)
{
    if ( fond != "") {
        evt->m_CheminImgFond = fond;
    }
}

std::shared_ptr<Effet> GenEvt::AjouterEffet(std::shared_ptr<Effet> effet, std::shared_ptr<Evt> evtDest)
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

std::shared_ptr<Effet> GenEvt::AjouterEffetVide(std::shared_ptr<Evt> evtDest, QString id)
{
    if ( evtDest == nullptr)
        evtDest = m_DernierEvtGenere;

    if ( id != "" ) {
        QString msg = "Cet id d'effet est déjà présent dans cet événement : " + id + ".";
        Q_ASSERT_X(evtDest->TrouverEffet(id) == nullptr,
                   msg.toStdString().c_str(),
                   "GenEvt::AjouterEffetVide");
    }
    std::shared_ptr<Effet> effet = std::make_shared<Effet>(evtDest, id);

    return AjouterEffet(effet, evtDest);
}

std::shared_ptr<Effet> GenEvt::AjouterEffetSelectionneurDeNoeud(QString id, QString text, std::shared_ptr<Evt> evtDest)
{
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest, id);
    effet->m_Texte = text;
    effet->m_SelectionneurDeNoeud = make_shared<SelectionneurDeNoeud>(id);
    return effet;
}

std::shared_ptr<Effet> GenEvt::AjouterEffetRetireurACarac(
        QString caracId, QString valeurRetire, QString text, QString id, std::shared_ptr<Evt> evtDest)
{
    if ( evtDest == nullptr)
        evtDest = m_DernierEvtGenere;
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->AjouterRetireurACarac(caracId, valeurRetire);
    return effet;
}

std::shared_ptr<Effet> GenEvt::AjouterEffetModificateurCarac(
        QString caracId, QString nouvelleValeur, QString text, QString id, std::shared_ptr<Evt> evtDest)
{
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest, id);
    effet->m_Texte = text;
    effet->AjouterChangeurDeCarac(caracId, nouvelleValeur);
    return effet;
}

shared_ptr<Noeud> GenEvt::GenererNoeudModificateurCarac(
        QString caracId, QString nouvelleValeur, QList<shared_ptr<Condition>> conditions)
{
    shared_ptr<Noeud> noeud = make_shared<Noeud>();
    noeud->AjouterChangeurDeCarac(caracId, nouvelleValeur);
    noeud->m_Conditions.append( conditions );
    return noeud;
}

std::shared_ptr<Effet> GenEvt::AjouterEffetAjouteurACarac(
        QString caracId, QString valeurAjoutee, QString id, std::shared_ptr<Evt> evtDest)
{
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest, id);
    effet->AjouterAjouteurACarac(caracId, valeurAjoutee);
    return effet;
}

std::shared_ptr<Effet> GenEvt::AjouterEffetNarration(
        QString text, QString cheminImg, QString id, std::shared_ptr<Evt> evtDest)
{
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest, id);
    effet->m_Texte = text;
    effet->m_ImgPath = cheminImg;
    return effet;
}



std::shared_ptr<LancerDe> GenEvt::AjouterLancerDe(QString texte,
                                  int nbDes,
                                  std::function<shared_ptr<ResExecutionLancerDe>(int)> callback,
                                  std::shared_ptr<Effet> effetDest)
{
    if ( effetDest == nullptr)
        effetDest = m_DernierEffetGenere;

    std::shared_ptr<LancerDe> lancerDe = make_shared<LancerDe>(texte, nbDes, callback);
    effetDest->m_LancerDe = lancerDe;

    return lancerDe;
}

shared_ptr<Effet> GenEvt::AjouterEffetCallbackDisplay(
        std::function<void()> callbackDisplay, QString text, QString cheminImg, QString id, shared_ptr<Evt> evtDest)
{
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest, id);
    effet->m_Texte = text;
    effet->m_ImgPath = cheminImg;
    effet->m_CallbackDisplay = callbackDisplay;
    return effet;
}

std::shared_ptr<Effet> GenEvt::AjouterEffetGlisseur(
        QString text, QString valeur_min, QString valeur_max, QString valeur_depart,
        QString carac_id, QString cheminImg, QString id, std::shared_ptr<Evt> evtDest )
{
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest, id);
    effet->m_Texte = text;
    effet->m_ImgPath = cheminImg;

    effet->AjouterGlisseur(valeur_min, valeur_max, valeur_depart, carac_id);
    return effet;
}

std::shared_ptr<Effet> GenEvt::AjouterEffetChangementPerso(
        QString persoId, QString text, QString cheminImg, QString id, std::shared_ptr<Evt> evtDest)
{
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest);
    effet->m_Id = id;
    effet->m_Texte = text;
    effet->m_ImgPath = cheminImg;
    effet->m_ChangePerso = persoId;
    return AjouterEffet(effet, evtDest);
}

std::shared_ptr<Effet> GenEvt::AjouterEffetTest(
        QString caracId, Comparateur comparateur, QString valeur, QString id, std::shared_ptr<Evt> evtDest )
{
    std::shared_ptr<Effet> effet = this->AjouterEffetVide(evtDest, id);
    effet->m_Conditions.push_back(make_shared<Condition>(caracId, valeur, comparateur));
    return effet;
}

std::shared_ptr<Evt> GenEvt::GenererEvt(QString id, QString nom)
{
    std::shared_ptr<Evt> evt = std::make_shared<Evt>(id, nom);
    m_DernierEvtGenere = evt;
    return evt;
}

shared_ptr<EvtAleatoire> GenEvt::GenererEvtAleatoire(QString id, QString nom)
{
    shared_ptr<EvtAleatoire> evt = make_shared<EvtAleatoire>(id, nom);
    m_DernierEvtGenere = evt;
    return evt;
}

std::shared_ptr<Choix> GenEvt::AjouterChoixVide(std::shared_ptr<Effet> effetDest)
{
    if ( effetDest == nullptr)
        effetDest = m_DernierEffetGenere;

    std::shared_ptr<Choix> choix = make_shared<Choix>();
    effetDest->m_Choix.push_back(choix);
    return choix;
}

std::shared_ptr<Choix> GenEvt::AjouterChoixVide(std::shared_ptr<LancerDe> lancerDe)
{
    std::shared_ptr<Choix> choix = make_shared<Choix>();
    lancerDe->m_Choix.push_back(choix);
    return choix;
}


std::shared_ptr<Choix> GenEvt::AjouterChoixAjouteurACarac(QString text, QString carac, QString valeur, QString go_to_effet_id, std::shared_ptr<Effet> effetDest)
{
    std::shared_ptr<Choix> choix = AjouterChoixVide(effetDest);
    choix->m_Texte = text;
    choix->AjouterAjouteurACarac(carac, valeur);
    choix->m_GoToEffetId = go_to_effet_id;
    return choix;
}


std::shared_ptr<Choix> GenEvt::AjouterChoixChangeurDeCarac(QString text, QString carac, QString valeur, QString go_to_effet_id, std::shared_ptr<Effet> effetDest)
{
    std::shared_ptr<Choix> choix = AjouterChoixVide(effetDest);
    choix->m_Texte = text;
    choix->AjouterChangeurDeCarac(carac, valeur);
    choix->m_GoToEffetId = go_to_effet_id;
    return choix;
}

std::shared_ptr<Choix> GenEvt::AjouterChoixGoToEffet(QString text, QString go_to_effet_id, QString cheminImg, std::shared_ptr<LancerDe> lancerDe)
{
    std::shared_ptr<Choix> choix = AjouterChoixVide(lancerDe);
    choix->m_Texte = text;
    choix->m_CheminImg = cheminImg;
    choix->m_GoToEffetId = go_to_effet_id;
    return choix;
}

std::shared_ptr<Choix> GenEvt::AjouterChoixGoToEffet(QString text, QString go_to_effet_id, QString cheminImg, std::shared_ptr<Effet> effetDest)
{
    std::shared_ptr<Choix> choix = AjouterChoixVide(effetDest);
    choix->m_Texte = text;
    choix->m_CheminImg = cheminImg;
    choix->m_GoToEffetId = go_to_effet_id;
    return choix;
}

void GenEvt::ChargerChoixBdd(std::shared_ptr<Effet> effet)
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

       std::shared_ptr<Choix> choix = this->AjouterChoixVide(effet);
       choix->m_BDD_ChoixId = bd_id;

       // récupération de la partie noeud :
       choix->AppliquerValeurDeNoeudBDD( query.value("est_un_noeud_id").toInt());
    }
}

shared_ptr<Effet> GenEvt::AjouterEffetSelecteurDEvt(
        QVector<shared_ptr<NoeudProbable>> noeudsDestination, QString id, QString text, shared_ptr<Evt> evtDest)
{
    std::shared_ptr<Effet> effet = this->AjouterEffetSelectionneurDeNoeud(id, text, evtDest);

    // vérification et ajout des noeuds destination
    for (int i = 0; i < noeudsDestination.length() ; ++i) {
        shared_ptr<NoeudProbable> noeudProbable = noeudsDestination.at(i);
        /*Q_ASSERT_X( noeudProbable->m_PoidsProba->m_Proba > 0 ,
                    "Tentative d'ajouter un neud sans probabilité à un sélecteur de noeud via probabilité...",
                    "GenEvt::AjouterEffetSelecteurDEvt");
*/
        effet->m_SelectionneurDeNoeud->m_NoeudsProbables.push_back(noeudProbable);
    }

    return effet;
}

