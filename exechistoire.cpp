#include "exechistoire.h"
#include "ui_histoire.h"
#include <QMessageBox>
#include "univers.h"
#include <QScrollBar>
#include <QDebug>
#include <QTime>
#include "reglages.h"
#include "histoire.h"
#include "execevt.h"
#include "execeffet.h"
#include "gestionnairecarac.h"

ExecHistoire::ExecHistoire(Hist* histoire, QWidget *parent) :
    QWidget(parent),
    m_Histoire(histoire),
    ui(new Ui::Histoire)
{
    ui->setupUi(this);

    //m_CurrentEvtId = "";
    //m_EffetIndex = 0;

    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));

    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details )
        ui->histoireScrollArea->setStyleSheet("background-color : rgb(0,255,0)");
}

QString ExecHistoire::GetTitre()
{
    return m_Histoire->m_Titre;
}

void ExecHistoire::RafraichirAffichageLayouts(int largeur, int hauteur)
{
    if( largeur != -1)
    {
        this->setFixedSize(largeur, hauteur);
        this->ui->histoireScrollArea->setFixedSize(largeur - Univers::ME->m_Reglages.m_LargeurColonneGauche, hauteur);
        this->ui->histoireLayout->setFixedSize(largeur - Univers::ME->m_Reglages.m_LargeurColonneGauche, hauteur);
    }

    ui->histoireLayout->layout()->update();
    ui->histoireLayout->layout()->activate();
}

ExecHistoire::~ExecHistoire()
{
    delete ui;
}


/*void Histoire::Generer(QJsonObject aventure)
{
    // récupération des différents types d'événements :
    if ( aventure.contains("evts") && aventure["evts"].isArray())
    {
        QJsonArray jsonArrayEvts = aventure["evts"].toArray();

        for ( int i = 0; i < jsonArrayEvts.size(); ++i)
        {
            Evt* evt = new Evt(jsonArrayEvts[i].toObject());
            this->m_Histoire->m_Evts.append(evt);
        }
    }
    else
        QMessageBox::warning(Univers::ME, "erreur", "Pas d'événement dans l'aventure !");

    if ( aventure.contains("evts_conditionnels") && aventure["evts_conditionnels"].isArray())
    {
        QJsonArray jsonArrayEvts = aventure["evts_conditionnels"].toArray();

        for ( int i = 0; i < jsonArrayEvts.size(); ++i)
        {
            Evt* evt = new Evt(jsonArrayEvts[i].toObject());
            this->m_Histoire->m_EvtsConditionnels.append(evt);
        }
    }

}*/

Evt* ExecHistoire::GetEvtSelonId(QString idATrouver)
{
    for ( int i = 0; i < this->m_Histoire->m_Evts.size(); ++i)
    {
        if ( this->m_Histoire->m_Evts[i]->m_Id == idATrouver)
            return this->m_Histoire->m_Evts[i];
    }
    for ( int i = 0; i < this->m_Histoire->m_EvtsAleatoires.size(); ++i)
    {
        if ( this->m_Histoire->m_Evts[i]->m_Id == idATrouver)
            return this->m_Histoire->m_Evts[i];
    }

    for ( int i = 0; i < this->m_Histoire->m_EvtsConditionnels.size(); ++i)
    {
        if ( this->m_Histoire->m_EvtsConditionnels[i]->m_Id == idATrouver)
            return this->m_Histoire->m_EvtsConditionnels[i];
    }
    return nullptr;
}

Evt* ExecHistoire::EvtActuel(bool forceHistoireMode)
{
    return this->GetExecEvtActuel(forceHistoireMode)->GetEvt();
}

