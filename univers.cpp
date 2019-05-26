#include "univers.h"
#include "ui_univers.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>

const QFont* Univers::BASE_FONT = new QFont("Verdana", 10);
const QFont* Univers::TITRE_FONT = new QFont("Verdana", 20);
QString Univers::CHEMIN = "";
Univers* Univers::ME;

Univers::Univers(QWidget *parent, ModeAffichage modeAffichage):QMainWindow(parent),
    ui(new Ui::Univers), m_ModeAffichage(modeAffichage)
{
    InstallerInterface();
    m_Perso = new IPerso(ui->persoWidget);
}

void Univers::LancerHistoire(QString /*premierEvt*/, QString /*premierEffet*/, bool BarreDeCote)
{
    this->AfficherHistoire(ui->histoireWidget);

    m_Perso->RafraichirAffichage();

    // positionner l'interface
    ui->persoWidget->layout()->addWidget(m_Perso);
    m_Perso->show();

    m_Lecteur = new QMediaPlayer;

    if ( !BarreDeCote)
        ui->persoWidget->hide();

    // positionner l'interface
    /*m_Perso = new IPerso(ui->persoWidget);
    ui->persoWidget->layout()->addWidget(m_Perso);
    //m_Perso->show();
    m_Histoire = new Histoire(ui->histoireWidget);
    ui->histoireWidget->layout()->addWidget(m_Histoire);*/

    //LancerAventure();


    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details )
        ui->histoireWidget->setStyleSheet("background-color : rgb(255,0,0)");
}

void Univers::ExecuterGenerateurHistoire(QWidget *parent)
{
    m_GenHistoire = new GenHistoire(parent);
    m_Histoire = m_GenHistoire->GenererHistoire();
}


void Univers::AfficherHistoire(QWidget *parent)
{
    this->setWindowTitle(m_Histoire->GetTitre());
    parent->layout()->addWidget(m_Histoire);
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

    m_EtatPartie = EP_Chargement;
    m_Duree = 0.0f;
}

/*bool Univers::ExtraireAventure(QString cheminAventure)
{
    QFile fichier(cheminAventure);
    Univers::CHEMIN = cheminAventure.left(cheminAventure.lastIndexOf('/') +1);

    if ( !fichier.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "erreur", "Impossible d'ouvrir le fichier aventure !");
        return false;
    }

    QByteArray avData = fichier.readAll();

    m_AvJson = QJsonDocument::fromJson(avData).object();

    return true;
}*/

/*void Univers::LancerAventure(QString premierEvt, QString premierEffet)
{
    if ( m_AvJson.contains("titre") && m_AvJson["titre"].isString() )
        this->setWindowTitle(m_AvJson["titre"].toString());
    else
        QMessageBox::warning(this, "erreur", "Pas de titre dans ce fichier !");

    if ( m_AvJson.contains("heros") && m_AvJson["heros"].isArray() )
    {
        m_Perso->Rafraichir(m_AvJson["heros"].toArray());
    }
    else
        QMessageBox::warning(this, "erreur", "Pas de heros dans ce fichier aventure !");

    m_Histoire->Generer(m_AvJson);

    if ( premierEvt != "" )
        m_Histoire->SetCurrentEvtId( premierEvt ) ;
    if ( premierEffet!= "" )
        m_Histoire->SetEffetIndex(m_Histoire->DeterminerIndexEffet(premierEffet) );

    m_EtatPartie = EP_Deroulement;
    LancerEvtEtOuEffetCourant();
}*/

bool Univers::EstEnModeHistoire()
{
    return (Univers::ME->GetEtatPartie() == EP_Deroulement);
}

void Univers::SetEtatPartie(EtatPartie etat)
{
    m_EtatPartie = etat;
}

TypeEvt Univers::GetTypeEvtActuel()
{
    if ( m_Histoire != nullptr && m_Histoire->EvtActuel() != nullptr)
        return m_Histoire->EvtActuel()->m_TypeEvenement;

    return TE_Base;
}

EtatPartie Univers::GetEtatPartie()
{
    return m_EtatPartie;
}

void Univers::AjouterDuree(float duree)
{
    m_Duree += duree;
}

EtatPartie Univers::ChangerEtatPartie(QString nouvelEtatPartie)
{
    if (nouvelEtatPartie == "fin_de_partie" )
        m_EtatPartie = EP_FinPartie;
    else
    {
        QString msg = "Nouvel état de partie non reconnu : " + nouvelEtatPartie ;
        Q_ASSERT_X(false, "ChangerEtatPartie", msg.toStdString().c_str());
    }

    return m_EtatPartie;
}

ExecHistoire* Univers::GetHistoire()
{
    return m_Histoire;
}

GenHistoire* Univers::GetGenHistoire()
{
    return m_GenHistoire;
}

IPerso* Univers::GetPersoInterface()
{
    return m_Perso;
}

bool Univers::LancerEvtEtOuEffetCourant()
{
    Evt* evt_actuel = m_Histoire->EvtActuel();
    Effet* effet_actuel = m_Histoire->EffetActuel();
    if ( evt_actuel == nullptr || evt_actuel == nullptr || effet_actuel == 0 || effet_actuel == nullptr)
        return false;

    evt_actuel->LancerNoeud();
    effet_actuel->LancerNoeud();

    //m_Histoire->RafraichirAffichageEvtEtOuEffet( evt_actuel, effet_actuel );
    return true;
}

void Univers::DeclencherEffetSuivant()
{
    QObject* declencheur = sender();
    if (declencheur)
        disconnect( declencheur, SIGNAL(clicked()), nullptr, nullptr );

    m_Histoire->DeterminerPuisLancerNoeudSuivant();
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
