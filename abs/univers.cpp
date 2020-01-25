#include "univers.h"
#include "ui_univers.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include "histoire.h"
#include "../exec/execeffet.h"
#include "../exec/execevt.h"

const QFont* Univers::BASE_FONT = new QFont("Verdana", 10);
const QFont* Univers::TITRE_FONT = new QFont("Verdana", 20);
QString Univers::CHEMIN = "";
Univers* Univers::ME;

Univers::Univers(QWidget *parent, ModeAffichage modeAffichage, bool persoAffiche):QMainWindow(parent),
    ui(new Ui::Univers), m_PersoAffiche(persoAffiche), m_ModeAffichage(modeAffichage)
{
    InstallerInterface();
    if ( m_PersoAffiche )
        m_Perso = new IPerso(ui->persoWidget);
}

void Univers::LancerHistoire(QString idHistoire, QWidget* /* parent*/, QString premierEvt, QString premierEffet, bool BarreDeCote)
{
    this->m_Histoire = this->GenererUneHistoire(idHistoire);

    this->m_ExecHistoire = new ExecHistoire(this->m_Histoire);

    this->AfficherHistoire(ui->histoireWidget);

   if ( m_PersoAffiche ) {
        m_Perso->InitialiserPerso();
        m_Perso->RafraichirAffichage();

        // positionner l'interface
        ui->persoWidget->layout()->addWidget(m_Perso);
        m_Perso->show();
   }
   else {
       ui->persoWidget->hide();
   }

    m_Lecteur = new QMediaPlayer;

    if ( !BarreDeCote)
        ui->persoWidget->hide();

    // positionner l'interface
    /*m_Perso = new IPerso(ui->persoWidget);
    ui->persoWidget->layout()->addWidget(m_Perso);
    //m_Perso->show();
    m_Histoire = new Histoire(ui->histoireWidget);
    ui->histoireWidget->layout()->addWidget(m_Histoire);*/


    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details )
        ui->histoireWidget->setStyleSheet("background-color : rgb(255,0,0)");

    //LancerAventure();

    if ( premierEvt != "" ) {
        this->GetExecHistoire()->SetCurrentEvtId(premierEvt);
    }

    if ( premierEffet != "" ) {
        this->GetExecHistoire()->GetExecEvtActuel()->SetExecEffet(premierEffet);
    }

    m_Histoire->m_PhaseDeroulement = PhaseDeroulement::epd_Deroulement;
    LancerEvtEtOuEffetCourant();
    //DeclencherEffetSuivant();
    //this->m_ExecHistoire->GetExecEvtActuel();
}



Hist* Univers::GenererUneHistoire(QString histoireId)
{
    Q_ASSERT_X(m_GensHistoire.contains(histoireId), "Impossible de générer l'histoire 'histoireId' !!", "Univers::GenererUneHistoire");

    m_Histoire = m_GensHistoire[histoireId]->GenererHistoire();

    return m_Histoire;
}


void Univers::AfficherHistoire(QWidget *parent)
{
    this->setWindowTitle(m_ExecHistoire->GetTitre());
    parent->layout()->addWidget(m_ExecHistoire);
}

void Univers::AppliquerFond(QString urlImageFond)
{
    ui->Fond->setStyleSheet("QWidget#Fond { background-image: url(" + urlImageFond + ") }");
}

void Univers::AppliquerTheme(QColor fond)
{
    ui->Fond->setStyleSheet("QWidget#Fond { background-color: rgba(" +
                            QString::number(fond.red()) +
                            "," + QString::number(fond.green()) +
                            "," + QString::number(fond.blue()) +
                            "," + QString::number(fond.alpha()) +
                            ") }");
}

/*Univers::Univers(QString cheminAventure, ModeAffichage modeAffichage, QString firstEvt, QString premierEffet, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Aventure), m_ModeAffichage(modeAffichage)
{
    InstallerInterface();

    m_Lecteur = new QMediaPlayer;

    // lance réellement l'aventure
    if ( ExtraireAventure(cheminAventure))
    {
        LancerAventure(firstEvt, premierEffet);
    }
}*/

void Univers::InstallerInterface()
{
    ui->setupUi(this);

    Univers::ME = this;

    m_Duree = 0.0f;
}

TypeEvt Univers::GetTypeEvtActuel()
{
    if ( m_ExecHistoire != nullptr && m_ExecHistoire->GetExecEvtActuel() != nullptr)
        return m_ExecHistoire->EvtActuel()->m_TypeEvenement;

    return TE_Base;
}

void Univers::AjouterDuree(float duree)
{
    m_Duree += duree;
}

ExecHistoire* Univers::GetExecHistoire()
{
    return m_ExecHistoire;
}

Hist* Univers::GetHistoire()
{
    return m_Histoire;
}

GenHistoire* Univers::GetGenHistoire(QString idHistoire)
{
    return m_GensHistoire[idHistoire];
}

IPerso* Univers::GetPersoInterface()
{
    return m_Perso;
}

void Univers::RafraichirAffichage()
{
    if ( m_PersoAffiche)
        m_Perso->RafraichirAffichage();
}

bool Univers::LancerEvtEtOuEffetCourant()
{
    ExecEvt* evt_actuel = m_ExecHistoire->GetExecEvtActuel();
    ExecEffet* effet_actuel = m_ExecHistoire->GetExecEffetActuel();
    if ( evt_actuel == nullptr || evt_actuel == nullptr || effet_actuel == nullptr || effet_actuel == nullptr)
        return false;

    evt_actuel->LancerNoeud();
    //effet_actuel->LancerNoeud();

    //m_Histoire->RafraichirAffichageEvtEtOuEffet( evt_actuel, effet_actuel );
    return true;
}

void Univers::DeclencherEffetSuivant()
{
    QObject* declencheur = sender();
    if (declencheur)
        disconnect( declencheur, SIGNAL(clicked()), nullptr, nullptr );

    m_ExecHistoire->DeterminerPuisLancerNoeudSuivant();
}


void Univers::RafraichirAffichageLayouts(int largeur, int hauteur)
{
    if( largeur != -1)
    {
        this->setFixedSize(largeur, hauteur);
        this->ui->Fond->setFixedSize(largeur, hauteur);
        this->ui->histoireWidget->setFixedSize(largeur - m_Reglages.m_LargeurColonneGauche, hauteur);
        this->ui->persoWidget->setFixedSize(m_Reglages.m_LargeurColonneGauche, hauteur);
        this->ui->centralwidget->setFixedSize(largeur, hauteur);
    }
    //ui->centralwidget->layout()->update();
   // ui->centralwidget->layout()->activate();

    ui->Fond->layout()->update();
    ui->Fond->layout()->activate();

    this->layout()->update();
    this->layout()->activate();
}


Univers::~Univers()
{
    delete ui;
}