ExecEvt* ExecHistoire::GetExecEvtActuel(bool /*forceHistoireMode*/)
{
    // premier lancement
    if ( this->m_ExecEvtActuel == nullptr)
    {
        Univers::ME->SetEtatPartie(EP_Deroulement);
        if ( this->m_Histoire->m_Evts.count() < 1)
        {
            QMessageBox::warning(Univers::ME, "erreur dans Evt* Histoire::EvtActuel()", "Il n'y a aucun événement dans l'histoire !");
            return nullptr;
        }
        // si un événement s'appelle 'Debut' alors c'est que c'est le premiers, sinon on commence simplement au premier événement :
        this->SetExecEvtActuel(this->GetEvtSelonId("Debut"));

        /*m_CurrentEvtId = "Debut";
        Evt* prochainEvt = this->EvtActuel(forceHistoireMode);
        if ( prochainEvt != nullptr)
            return prochainEvt;
        else m_CurrentEvtId = "";*/

        if ( this->m_ExecEvtActuel == nullptr )
            this->SetCurrentEvtId(this->m_Histoire->m_Evts.at(0)->m_Id);

        //return this->m_Histoire->m_Evts.at(0);
    }

    if ( this->m_ExecEvtActuel->m_Noeud->m_TypeNoeud == TypeNoeud::etn_Evt)
        return static_cast<ExecEvt*>(this->m_ExecEvtActuel);

    /*if ( this->m_ExecEvtActuel->m_Noeud->m_TypeNoeud == TypeNoeud::etn_Effet)
    {
        ExecEffet* effetActuel = static_cast<ExecEffet*>(this->m_ExecNoeudActuel);
        return effetActuel->m_ExecEvt;
    }*/

    /*if ( this->m_ExecNoeudActuel->m_Noeud->m_TypeNoeud == TypeNoeud::etn_Choix)
    {
        ExecChoix* choixActuel = static_cast<ExecChoix*>(this->m_ExecNoeudActuel);
        return choixActuel->m_ExecEffet->m_ExecEvt;
    }*/

    //Q_ASSERT_X(true, "m_NoeudActuel n'est ni un choix ni un evt ni un effet : bizarre", "Histoire::EvtActuel");
    Q_ASSERT_X(true, "Pas d'événement actuel : bizarre", "Histoire::EvtActuel");


    /*QString idATrouver = m_CurrentEvtId;

    if ( forceHistoireMode || Univers::ME->GetTypeEvtActuel() == TE_Base )
    {
        // puis recherche de l'id actuel parmi tous les tableaux d'événements
        for ( int i = 0; i < this->m_Histoire->m_Evts.size(); ++i)
        {
            if ( this->m_Histoire->m_Evts[i]->m_Id == idATrouver)
                return this->m_Histoire->m_Evts[i];
        }
    }
    else if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel)
    {
        idATrouver = m_CurrentConditionnelEvtId;

        for ( int i = 0; i < this->m_Histoire->m_EvtsConditionnels.size(); ++i)
        {
            if ( this->m_Histoire->m_EvtsConditionnels[i]->m_Id == idATrouver)
                return this->m_Histoire->m_EvtsConditionnels[i];
        }
    }

    // dans les modes non histoire il y a quand même un événement actuel qui peut encore se trouver dans les événements histoire ou aléatoires :
    for ( int i = 0; i < this->m_Histoire->m_Evts.size(); ++i)
    {
        if ( this->m_Histoire->m_Evts[i]->m_Id == idATrouver)
            return this->m_Histoire->m_Evts[i];
    }

    for ( int i = 0; i < this->m_Histoire->m_EvtsConditionnels.size(); ++i)
    {
        if ( this->m_Histoire->m_EvtsConditionnels[i]->m_Id == m_CurrentEvtId)
            return this->m_Histoire->m_EvtsConditionnels[i];
    }*/

    return this->m_ExecEvtActuel;
}


bool ExecHistoire::AppelerFonctionCallback(QString fonction, QVector<QString> caracs, QVector<QString> params)
{
    QString msg = "Fonction callback inexistante : " + fonction;
    Q_ASSERT_X(m_Histoire->m_CallbackFunctions.contains(fonction), msg.toStdString().c_str(), "AppelerFonctionCallback");
    return m_Histoire->m_CallbackFunctions[fonction](caracs, params);
}

