#include "choix.h"
#include "ui_choix.h"
#include "aventure.h"

Choix::Choix(QJsonObject choixJson, QWidget *parent) :
    QPushButton("", parent),
    Noeud(choixJson/*, parent*/),
    ui(new Ui::Choix)
{
    ui->setupUi(this);

    this->setCursor(Qt::PointingHandCursor);

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(ExecuterNoeudSlot()));
}

Choix::Choix(QString text, QWidget *parent) :
    QPushButton(text, parent),
    Noeud(),
    ui(new Ui::Choix)
{
    ui->setupUi(this);

    this->setCursor(Qt::PointingHandCursor);

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(ExecuterNoeudSlot()));

    m_Text = text;
}

void Choix::AfficherNoeud()
{
    this->setText(TexteAAfficher() );
}

bool Choix::AQuelqueChoseAAfficher()
{
    return false;
}

bool Choix::GestionTransition()
{
    Aventure::ME->GetHistoire()->DeterminerPuisLancerEffetSuivant(this);
    return true;
}


void Choix::ExecuterNoeudSlot(/*bool afficherNoeud, bool lancerNoeudSuivantSiRienAAfiicher*/)
{
    /*QObject* declencheur = sender();
    if (declencheur)
        disconnect( declencheur, SIGNAL(clicked()), nullptr, nullptr );*/

    this->ExecuterActionsNoeud(/*afficherNoeud, lancerNoeudSuivantSiRienAAfiicher*/);

    this->GestionTransition();
}

void Choix::LancerNoeud()
{
    Q_ASSERT_X(false, "Choix::LancerNoeud", "On ne doit jamais lancer intégralement l'effet d'un bouton, il doit être soit affiché soit exécuté");
}

Choix::~Choix()
{
    delete ui;
}
