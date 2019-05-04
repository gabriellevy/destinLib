#include "histoire.h"
#include "ui_histoire.h"
#include <QMessageBox>
#include "univers.h"
#include <QScrollBar>
#include <QDebug>
#include <QTime>
#include "reglages.h"

Histoire::Histoire(QWidget *parent) :
    QWidget(parent),
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


void Histoire::RafraichirAffichageLayouts(int largeur, int hauteur)
{
    if( largeur != -1)
    {
        this->setFixedSize(largeur, hauteur);
        this->ui->histoireScrollArea->setFixedSize(largeur - Univers::ME->m_Reglages.m_LargeurColonneGauche, hauteur);
        this->ui->histoireLayout->setFixedSize(largeur - Univers::ME->m_Reglages.m_LargeurColonneGauche, hauteur);
    }
    //ui->histoireScrollArea->layout()->update();
   // ui->histoireScrollArea->layout()->activate();

    ui->histoireLayout->layout()->update();
    ui->histoireLayout->layout()->activate();

    //this->layout()->update();
    //this->layout()->activate();
}

Histoire::~Histoire()
{
    delete ui;
}


/*void Histoire::Generer(QJsonObject aventure)
{
    if ( aventure.contains("themes") )
    {
        if ( aventure["themes"].isArray() )
        {
            QJsonArray jsonArrayThemes = aventure["themes"].toArray();

            for ( int i = 0; i < jsonArrayThemes.size(); ++i)
            {
                m_Themes.append(jsonArrayThemes[i].toObject()["valeur"].toString());
            }
        }
        else
            QMessageBox::warning(Univers::ME, "erreur", "Le champs themes doit être un tableau de thèmes !");
    }

    // récupération des différents types d'événements :
    if ( aventure.contains("evts") && aventure["evts"].isArray())
    {
        QJsonArray jsonArrayEvts = aventure["evts"].toArray();

        for ( int i = 0; i < jsonArrayEvts.size(); ++i)
        {
            Evt* evt = new Evt(jsonArrayEvts[i].toObject());
            m_Evts.append(evt);
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
            m_EvtsConditionnels.append(evt);
        }
    }

}*/

Evt* Histoire::GetEvtSelonId(QString idATrouver)
{
    for ( int i = 0; i < m_Evts.size(); ++i)
    {
        if ( m_Evts[i]->m_Id == idATrouver)
            return m_Evts[i];
    }
    for ( int i = 0; i < m_EvtsAleatoires.size(); ++i)
    {
        if ( m_Evts[i]->m_Id == idATrouver)
            return m_Evts[i];
    }

    for ( int i = 0; i < m_EvtsConditionnels.size(); ++i)
    {
        if ( m_EvtsConditionnels[i]->m_Id == idATrouver)
            return m_EvtsConditionnels[i];
    }
    return nullptr;
}