void ExecHistoire::SetCurrentEvtId(QString id)
{
    for (Evt* evt: this->m_Histoire->m_Evts)
    {
        if ( evt->m_Id == id) {
            this->SetExecEvtActuel(evt);
            return;
        }
    }
    for (Evt* evt: this->m_Histoire->m_EvtsAleatoires)
    {
        if ( evt->m_Id == id) {
            this->SetExecEvtActuel(evt);
            return;
        }
    }
    for (Evt* evt: this->m_Histoire->m_EvtsConditionnels)
    {
        if ( evt->m_Id == id) {
            this->SetExecEvtActuel(evt);
            return;
        }
    }
    /*if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel)
    {
        m_CurrentConditionnelEvtId = id;
    }
    else
    {
        m_CurrentEvtId = id;
    }
    GetIndexEffetConcerne() = 0;*/
}

int ExecHistoire::DeterminerIndexEvt(QString idEvt)
{
    for ( int i = 0; i < this->m_Histoire->m_Evts.size(); ++i)
    {
        if ( this->m_Histoire->m_Evts[i] != nullptr && this->m_Histoire->m_Evts[i]->m_Id == idEvt)
            return i;
    }

    QString msg = "Recherche d'un evt qui n'existe pas ! id evt : " + idEvt ;
    Q_ASSERT_X(false, "aventure", msg.toStdString().c_str());
    return -1;
}

int ExecHistoire::CalculerIndex(Evt* evtATrouver)
{
    int index =0;
    for ( Evt* evt: this->m_Histoire->m_Evts)
    {
        if ( evt == evtATrouver)
            return index;
        index++;
    }
    Q_ASSERT_X(true, "Evt introuvable dans sa propre histoire !", "Evt::CalculerIndex");
    return -1;
}

Effet* ExecHistoire::EffetActuel(bool forceHistoireMode)
{
    return this->GetExecEffetActuel(forceHistoireMode)->GetEffet();
}

ExecEffet* ExecHistoire::GetExecEffetActuel(bool /*forceHistoireMode*/)
{
    return this->m_ExecEvtActuel->GetExecEffetActuel();
    /*if ( this->m_ExecEvtActuel->m_ExecEffetActuel->m_TypeNoeud == TypeNoeud::etn_Effet)
    {
        return static_cast<ExecEffet*>(this->m_ExecNoeudActuel);
    }

    if ( this->m_ExecNoeudActuel->m_TypeNoeud == TypeNoeud::etn_Choix)
    {
        Choix* choixActuel = static_cast<Choix*>(this->m_ExecNoeudActuel);
        return choixActuel->m_ParentEffet;
    }

    if ( this->m_ExecNoeudActuel->m_TypeNoeud == TypeNoeud::etn_Evt)
    {
        Evt* evt = static_cast<Evt*>(this->m_ExecNoeudActuel);
        qDebug()<<"Attention : GetEffet lancé alors qu'un Evt est le noeud actuel ! On a renvoyé le premier effet de ce noeud"<<endl;
        return evt->m_Effets[0];
    }*/

    Q_ASSERT_X(true, "m_NoeudActuel n'est ni un choix ni un evt ni un effet : bizarre", "Histoire::EvtActuel");

    return nullptr;

    /*if ( !forceHistoireMode )
    {
        if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel )
        {
            if ( m_EffetConditionnelIndex == -1 || m_EffetConditionnelIndex >= EvtActuel()->m_Effets.size() )
            {
                QString msg = "m_EffetConditionnelIndex invalide : " + QString::number(m_EffetConditionnelIndex);
                Q_ASSERT_X(true, "Histoire::EffetActuel", msg.toStdString().c_str());
                return nullptr;
            }

            return EvtActuel()->m_Effets[m_EffetConditionnelIndex];
        }
    }
    else
    {
        if ( m_EffetIndex == -1 || m_EffetIndex >= EvtActuel(forceHistoireMode)->m_Effets.size() )
        {
            QString msg = "m_EffetIndex invalide : " + QString::number(m_EffetIndex);
            Q_ASSERT_X(true, "Histoire::EffetActuel", msg.toStdString().c_str());
            return nullptr;
        }

        return EvtActuel(forceHistoireMode)->m_Effets[m_EffetIndex];
    }

    Evt* evt = EvtActuel();
    Q_ASSERT_X(m_EffetIndex < evt->m_Effets.size(), "Histoire::EffetActuel", "m_EffetIndex actuel ne fait pas partie de l'événement actuel => ratage de passage à l'événement suivant à priori...");

    return EvtActuel()->m_Effets[m_EffetIndex];*/

}

