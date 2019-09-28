#include "execlancerde.h"
#include "ui_lancerde.h"
#include "univers.h"
#include "aleatoire.h"
#include "execchoix.h"

ExecLancerDe::ExecLancerDe(ExecEffet* execEffet, LancerDe* lancerDe, QWidget *parent) :
    ExecNoeud(lancerDe, parent),
    m_LancerDe(lancerDe),
    m_ExecEffet(execEffet),
    ui(new Ui::LancerDe)
{
    ui->setupUi(this);

    ui->bouton->setText(lancerDe->TexteAAfficher() );
    m_Noeud = lancerDe;

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
    ExecNoeud::AfficherNoeud();

    ui->bouton->setText(m_LancerDe->TexteAAfficher() );
    ui->bouton->setFont( *Univers::BASE_FONT);

    if ( !m_Img.isNull() )
    {
        QIcon icone(m_Img);
         ui->bouton->setIcon(icone);
         ui->bouton->setIconSize(m_Img.rect().size());

        // si il n'y a pas de texte c'est un bouton uniquement icone
        if ( m_LancerDe->m_Texte == "")
            this->setFixedSize(m_Img.rect().size());
    }
}

bool ExecLancerDe::GestionTransition()
{
    ExecNoeud::GestionTransition();
    // tant qu'on a n'a pas de résultat ni de résultat mettant fin au lancer on reste ici :
    if ( m_ResExecution == nullptr || m_ResExecution->m_RestAffiche)
        return false;

    this->m_ExecEffet->GestionTransition();

    return true;
}

ExecNoeud* ExecLancerDe::GetExecNoeud()
{
    return static_cast<ExecNoeud*>(this);
}

int ExecLancerDe::GetTotalRes()
{
    int res = 0;
    for ( int i = 0 ; i < m_Res.length() ; ++i) {
        res += m_Res[i];
    }
    return res;
}

void ExecLancerDe::NettoyageAffichage()
{
    for ( ExecChoix* choix: this->m_ExecChoix ) {
        this->layout()->removeWidget(choix);
    }
}


void ExecLancerDe::AjouterAuxBoutonsHoriz(ExecNoeud* execNoeud)
{
    ui->horizontalLayoutBoutons->layout()->addWidget(execNoeud);
}

void ExecLancerDe::AjouterAuxBoutonsVertic(ExecNoeud* execNoeud)
{
    ui->layoutBoutons->layout()->addWidget(execNoeud);
}

void ExecLancerDe::ExecuterNoeudSlot()
{
    this->ExecuterActionsNoeud();

    m_Res.clear();
    for ( int i = 0 ; i < m_LancerDe->m_NbDes ; ++i) {
        m_Res.push_back(Aleatoire::GetAl()->EntierEntreAEtB(1, 6));
    }

    //QVector<QString> args = {};
    QString resExec = ui->texteLancerDe->text();
    if ( resExec != "" )
        resExec += "\n\n";

    if ( m_ResExecution != nullptr)
        delete m_ResExecution;

    m_ResExecution = m_LancerDe->m_Callback(GetTotalRes()/*, args*/);
    resExec += m_ResExecution->m_TexteRes;

    ui->texteLancerDe->setText(resExec);

    // l'exécution est terminée : on cache le bouton et on repasse la responsabilité de la transition à l'effet "père"
    if ( !m_ResExecution->m_RestAffiche) {
        ui->bouton->hide();
        this->GestionTransition();
    }
}
