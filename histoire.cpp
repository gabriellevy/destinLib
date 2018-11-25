#include "histoire.h"
#include "ui_histoire.h"
#include <QMessageBox>
#include "aventure.h"
#include <QScrollBar>
#include <QDebug>
#include <QTime>

Histoire::Histoire(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Histoire)
{
    ui->setupUi(this);

    m_CurrentEvtId = "";
    m_EffetIndex = 0;

    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));
}

Histoire::~Histoire()
{
    delete ui;
}


void Histoire::Generer(QJsonObject aventure)
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

}

Evt* Histoire::EvtActuel(bool forceHistoireMode)
{
    QString idATrouver = m_CurrentEvtId;

    // premier lancement
    if ( idATrouver == "")
    {
        if ( m_Evts.count() < 1)
        {
            QMessageBox::warning(Univers::ME, "erreur dans Evt* Histoire::EvtActuel()", "Il n'y a aucun événement dans l'histoire !");
            return nullptr;
        }
        this->SetCurrentEvtId(m_Evts.at(0)->m_Id);

        return m_Evts.at(0);
    }

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
    }

    return nullptr;
}


void Histoire::SetCurrentEvtId(QString id)
{
    if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel)
    {
        m_CurrentConditionnelEvtId = id;
    }
    else
    {
        m_CurrentEvtId = id;
    }
    GetIndexEffetConcerne() = 0;
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

Effet* Histoire::EffetActuel(bool forceHistoireMode)
{
    if ( !forceHistoireMode )
    {
        if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel )
        {
            if ( m_EffetConditionnelIndex == -1 || m_EffetConditionnelIndex >= EvtActuel()->m_Effets.size() )
                return nullptr;

            return EvtActuel()->m_Effets[m_EffetConditionnelIndex];
        }
    }
    else
    {
        if ( m_EffetIndex == -1 || m_EffetIndex >= EvtActuel(forceHistoireMode)->m_Effets.size() )
            return nullptr;

        return EvtActuel(forceHistoireMode)->m_Effets[m_EffetIndex];
    }

    Evt* evt = EvtActuel();
    Q_ASSERT_X(m_EffetIndex < evt->m_Effets.size(), "Histoire::EffetActuel", "m_EffetIndex actuel ne fait pas partie de l'événement actuel => ratage de passage à l'événement suivant à priori...");

    return EvtActuel()->m_Effets[m_EffetIndex];

}

void Histoire::SetEffetIndex(int index)
{
    GetIndexEffetConcerne() = index;
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
    if ( noeud->m_GoToEvtId != "" )
    {
        QString msg = "Interdit de faire des go_to_evt dans les événements aléatoires ou conditionnels ! m_GoToEvtId : " + noeud->m_GoToEvtId;
        Q_ASSERT_X( Univers::ME->GetTypeEvtActuel() != TE_Conditionnel,
                    "go_to_evt", msg.toStdString().c_str());

        // changement d'événement :
        this->SetCurrentEvtId(noeud->m_GoToEvtId);

        if ( noeud->m_GoToEffetId != "" )
        {
            GetIndexEffetConcerne() = DeterminerIndexEffet(noeud->m_GoToEffetId);
        }
        return true;
    }
    else if ( noeud->m_GoToEffetId != "" )
    {
        GetIndexEffetConcerne() = DeterminerIndexEffet(noeud->m_GoToEffetId);
        return true;
    }
    return false;
}

Noeud* Histoire::TesterSiEffetEstLancableOuSonElse(Noeud* noeudActuel)
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
}

Noeud* Histoire::GetEffetDindexSuivant(Noeud* noeudActuel)
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
}

QString Histoire::GetCaracValue(QString caracId)
{
    QString val = "";

    for ( int i = 0; i < m_Caracs.size() ; i++)
    {
        if ( m_Caracs[i]->m_Id == caracId)
            return m_Caracs[i]->m_Valeur;
    }

    return val;
}

bool Histoire::CetteCaracExisteDeja(QString id)
{
    for ( int i = 0; i < m_Caracs.size() ; ++i)
    {
        if ( m_Caracs[i]->m_Id == id)
            return true;
    }
    return false;
}

