#include "execlancerde.h"
#include "ui_lancerde.h"
#include "univers.h"

ExecLancerDe::ExecLancerDe(ExecEffet* execEffet, LancerDe* lancerDe, QWidget *parent) :
    ExecNoeud(lancerDe, parent),
    m_LancerDe(lancerDe),
    m_ExecEffet(execEffet),
    ui(new Ui::LancerDe)
{
    ui->setupUi(this);

    ui->bouton->setText(lancerDe->TexteAAfficher() );

    QObject::connect(ui->bouton, SIGNAL(clicked()),
                     this, SLOT(ExecuterNoeudSlot()));
}

ExecLancerDe::~ExecLancerDe()
{
    delete ui;
}

void ExecLancerDe::RafraichirAffichageLayouts(int largeur, int)
{
    if( largeur != -1)
    {
        this->setFixedWidth(largeur);
    }
}

void ExecLancerDe::AfficherNoeud()
{
    ui->bouton->setText(m_LancerDe->TexteAAfficher() );
    ui->bouton->setFont( *Univers::BASE_FONT);

    if ( !m_Img.isNull() )
    {
        QIcon icone(m_Img);
         ui->bouton->setIcon(icone);
         ui->bouton->setIconSize(m_Img.rect().size());

        // si il n'y a pas de texte c'est un bouton uniquement icone
        if ( m_LancerDe->m_Text == "")
            this->setFixedSize(m_Img.rect().size());
    }
}

bool ExecLancerDe::GestionTransition()
{
    // HEUUU je sais pas encore exactement
    return true;
}

ExecNoeud* ExecLancerDe::GetExecNoeud()
{
    return static_cast<ExecNoeud*>(this);
}

void ExecLancerDe::ExecuterNoeudSlot()
{
    this->ExecuterActionsNoeud();

    this->GestionTransition();
}