void ExecHistoire::SetEffetIndex(int index)
{
    Evt* evtActuel = this->EvtActuel();
    Q_ASSERT_X(index<evtActuel->m_Effets.length(), "index impossible pour cet événement", "Histoire::SetEffetIndex");
    this->m_ExecNoeudActuel = m_ExecEvtActuel->SetEffetIndex(index);
    //GetIndexEffetConcerne() = index;
}

void ExecHistoire::GoToEffetId(QString idEffet)
{
    int index = this->DeterminerIndexEffet(idEffet);
    this->SetEffetIndex(index);
}

int ExecHistoire::DeterminerIndexEffet(QString idEffet)
{
    Evt* evtActuel = EvtActuel();

    if ( idEffet == QLatin1String("pas_fait") ||
         idEffet == QLatin1String("pas_encore_fait") ||
         idEffet == QLatin1String(""))
    {
        QMessageBox::warning(this,
                             "Partie non créée !",
                             " La suite de cette aventure n'est pas encore écrite. Ce 'trou' se trouve à l'effet d'id '" + EffetActuel()->m_Id
                             + "' et de texte : '" + EffetActuel()->TexteAAfficher() + "'"
                             );
        return -1;
    }

    for ( int i = 0; i < evtActuel->m_Effets.size(); ++i)
    {
        if ( evtActuel->m_Effets[i] != nullptr &&
             //typeid(*(evtActuel->m_Effets[i])) == typeid(Effet) &&
             evtActuel->m_Effets[i]->m_Id == idEffet)
        {
            return i;
        }
    }
    QString msg = "Recherche d'un effet qui n'existe pas ! id effet : " + idEffet + " - id evt : " + evtActuel->m_Id;
    Q_ASSERT_X(false, "aventure", msg.toStdString().c_str());
    return -1;
}

bool ExecHistoire::AppliquerGoTo(Noeud* noeud)
{
    bool ilYAgoto = false;
    if ( noeud->m_GoToEvtId != QLatin1String("") )
    {
        QString msg = "Interdit de faire des go_to_evt dans les événements aléatoires ou conditionnels ! m_GoToEvtId : " + noeud->m_GoToEvtId;
        Q_ASSERT_X( Univers::ME->GetTypeEvtActuel() != TE_Conditionnel,
                    "go_to_evt", msg.toStdString().c_str());

        // changement d'événement :
        this->SetCurrentEvtId(noeud->m_GoToEvtId);

        ilYAgoto = true;
    }

    if ( noeud->m_GoToEffetId != QLatin1String("") )
    {
        this->GoToEffetId(noeud->m_GoToEffetId);
        //GetIndexEffetConcerne() = DeterminerIndexEffet(noeud->m_GoToEffetId);
        ilYAgoto = true;
    }

    if ( noeud->m_SelectionneurDeNoeud != nullptr)
    {
        Noeud* noeudSuivant = noeud->m_SelectionneurDeNoeud->DeterminerEvtSuivant();

        Evt* evSuivant = dynamic_cast<Evt*>(noeudSuivant);
        if ( evSuivant != nullptr ) {
            this->SetCurrentEvtId(evSuivant->m_Id);
            ilYAgoto = true;
        } else {
            Effet* effetSuivant = dynamic_cast<Effet*>(noeudSuivant);
            if ( evSuivant != nullptr ) {
                this->GoToEffetId(effetSuivant->m_Id);
                ilYAgoto = true;
            } else {
                Noeud* noeudAExecuter = dynamic_cast<Noeud*>(noeudSuivant);
                this->m_ExecNoeudActuel->ExecuterActionsNoeud(noeudAExecuter);
            }
        }
    }

    return ilYAgoto;
}