void Histoire::AppliquerCarac(SetCarac setCarac)
{
    bool trouve = false;
    for ( int i = 0; i < m_Caracs.size() ; ++i)
    {
        if ( m_Caracs[i]->m_Id == setCarac.m_CaracId)
        {
            switch(setCarac.m_ModifCaracType)
            {
            case ModifCaracType::SetCarac : {
                m_Caracs[i]->m_Valeur = setCarac.GetValeur();
                break;
            }
            case ModifCaracType::AddToCarac : {
                double valeur = m_Caracs[i]->m_Valeur.toDouble();
                valeur += setCarac.GetValeur().toDouble();
                m_Caracs[i]->m_Valeur = QString::number(valeur);
                break;
            }
            case ModifCaracType::RetireDeCarac : {
                double valeur = m_Caracs[i]->m_Valeur.toDouble();
                valeur -= setCarac.GetValeur().toDouble();
                m_Caracs[i]->m_Valeur = QString::number(valeur);
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
        carac->m_Id = setCarac.m_CaracId;
        carac->m_Valeur = setCarac.GetValeur();
        m_Caracs.append(carac);
    }
}


Noeud* Histoire::DeterminerPuisLancerEffetSuivant(Noeud* noeudActuel)
{
    EffetActuel()->FinExecutionNoeud();// le faire aussi pour l'évt ?

    bool effet_suivant_trouve = false;

    // il n'y a pas d'effet suivant si on n'est pas en état de partie "Histoire"
    if ( Univers::ME->GetEtatPartie() != EP_Deroulement )
        return nullptr;


    if ( noeudActuel == nullptr)
    {
        noeudActuel = EffetActuel();
        if ( !noeudActuel->TesterConditions() &&
             dynamic_cast<Effet*>(noeudActuel) != nullptr)
        {
            noeudActuel = static_cast<Effet*>(noeudActuel)->GetElse();
        }
    }

    Evt* evtActuel = EvtActuel();
    Evt* oldEvtActuel = EvtActuel();

    // les événements conditionnels, si leurs conditions sont remplies, sont immédiatement lancés :
    for ( int y = 0 ; y < m_EvtsConditionnels.size() ; ++y)
    {
        if ( m_EvtsConditionnels[y]->TesterConditions())
        {
            this->SetCurrentEvtId(m_EvtsConditionnels[y]->m_Id);
            noeudActuel = EffetActuel();
            evtActuel = EvtActuel();
            effet_suivant_trouve = true;
        }
    }

    // si le noeud doit se répéter on bloque le passage auto à l'effet suivant mais les actions et go to s'appliquent normalement
    bool repeter = ( noeudActuel->m_RepeatWhileConditions.size() > 0 &&
               Condition::TesterTableauDeConditions(noeudActuel->m_RepeatWhileConditions) );

    // déterminer si l'effet actuel contenait des 'go to' qui conditionnent le prochain événement ou effet :
    if ( !effet_suivant_trouve )
    {
         if ( noeudActuel != nullptr
             && AppliquerGoTo(noeudActuel))
        {
            evtActuel = EvtActuel();

            noeudActuel = EffetActuel();

            noeudActuel = TesterSiEffetEstLancableOuSonElse(noeudActuel);

            effet_suivant_trouve = (noeudActuel != nullptr);
        }
        else
        {
            // dans ce cas on doit forcément tester l'effet suivant :
            GetIndexEffetConcerne()++;
        }
    }

    // on est peut-être dans un événement aléatoire dont il faut sélectionner un effet ?
    if ( evtActuel->m_TypeEvenement == TE_Aleatoire)
    {
        noeudActuel = (static_cast<EvtAleatoire*>(evtActuel))->DeterminerEffetAleatoire();

        SetEffetIndex( evtActuel->m_Effets.indexOf(static_cast<Effet*>(noeudActuel)));

        effet_suivant_trouve = (noeudActuel != nullptr);
    }

    // on ne passe pas à l'effet suivant si il y a un while qui force à y rester :
    if ( !effet_suivant_trouve && !repeter )
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
    }

    bool afficheEffet = false;

    if ( noeudActuel == nullptr)
        noeudActuel = EffetActuel();

    if ( noeudActuel != nullptr)
        afficheEffet = true;

    if ( evtActuel == nullptr)
        evtActuel = EvtActuel();

    if ( oldEvtActuel != evtActuel )
    {
        // on est entré dans un nouvel événement => on exécute ses actions puis on passera à celles du premier effet si il y en a un
        //m_EffetIndex = -1;
        // si l'effet courant va être exécuté il se chargera d'afficher les élément de l'événement et/ou de alncer l'effet suivant dnc on se contente d'excuter le noeud
        evtActuel->LancerNoeud();
    }

    // note : ce peut être un Effet au sens objet mais aussi un simple noeud, un else par exemple
    if ( afficheEffet )
    {
        noeudActuel->LancerNoeud();
    }

    return noeudActuel;
}

int& Histoire::GetIndexEffetConcerne()
{
    if ( Univers::ME->GetTypeEvtActuel() == TE_Conditionnel)
        return m_EffetConditionnelIndex;
   return  m_EffetIndex;
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
        m_DernierEvtAffiche = evt;
        // nettoyage de l'evenement précédent
        if ( m_CurrentEvt != nullptr)
        {
            m_CurrentEvt->hide();
            /*ui->histoireLayout->layout()->removeWidget(m_CurrentEvt);
            m_CurrentEvt->Clean();
            delete m_CurrentEvt;
            m_CurrentEvt = nullptr;*/
        }

        //affichage du nouveau
        m_CurrentEvt = evt;
        //m_CurrentEvt->LancerNoeud();
        ui->histoireLayout->layout()->addWidget(m_CurrentEvt);
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
        ui->histoireScrollArea->setWidget(m_CurrentEvt);
        m_CurrentEvt->show();
    }
    if ( effetChangement )
    {
        QScrollBar* vertScroll = ui->histoireScrollArea->verticalScrollBar();
        vertScroll->setValue(0);
        m_CurrentEvt->RafraichirAffichageEffet(effet);
    }

    if ( evtChangement || effetChangement)
        this->update();
}