Evt* Histoire::EvtActuel(bool forceHistoireMode)
{
    // premier lancement
    if ( this->m_NoeudActuel == nullptr)
    {
        if ( m_Evts.count() < 1)
        {
            QMessageBox::warning(Univers::ME, "erreur dans Evt* Histoire::EvtActuel()", "Il n'y a aucun événement dans l'histoire !");
            return nullptr;
        }
        // si un événement s'appelle 'Debut' alors c'est que c'est le premiers, sinon on commence simplement au premier événement :
        this->m_NoeudActuel = this->GetEvtSelonId("Debut");
        /*m_CurrentEvtId = "Debut";
        Evt* prochainEvt = this->EvtActuel(forceHistoireMode);
        if ( prochainEvt != nullptr)
            return prochainEvt;
        else m_CurrentEvtId = "";*/

        if ( this->m_NoeudActuel == nullptr )
            this->SetCurrentEvtId(m_Evts.at(0)->m_Id);

        //return m_Evts.at(0);
    }

    if ( this->m_NoeudActuel->m_TypeNoeud == TypeNoeud::etn_Evt)
        return static_cast<Evt*>(this->m_NoeudActuel);

    if ( this->m_NoeudActuel->m_TypeNoeud == TypeNoeud::etn_Effet)
    {
        Effet* effetActuel = static_cast<Effet*>(this->m_NoeudActuel);
        return effetActuel->m_Evt;
    }

    if ( this->m_NoeudActuel->m_TypeNoeud == TypeNoeud::etn_Choix)
    {
        Choix* choixActuel = static_cast<Choix*>(this->m_NoeudActuel);
        return choixActuel->m_ParentEffet->m_Evt;
    }

    Q_ASSERT_X(true, "m_NoeudActuel n'est ni un choix ni un evt ni un effet : bizarre", "Histoire::EvtActuel");


    /*QString idATrouver = m_CurrentEvtId;

    if ( forceHistoireMode || Univers::ME->GetTypeEvtActuel() == TE_Base )
    {
        // puis recherche de l'id actuel parmi tous les tableaux d'événements
        for ( int i = 0; i < m_Evts.size(); ++i)
        {
            if ( m_Evts[i]->m_Id == idATrouver)
                return m_Evts[i];
        }
    }
    else if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel)
    {
        idATrouver = m_CurrentConditionnelEvtId;

        for ( int i = 0; i < m_EvtsConditionnels.size(); ++i)
        {
            if ( m_EvtsConditionnels[i]->m_Id == idATrouver)
                return m_EvtsConditionnels[i];
        }
    }

    // dans les modes non histoire il y a quand même un événement actuel qui peut encore se trouver dans les événements histoire ou aléatoires :
    for ( int i = 0; i < m_Evts.size(); ++i)
    {
        if ( m_Evts[i]->m_Id == idATrouver)
            return m_Evts[i];
    }

    for ( int i = 0; i < m_EvtsConditionnels.size(); ++i)
    {
        if ( m_EvtsConditionnels[i]->m_Id == m_CurrentEvtId)
            return m_EvtsConditionnels[i];
    }*/

    return nullptr;
}


bool Histoire::AppelerFonctionCallback(QString fonction, QVector<QString> caracs, QVector<QString> params)
{
    QString msg = "Fonction callback inexistante : " + fonction;
    Q_ASSERT_X(this->m_CallbackFunctions.contains(fonction), msg.toStdString().c_str(), "AppelerFonctionCallback");
    return this->m_CallbackFunctions[fonction](caracs, params);
}