/*Noeud* Histoire::TesterSiEffetEstLancableOuSonElse(Noeud* noeudActuel)
{
    if ( noeudActuel->TesterConditions())
    {
        return noeudActuel;
    }
    else
    {
        noeudActuel = EffetActuel()->GetElse();
        if ( noeudActuel != nullptr)
            return noeudActuel;
    }
    return nullptr;
}*/

/*Noeud* Histoire::GetEffetDindexSuivant(Noeud* noeudActuel)
{
    Evt* evtActuel = EvtActuel();
    while ( GetIndexEffetConcerne() < evtActuel->m_Effets.size() )
    {
        noeudActuel = EffetActuel();

        noeudActuel = TesterSiEffetEstLancableOuSonElse( noeudActuel);

        if ( noeudActuel != nullptr)
        {
             return noeudActuel;
        }
        else {
            GetIndexEffetConcerne()++;
        }
    }
    return nullptr;
}*/

/*QString ExecHistoire::GetCaracValue(QString caracId)
{
    QString val = "";

    for ( int i = 0; i < GestionnaireCarac::GetGestionnaireCarac()->m_Caracs.size() ; i++)
    {
        if ( GestionnaireCarac::GetGestionnaireCarac()->m_Caracs[i]->m_DataCarac.m_Id == caracId)
            return GestionnaireCarac::GetGestionnaireCarac()->m_Caracs[i]->m_DataCarac.m_Valeur;
    }

    return val;
}*/


