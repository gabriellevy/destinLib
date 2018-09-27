#include "aventure.h"
#include "ui_aventure.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>

const QFont* Aventure::BASE_FONT = new QFont("Verdana", 10);
const QFont* Aventure::TITRE_FONT = new QFont("Verdana", 20);
QString Aventure::CHEMIN = "";
Aventure* Aventure::ME;

Aventure::Aventure(QWidget *parent, ModeAffichage modeAffichage):QMainWindow(parent),
    ui(new Ui::Aventure), m_ModeAffichage(modeAffichage)
{
    InstallerInterface();

    m_Lecteur = new QMediaPlayer;

    // positionner l'interface
    /*m_Perso = new IPerso(ui->persoWidget);
    ui->persoWidget->layout()->addWidget(m_Perso);
    //m_Perso->show();
    m_Histoire = new Histoire(ui->histoireWidget);
    ui->histoireWidget->layout()->addWidget(m_Histoire);*/

    //LancerAventure();
}

Aventure::Aventure(QString cheminAventure, ModeAffichage modeAffichage, QString firstEvt, QString premierEffet, QWidget *parent) :
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
}

void Aventure::InstallerInterface()
{
    ui->setupUi(this);

    Aventure::ME = this;

    m_EtatPartie = EP_Chargement;
    m_Duree = 0.0f;
}

bool Aventure::ExtraireAventure(QString cheminAventure)
{
    QFile fichier(cheminAventure);
    Aventure::CHEMIN = cheminAventure.left(cheminAventure.lastIndexOf('/') +1);

    if ( !fichier.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "erreur", "Impossible d'ouvrir le fichier aventure !");
        return false;
    }

    QByteArray avData = fichier.readAll();

    m_AvJson = QJsonDocument::fromJson(avData).object();

    return true;
}

void Aventure::LancerAventure(QString premierEvt, QString premierEffet)
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
}

bool Aventure::EstEnModeHistoire()
{
    return (Aventure::ME->GetEtatPartie() == EP_Deroulement);
}

void Aventure::SetEtatPartie(EtatPartie etat)
{
    m_EtatPartie = etat;
}

TypeEvt Aventure::GetTypeEvtActuel()
{
    if ( m_Histoire != nullptr && m_Histoire->m_CurrentEvt != nullptr)
        return m_Histoire->m_CurrentEvt->m_TypeEvenement;

    /*Q_ASSERT_X(false,
               "Choix::GetTypeEvtActuel",
               "l'événement actuel ne semble pas accessible");*/
    return TE_Base;
}

EtatPartie Aventure::GetEtatPartie()
{
    return m_EtatPartie;
}

void Aventure::AjouterDuree(float duree)
{
    m_Duree += duree;
}

EtatPartie Aventure::ChangerEtatPartie(QString nouvelEtatPartie)
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

Histoire* Aventure::GetHistoire()
{
    return m_Histoire;
}

IPerso* Aventure::GetPersoInterface()
{
    return m_Perso;
}

bool Aventure::LancerEvtEtOuEffetCourant()
{
    Evt* evt_actuel = m_Histoire->EvtActuel();
    Effet* effet_actuel = m_Histoire->EffetActuel();
    if ( evt_actuel == 0 || evt_actuel == nullptr || effet_actuel == 0 || effet_actuel == nullptr)
        return false;

    evt_actuel->LancerNoeud();
    effet_actuel->LancerNoeud();

    //m_Histoire->RafraichirAffichageEvtEtOuEffet( evt_actuel, effet_actuel );
    return true;
}

void Aventure::DeclencherEffetSuivant()
{
    QObject* declencheur = sender();
    if (declencheur)
        disconnect( declencheur, SIGNAL(clicked()), nullptr, nullptr );

    m_Histoire->DeterminerPuisLancerEffetSuivant();
}



Aventure::~Aventure()
{
    delete ui;
}