void Histoire::SetCurrentEvtId(QString id)
{
    for (Evt* evt: m_Evts)
    {
        if ( evt->m_Id == id) {
            m_NoeudActuel = evt;
            return;
        }
    }
    for (Evt* evt: m_EvtsAleatoires)
    {
        if ( evt->m_Id == id) {
            m_NoeudActuel = evt;
            return;
        }
    }
    for (Evt* evt: m_EvtsConditionnels)
    {
        if ( evt->m_Id == id) {
            m_NoeudActuel = evt;
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

EvtAleatoire* Histoire::AjouterEvtAleatoire(QString id, QString nom)
{
    EvtAleatoire* evt = new EvtAleatoire(id, nom);
    m_Evts.push_back(static_cast<Evt*>(evt));
    return evt;
}

Evt* Histoire::AjouterEvt(QString id, QString nom)
{
    Evt* evt = new Evt(id, nom);
    m_Evts.push_back(evt);
    return evt;
}

int Histoire::DeterminerIndexEvt(QString idEvt)
{
    for ( int i = 0; i < m_Evts.size(); ++i)
    {
        if ( m_Evts[i] != nullptr && m_Evts[i]->m_Id == idEvt)
            return i;
    }

    QString msg = "Recherche d'un evt qui n'existe pas ! id evt : " + idEvt ;
    Q_ASSERT_X(false, "aventure", msg.toStdString().c_str());
    return -1;
}

int Histoire::CalculerIndex(Evt* evtATrouver)
{
    int index =0;
    for ( Evt* evt: this->m_Evts)
    {
        if ( evt == evtATrouver)
            return index;
        index++;
    }
    Q_ASSERT_X(true, "Evt introuvable dans sa propre histoire !", "Evt::CalculerIndex");
    return -1;
}

Effet* Histoire::EffetActuel(bool forceHistoireMode)
{
    if ( this->m_NoeudActuel->m_TypeNoeud == TypeNoeud::etn_Effet)
    {
        return static_cast<Effet*>(this->m_NoeudActuel);
    }

    if ( this->m_NoeudActuel->m_TypeNoeud == TypeNoeud::etn_Choix)
    {
        Choix* choixActuel = static_cast<Choix*>(this->m_NoeudActuel);
        return choixActuel->m_ParentEffet;
    }

    if ( this->m_NoeudActuel->m_TypeNoeud == TypeNoeud::etn_Evt)
    {
        Evt* evt = static_cast<Evt*>(this->m_NoeudActuel);
        qDebug()<<"Attention : GetEffet lancé alors qu'un Evt est le noeud actuel ! On a renvoyé le premier effet de ce noeud"<<endl;
        return evt->m_Effets[0];
    }

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


void Histoire::ChargerBDD(QString cheminBDD)
{
    if ( !this->m_Db.m_db.isOpen())
        this->m_Db.Initialisation(cheminBDD);

    if ( this->m_Db.m_db.isOpen())
    {
        this->ChargerEvtsBdd();
    }
}

void Histoire::SetEffetIndex(int index)
{
    Evt* evtActuel = this->EvtActuel();
    Q_ASSERT_X(index<evtActuel->m_Effets.length(), "index impossible pour cet événement", "Histoire::SetEffetIndex");
    m_NoeudActuel = evtActuel->m_Effets[index];
    //GetIndexEffetConcerne() = index;
}

void Histoire::GoToEffetId(QString idEffet)
{
    this->m_NoeudActuel = EvtActuel()->m_Effets[this->DeterminerIndexEffet(idEffet)];
}

int Histoire::DeterminerIndexEffet(QString idEffet)
{
    Evt* evtActuel = EvtActuel();

    if ( idEffet == "pas_fait" || idEffet == "pas_encore_fait" || idEffet == "")
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

bool Histoire::AppliquerGoTo(Noeud* noeud)
{
    bool ilYAgoto = false;
    if ( noeud->m_GoToEvtId != "" )
    {
        QString msg = "Interdit de faire des go_to_evt dans les événements aléatoires ou conditionnels ! m_GoToEvtId : " + noeud->m_GoToEvtId;
        Q_ASSERT_X( Univers::ME->GetTypeEvtActuel() != TE_Conditionnel,
                    "go_to_evt", msg.toStdString().c_str());

        // changement d'événement :
        this->SetCurrentEvtId(noeud->m_GoToEvtId);

        ilYAgoto = true;
    }

    if ( noeud->m_GoToEffetId != "" )
    {
        this->GoToEffetId(noeud->m_GoToEffetId);
        //GetIndexEffetConcerne() = DeterminerIndexEffet(noeud->m_GoToEffetId);
        ilYAgoto = true;
    }

    if ( noeud->m_SelectionneurDEvenement != nullptr)
    {
        Evt* evSuivant = noeud->m_SelectionneurDEvenement->DeterminerEvtSuivant();
        this->SetCurrentEvtId(evSuivant->m_Id);

        ilYAgoto = true;
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

void Histoire::AppliquerTheme(Theme* theme)
{
    this->m_Themes.push_back(theme);

    for(Carac* carac: theme->m_CaracsDeBase)
    {
        this->m_Caracs.push_back(carac);
    }
}

QString Histoire::GetCaracValue(QString caracId)
{
    QString val = "";

    for ( int i = 0; i < m_Caracs.size() ; i++)
    {
        if ( m_Caracs[i]->m_DataCarac.m_Id == caracId)
            return m_Caracs[i]->m_DataCarac.m_Valeur;
    }

    return val;
}

DPerso* Histoire::GetPersoCourant()
{
    return Univers::ME->GetPersoInterface()->GetPersoCourant();
}

bool Histoire::CetteCaracExisteDeja(QString id)
{
    for ( int i = 0; i < m_Caracs.size() ; ++i)
    {
        if ( m_Caracs[i]->m_DataCarac.m_Id == id)
            return true;
    }
    return false;
}

void Histoire::AppliquerCarac(SetCarac setCarac)
{
    bool trouve = false;
    for ( int i = 0; i < m_Caracs.size() ; ++i)
    {
        if ( m_Caracs[i]->m_DataCarac.m_Id == setCarac.m_CaracId)
        {
            switch(setCarac.m_ModifCaracType)
            {
            case ModifCaracType::SetCarac : {
                m_Caracs[i]->m_DataCarac.m_Valeur = setCarac.GetValeur();
                break;
            }
            case ModifCaracType::AddToCarac : {
                double valeur = m_Caracs[i]->m_DataCarac.m_Valeur.toDouble();
                valeur += setCarac.GetValeur().toDouble();
                m_Caracs[i]->m_DataCarac.m_Valeur = QString::number(valeur);
                break;
            }
            case ModifCaracType::RetireDeCarac : {
                double valeur = m_Caracs[i]->m_DataCarac.m_Valeur.toDouble();
                valeur -= setCarac.GetValeur().toDouble();
                m_Caracs[i]->m_DataCarac.m_Valeur = QString::number(valeur);
                break;
            }
            }
            trouve = true;
            return;
        }
    }

    if (!trouve)
    {
        Carac* carac = new Carac;
        carac->m_DataCarac.m_Id = setCarac.m_CaracId;
        carac->m_DataCarac.m_Valeur = setCarac.GetValeur();
        m_Caracs.append(carac);
    }
}


Noeud* Histoire::DeterminerPuisLancerNoeudSuivant(Noeud* noeudActuel, bool noeudActuelEstValide)
{
    if ( noeudActuel != nullptr)
        this->m_NoeudActuel = noeudActuel;

    this->m_NoeudActuel->FinExecutionNoeud();

    bool noeud_suivant_trouve = false;

    // si le noeud actuel est un evt alors il faut lancer immédiatement automatquement son prmeier effet :
    if ( noeudActuelEstValide && !noeud_suivant_trouve && this->m_NoeudActuel->m_TypeNoeud == TypeNoeud::etn_Evt)
    {
        this->m_NoeudActuel = this->EvtActuel()->m_Effets[0];
        noeud_suivant_trouve = true;
    }

    // il n'y a pas d'effet suivant si on n'est pas en état de partie "Histoire"
    // note => je ne comprends même plus ce que ça veut dire. Si je refais ce fameux mode histoire il faudra le documenter dans le wiki...
    if ( Univers::ME->GetEtatPartie() != EP_Deroulement )
        return nullptr;

    Evt* evtActuel = EvtActuel();
    //Evt* oldEvtActuel = EvtActuel();

    // les événements conditionnels, si leurs conditions sont remplies, sont immédiatement lancés :
    /*for ( int y = 0 ; y < m_EvtsConditionnels.size() ; ++y)
    {
        if ( m_EvtsConditionnels[y]->TesterConditions())
        {
            this->SetCurrentEvtId(m_EvtsConditionnels[y]->m_Id);
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
              this->m_NoeudActuel != nullptr
             && AppliquerGoTo(this->m_NoeudActuel))
        {
            //evtActuel = EvtActuel();

            //noeudActuel = EffetActuel();

            //noeudActuel = TesterSiEffetEstLancableOuSonElse(noeudActuel);

            noeud_suivant_trouve = (this->m_NoeudActuel != nullptr);
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
        this->m_NoeudActuel = (static_cast<EvtAleatoire*>(evtActuel))->DeterminerEffetAleatoire();

        SetEffetIndex( evtActuel->m_Effets.indexOf(static_cast<Effet*>(this->m_NoeudActuel)));

        noeud_suivant_trouve = (this->m_NoeudActuel != nullptr);
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
                if ( indexEvtActuel >= m_Evts.size())
                {
                    qDebug()<<"Impossible de trouver un événement suivant pour: evt id : " <<evtActuel->m_Id.toStdString().c_str();
                }
                qDebug()<<"Effet* Histoire::DeterminerEtActiverEffetSuivant(). indexEvtActuel : "<< indexEvtActuel;
                this->SetCurrentEvtId(m_Evts[indexEvtActuel]->m_Id);

                evtActuel = EvtActuel();
            }

        }
    }*/

    bool afficheNoeud = false;

    if ( this->m_NoeudActuel == nullptr)
        this->m_NoeudActuel = EffetActuel();

    if ( this->m_NoeudActuel != nullptr)
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
    while ( !this->m_NoeudActuel->TesterConditions())
    {
        afficheNoeud = false; // de toute façon le noeud actuel n'est pas lançable : on doit passer au suivant

        if ( this->m_NoeudActuel->m_TypeNoeud == TypeNoeud::etn_Effet)
        {
            Effet* effet = dynamic_cast<Effet*>(this->m_NoeudActuel);
            if ( effet->GetElse() != nullptr)
            {
                this->m_NoeudActuel = dynamic_cast<Effet*>(this->m_NoeudActuel)->GetElse();
                afficheNoeud = true;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    // note : ce peut être un Effet au sens objet mais aussi un simple noeud, un else par exemple ou un Evt
    if ( afficheNoeud )
    {
        this->m_NoeudActuel->LancerNoeud();
    } else {
        // le noeud courant était invalide et on n'a asp trouvé de else valide, on cherche le suivant
        return this->DeterminerPuisLancerNoeudSuivant(this->m_NoeudActuel, false);
    }

    return this->m_NoeudActuel;
}

/*int& Histoire::GetIndexEffetConcerne()
{
   if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel)
        return m_EffetConditionnelIndex;
   return  m_EffetIndex;
}*/

void Histoire::PasserAEffetIndexSuivant()
{
    int index = this->EffetActuel()->CalculerIndex() + 1;
    Evt* evtActuel = this->EvtActuel();
    if (index >= evtActuel->m_Effets.length())
    {
        // Il n'y a pas d'effet suivant par défaut dans cet événement
        // on va essayer de apsser à l'événement suivant mais ce n'est pas très "propre"
        this->PasserAEvtIndexSuivant();
    } else {
        this->m_NoeudActuel = evtActuel->m_Effets[index];
    }
}

void Histoire::PasserAEvtIndexSuivant()
{
    qDebug()<<"Histoire::PasserAEvtIndexSuivant : Attention il n'est pas très recommandé de passer d'un événement à un autre sans goto. De plus ça ne fonctionne que pour les événements de l'histoire de base."<<endl;
    int index = this->CalculerIndex(this->EvtActuel()) + 1;

    QString msg = "Impossible de passer à l'index d'effet suivant index : " + QString::number(index);
    Q_ASSERT_X(index >= this->m_Evts.length(), msg.toStdString().c_str(), "Histoire::PasserAEvtIndexSuivant");
    this->m_NoeudActuel = this->m_Evts[index];
}

void Histoire::AjouterDureeAEffetHistoireCourant(float duree)
{
    Effet* effet = EffetActuel(true);
    Q_ASSERT_X(effet!= nullptr, "effet == nullptr", "Effet actuel d'histoire introuvable ! ");
    Evt* evt = EvtActuel(true);
    Q_ASSERT_X(evt!= nullptr, "effet == nullptr", "Événement actuel d'histoire introuvable ! ");
    effet->AjouterDuree(duree);
    evt->AjouterDuree(duree);
}

void Histoire::RafraichirAffichageEvtEtOuEffet(Evt* evt, Effet* effet)
{
    if ( evt == nullptr) evt = EvtActuel();
    if ( effet == nullptr ) effet = EffetActuel();

    bool evtChangement = false;
    bool effetChangement = false;
    if ( evt != m_DernierEvtAffiche)
    {
        evtChangement = true;
        // nettoyage de l'evenement précédent
        if ( m_DernierEvtAffiche != nullptr)
        {
            m_DernierEvtAffiche->hide();
        }
        m_DernierEvtAffiche = evt;

        //affichage du nouveau
        //m_CurrentEvt = evt;
        //m_CurrentEvt->LancerNoeud();
        ui->histoireLayout->layout()->addWidget(evt);
        //ui->histoireLayout->layout()->addWidget(m_CurrentEvt);
    }

    /*if ( effet == m_DernierEffetAffiche)
    {
        QMessageBox::warning(Aventure::ME, "erreur dans Deroulement::AfficherEffet", "Tentative d'afficher deux fois d'affilée le même effet !");
    }
    else*/ if ( effet != nullptr )
    {
        if ( m_DernierEffetAffiche != effet)
        {
            effetChangement = true;
            m_DernierEffetAffiche = effet;
        }
    }
    if ( evtChangement)
    {
        // the point of the following takeWidget is to avoid the destruction of the previous evt by the setWidget call just after
        /*QWidget* evt = */ui->histoireScrollArea->takeWidget();
        ui->histoireScrollArea->setWidget(evt);
        evt->show();
    }
    if ( effetChangement )
    {
        QScrollBar* vertScroll = ui->histoireScrollArea->verticalScrollBar();
        vertScroll->setValue(0);
        evt->RafraichirAffichageEffet(effet);
    }

    if ( evtChangement || effetChangement)
        this->update();
}

Evt* Histoire::GetEvtSelonBddId(int id)
{
    for ( Evt* evt: this->m_Evts)
    {
        if ( evt->m_BDD_EvtId == id)
            return evt;
    }

    QString msg = QString("evt introuvable pour cet id : " + QString::number(id));
    Q_ASSERT_X(true, "GetEvtSelonBddId", msg.toStdString().c_str());

    return nullptr;
}

void Histoire::ChargerEvtsBdd()
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
