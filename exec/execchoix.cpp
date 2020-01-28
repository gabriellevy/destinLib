#include "execchoix.h"
#include "ui_choix.h"
#include "../abs/univers.h"

using std::shared_ptr;
using std::make_shared;

ExecChoix::ExecChoix(std::shared_ptr<Choix> choix, QWidget *parent) :
    ExecNoeud(choix, parent),
    m_Choix(choix)
{
    ui = new Ui::Choix;

    ui->setupUi(this);

    ui->bouton->setText(choix->TexteAAfficher() );

    this->setCursor(Qt::PointingHandCursor);

    QObject::connect(ui->bouton, SIGNAL(clicked()),
                     this, SLOT(ExecuterNoeudSlot()));

    if ( m_Choix->m_CheminImg != "" )
    {
        m_Img.load(m_Choix->m_CheminImg);
    }
}

ExecChoix::ExecChoix(shared_ptr<ExecNoeud>, std::shared_ptr<Choix> choix, QWidget *parent):
    ExecChoix(choix, parent)
{

}

ExecChoix::ExecChoix(shared_ptr<ExecEffet> execEffet, std::shared_ptr<Choix> choix, QWidget *parent):
    ExecChoix(choix, parent)
{
    m_ExecEffet = execEffet;
}

ExecChoix::ExecChoix(shared_ptr<ExecLancerDe> lancerDe, std::shared_ptr<Choix> choix, QWidget *parent) :
    ExecChoix(choix, parent)
{
    m_ExecLancerDe = lancerDe;
}

void ExecChoix::RafraichirAffichageLayouts(int largeur, int)
{
    if( largeur != -1)
    {
        this->setFixedWidth(largeur);
    }
}

void ExecChoix::AfficherNoeud()
{
    ui->bouton->setText(m_Choix->TexteAAfficher() );
    ui->bouton->setFont( *Univers::BASE_FONT);

    if ( !m_Img.isNull() )
    {
        QIcon icone(m_Img);
         ui->bouton->setIcon(icone);
         ui->bouton->setIconSize(m_Img.rect().size());

        // si il n'y a pas de texte c'est un bouton uniquement icone
        if ( m_Choix->m_Texte == "")
            this->setFixedSize(m_Img.rect().size());
    }
}

bool ExecChoix::GestionTransition()
{
    Univers::ME->GetExecHistoire()->DeterminerPuisLancerNoeudSuivant(ExecNoeud::shared_from_this());
    return true;
}

shared_ptr<ExecNoeud> ExecChoix::GetExecNoeud()
{
    return std::static_pointer_cast<ExecNoeud>(shared_from_this());
}

void ExecChoix::ExecuterNoeudSlot(/*bool afficherNoeud, bool lancerNoeudSuivantSiRienAAfiicher*/)
{
    /*QObject* declencheur = sender();
    if (declencheur)
        disconnect( declencheur, SIGNAL(clicked()), nullptr, nullptr );*/

    this->ExecuterActionsNoeud(/*afficherNoeud, lancerNoeudSuivantSiRienAAfiicher*/);

    this->GestionTransition();
}

/*void ExecChoix::LancerNoeud()
{
    Q_ASSERT_X(false, "Choix::LancerNoeud", "On ne doit jamais lancer intégralement l'effet d'un bouton, il doit être soit affiché soit exécuté");
}*/

ExecChoix::~ExecChoix()
{
    delete ui;
}