ExecNoeud* ExecHistoire::DeterminerPuisLancerNoeudSuivant(ExecNoeud* noeudActuel, bool noeudActuelEstValide)
{
    if ( noeudActuel != nullptr)
        this->m_ExecNoeudActuel = noeudActuel;

    this->m_ExecNoeudActuel->FinExecutionNoeud();

    bool noeud_suivant_trouve = false;

    // si le noeud actuel est un evt alors il faut lancer immédiatement automatquement son premier effet :
    if ( noeudActuelEstValide && !noeud_suivant_trouve && this->m_ExecNoeudActuel->m_Noeud->m_TypeNoeud == TypeNoeud::etn_Evt)
    {
        //this->m_ExecEvtActuel->SetEffetIndex(0);
        this->m_ExecNoeudActuel = this->m_ExecEvtActuel->GetExecEffetActuel();
        noeud_suivant_trouve = true;
    }

    // il n'y a pas d'effet suivant si on n'est pas en état de partie "Histoire"
    // note => je ne comprends même plus ce que ça veut dire. Si je refais ce fameux mode histoire il faudra le documenter dans le wiki...
    if ( Univers::ME->GetEtatPartie() != EP_Deroulement )
        return nullptr;

    Evt* evtActuel = EvtActuel();
    //Evt* oldEvtActuel = EvtActuel();

    // les événements conditionnels, si leurs conditions sont remplies, sont immédiatement lancés :
    /*for ( int y = 0 ; y < this->m_Histoire->m_EvtsConditionnels.size() ; ++y)
    {
        if ( this->m_Histoire->m_EvtsConditionnels[y]->TesterConditions())
        {
            this->SetCurrentEvtId(this->m_Histoire->m_EvtsConditionnels[y]->m_Id);
            this->m_NoeudActuel = EffetActuel();
            evtActuel = EvtActuel();
            effet_suivant_trouve = true;
        }
    }*/

    // si le noeud doit se répéter on bloque le passage auto à l'effet suivant mais les actions et go to s'appliquent normalement
    /*bool repeter = ( noeudActuel->m_RepeatWhileConditions.size() > 0 &&
               Condition::TesterTableauDeConditions(noeudActuel->m_RepeatWhileConditions) );*/

    // déterminer si l'effet actuel contenait des 'go to' qui conditionnent le prochain événement ou effet :
    if ( !noeud_suivant_trouve )
    {
         if ( noeudActuelEstValide &&
              this->m_ExecNoeudActuel != nullptr
             && AppliquerGoTo(this->m_ExecNoeudActuel->m_Noeud))
        {
            //evtActuel = EvtActuel();

            //noeudActuel = EffetActuel();

            //noeudActuel = TesterSiEffetEstLancableOuSonElse(noeudActuel);

            noeud_suivant_trouve = (this->m_ExecNoeudActuel != nullptr);
        }
        else
        {
            // dans ce cas on doit forcément tester l'effet suivant :
             this->PasserAEffetIndexSuivant();
            //GetIndexEffetConcerne()++;
        }
    }

    // on est peut-être dans un événement aléatoire dont il faut sélectionner un effet ?
    if ( evtActuel->m_TypeEvenement == TE_Aleatoire)
    {
        Effet* effetAleatoire = (static_cast<EvtAleatoire*>(evtActuel))->DeterminerEffetAleatoire();
        this->m_ExecNoeudActuel = this->m_ExecEvtActuel->SetExecEffet(effetAleatoire);

        //SetEffetIndex( evtActuel->m_Effets.indexOf(static_cast<Effet*>(this->m_ExecNoeudActuel)));

        noeud_suivant_trouve = (this->m_ExecNoeudActuel != nullptr);
    }

    // on ne passe pas à l'effet suivant si il y a un while qui force à y rester :
    /*if ( !effet_suivant_trouve && !repeter )
    {
        // effet suivant tout simplement :
        noeudActuel = GetEffetDindexSuivant( noeudActuel);
        if ( noeudActuel != nullptr)
            effet_suivant_trouve = true;

        if ( !effet_suivant_trouve )
        {
            // il n'y a pas d'effet suivant => evt suivant

            // si on était en mode aléatoire événement suivant signifie simplement retour en mode normal
            if ( Univers::ME->GetTypeEvtActuel() == TE_Aleatoire ||
                Univers::ME->GetTypeEvtActuel() == TE_Conditionnel
                 )
            {
                noeudActuel = EffetActuel();
                evtActuel = EvtActuel();

                noeudActuel = TesterSiEffetEstLancableOuSonElse( noeudActuel);
                if ( noeudActuel== nullptr)
                    noeudActuel = GetEffetDindexSuivant( noeudActuel);
            }
            else
            {
                qDebug()<<"Attention il n'est pas recommandé de passer ainsi dun événement à l'autre sans un go_to_evt. evt id : " <<evtActuel->m_Id.toStdString().c_str();
                GetIndexEffetConcerne() = 0;
                int indexEvtActuel = DeterminerIndexEvt(m_CurrentEvtId);
                indexEvtActuel++;
                if ( indexEvtActuel >= this->m_Histoire->m_Evts.size())
                {
                    qDebug()<<"Impossible de trouver un événement suivant pour: evt id : " <<evtActuel->m_Id.toStdString().c_str();
                }
                qDebug()<<"Effet* Histoire::DeterminerEtActiverEffetSuivant(). indexEvtActuel : "<< indexEvtActuel;
                this->SetCurrentEvtId(this->m_Histoire->m_Evts[indexEvtActuel]->m_Id);

                evtActuel = EvtActuel();
            }

        }
    }*/

    bool afficheNoeud = false;

    if ( this->m_ExecNoeudActuel == nullptr)
        this->m_ExecNoeudActuel = GetExecEffetActuel();

    if ( this->m_ExecNoeudActuel != nullptr)
        afficheNoeud = true;

    /*if ( evtActuel == nullptr)
        evtActuel = EvtActuel();*/

    /*if ( oldEvtActuel != evtActuel )
    {
        // on est entré dans un nouvel événement => on exécute ses actions puis on passera à celles du premier effet si il y en a un
        //m_EffetIndex = -1;
        // si l'effet courant va être exécuté il se chargera d'afficher les élément de l'événement et/ou de alncer l'effet suivant dnc on se contente d'excuter le noeud
        evtActuel->LancerNoeud();
    }*/

    // on fait le test de condition une seule fois juste avant d'effectuer les effets :
    while ( !this->m_ExecNoeudActuel->m_Noeud->TesterConditions())
    {
        afficheNoeud = false; // de toute façon le noeud actuel n'est pas lançable : on doit passer au suivant

        /* else désactivé jusqu'à nouvel ordre
         * if ( this->m_ExecNoeudActuel->m_Noeud->m_TypeNoeud == TypeNoeud::etn_Effet)
        {
            Effet* effet = dynamic_cast<Effet*>(this->m_ExecNoeudActuel);
            if ( effet->GetElse() != nullptr)
            {
                this->m_ExecNoeudActuel = dynamic_cast<Effet*>(this->m_ExecNoeudActuel)->GetElse();
                afficheNoeud = true;
            } else {
                break;
            }
        } else {
            break;
        }*/
    }

    // note : ce peut être un Effet au sens objet mais aussi un simple noeud, un else par exemple ou un Evt
    if ( afficheNoeud )
    {
        this->m_ExecNoeudActuel->LancerNoeud();
    } else {
        // le noeud courant était invalide et on n'a asp trouvé de else valide, on cherche le suivant
        return this->DeterminerPuisLancerNoeudSuivant(this->m_ExecNoeudActuel, false);
    }

    switch (this->m_ExecNoeudActuel->m_Noeud->m_TypeNoeud) {
    case TypeNoeud::etn_Evt: this->m_ExecEvtActuel = static_cast<ExecEvt*>(this->m_ExecNoeudActuel);
        break;
    /* a priori pas nécessaire :
     * case TypeNoeud::etn_Choix: this->m_ExecEvtActuel->m_ExecEffetActuel->m_ExecChoix = static_cast<ExecChoix*>(this->m_ExecNoeudActuel);
        break;*/
    case TypeNoeud::etn_Effet: this->m_ExecEvtActuel->SetExecEffet(static_cast<ExecEffet*>(this->m_ExecNoeudActuel));
        break;
    default:
        break;
    }

    return this->m_ExecNoeudActuel;
}

