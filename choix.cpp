#include "choix.h"
#include "ui_choix.h"
#include "aventure.h"

/*Choix::Choix(QJsonObject choixJson, QWidget *parent) :
    QPushButton("", parent),
    Noeud(choixJson),
    ui(new Ui::Choix)
{
    ui->setupUi(this);

    this->setCursor(Qt::PointingHandCursor);

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(ExecuterNoeudSlot()));
}*/

Choix::Choix(QString text, QString cheminImg, QWidget *parent) :
    QPushButton(text, parent),
    Noeud(),
    ui(new Ui::Choix)
{
    ui->setupUi(this);

    this->setCursor(Qt::PointingHandCursor);

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(ExecuterNoeudSlot()));

    m_Text = text;

    if ( cheminImg != "" )
    {
        m_Img.load(cheminImg);
    }
}

void Choix::AfficherNoeud()
{
    this->setText(TexteAAfficher() );

    if ( !m_Img.isNull() )
    {
        QIcon icone(m_Img);
        this->setIcon(icone);
        this->setIconSize(m_Img.rect().size());

        // si il n'y a pas de texte c'est un bouton uniquement icone
        if ( m_Text == "")
            this->setFixedSize(m_Img.rect().size());
    }
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