/*int& Histoire::GetIndexEffetConcerne()
{
   if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel)
        return m_EffetConditionnelIndex;
   return  m_EffetIndex;
}*/

void ExecHistoire::PasserAEffetIndexSuivant()
{
    int index = this->EffetActuel()->CalculerIndex() + 1;
    Evt* evtActuel = this->EvtActuel();
    if (index >= evtActuel->m_Effets.length())
    {
        // Il n'y a pas d'effet suivant par défaut dans cet événement
        // on va essayer de apsser à l'événement suivant mais ce n'est pas très "propre"
        this->PasserAEvtIndexSuivant();
    } else {
        this->m_ExecNoeudActuel = this->m_ExecEvtActuel->SetEffetIndex(index);
    }
}

void ExecHistoire::PasserAEvtIndexSuivant()
{
    qDebug()<<"Histoire::PasserAEvtIndexSuivant : Attention il n'est pas très recommandé de passer d'un événement à un autre sans goto. De plus ça ne fonctionne que pour les événements de l'histoire de base."<<endl;
    int index = this->CalculerIndex(this->EvtActuel()) + 1;

    QString msg = "Impossible de passer à l'index d'effet suivant index : " + QString::number(index);
    Q_ASSERT_X(index >= this->m_Histoire->m_Evts.length(), msg.toStdString().c_str(), "Histoire::PasserAEvtIndexSuivant");
    this->SetExecEvtActuel( this->m_Histoire->m_Evts[index] );
}

ExecEvt* ExecHistoire::SetExecEvtActuel(Evt* evt)
{
    if ( this->m_ExecEvtActuel != nullptr) {

        if ( this->m_ExecEvtActuel->GetEvt() == evt ) {
            this->m_ExecNoeudActuel = this->m_ExecEvtActuel;
            return this->m_ExecEvtActuel;
        }
        QString msg = "Tentative de alncer un événement déjà lancé : " + evt->m_Id;
        Q_ASSERT_X(this->m_ExecEvtActuel->GetEvt() == evt, msg.toStdString().c_str(), "ExecHistoire::SetEvtActuel");

        m_ExecEvtActuel->hide();
        //delete this->m_ExecEvtActuel;
    }

    this->m_ExecNoeudActuel = this->m_ExecEvtActuel = new ExecEvt(evt);

    return this->m_ExecEvtActuel;
}

void ExecHistoire::AjouterDureeAEffetHistoireCourant(float duree)
{
    Effet* effet = EffetActuel(true);
    Q_ASSERT_X(effet!= nullptr, "effet == nullptr", "Effet actuel d'histoire introuvable ! ");
    Evt* evt = EvtActuel(true);
    Q_ASSERT_X(evt!= nullptr, "effet == nullptr", "Événement actuel d'histoire introuvable ! ");
    effet->AjouterDuree(duree);
    evt->AjouterDuree(duree);
}

void ExecHistoire::RafraichirAffichageEvtEtOuEffet(Evt* evt, Effet* effet)
{
    if ( evt == nullptr) evt = EvtActuel();
    if ( effet == nullptr ) effet = EffetActuel();

    bool evtChangement = false;
    bool effetChangement = false;
    if ( evt != m_DernierEvtAffiche)
    {
        evtChangement = true;
        // nettoyage de l'evenement précédent
        /* normalement il n'y a plus à le retirer : c'est fait quand on change d'exeeffet
         * if ( m_ExecEvtActuel != nullptr)
        {
            m_ExecEvtActuel->hide();
        }*/
        m_DernierEvtAffiche = evt;

        //affichage du nouveau
        //m_CurrentEvt = evt;
        //m_CurrentEvt->LancerNoeud();
        ui->histoireLayout->layout()->addWidget(m_ExecEvtActuel);
        //ui->histoireLayout->layout()->addWidget(m_CurrentEvt);
    }

    /*Q_ASSERT_X( effet != m_DernierEffetAffiche,
                "Tentative d'afficher deux fois d'affilée le même effet !",
                "ExecHistoire::RafraichirAffichageEvtEtOuEffet");*/

    if ( effet != nullptr )
    {
        if ( m_DernierEffetAffiche != effet)
        {
            effetChangement = true;
            m_DernierEffetAffiche = effet;
        }
    }
    if ( evtChangement )
    {
        //this->SetExecEvtActuel(evt);
        // the point of the following takeWidget is to avoid the destruction of the previous evt by the setWidget call just after
        /*QWidget* evt = */ui->histoireScrollArea->takeWidget();
        ui->histoireScrollArea->setWidget(this->m_ExecEvtActuel);
        this->m_ExecEvtActuel->show();
    }
    if ( effetChangement )
    {
        /*ExecEffet* exec_effet = */this->m_ExecEvtActuel->SetExecEffet(effet);
        QScrollBar* vertScroll = ui->histoireScrollArea->verticalScrollBar();
        vertScroll->setValue(0);
        //this->m_ExecEvtActuel->RafraichirAffichageEffet(exec_effet);
        //this->m_ExecEvtActuel->SetExecEffet(exec_effet);
    }

    if ( evtChangement || effetChangement )
        this->update();
}

Evt* ExecHistoire::GetEvtSelonBddId(int id)
{
    for ( Evt* evt: this->m_Histoire->m_Evts)
    {
        if ( evt->m_BDD_EvtId == id)
            return evt;
    }

    QString msg = QString("evt introuvable pour cet id : " + QString::number(id));
    Q_ASSERT_X(true, "GetEvtSelonBddId", msg.toStdString().c_str());

    return nullptr;
}
