#include "execlancerde.h"
#include "ui_lancerde.h"
#include "../abs/univers.h"
#include "../aleatoire.h"
#include "execeffet.h"
#include "execchoix.h"

using std::shared_ptr;
using std::make_shared;

ExecLancerDe::ExecLancerDe(shared_ptr<ExecEffet> execEffet, std::shared_ptr<LancerDe> lancerDe, QWidget *parent) :
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

    m_ImgDe[0].load(":/images/des/1.png");
    m_ImgDe[1].load(":/images/des/2.png");
    m_ImgDe[2].load(":/images/des/3.png");
    m_ImgDe[3].load(":/images/des/4.png");
    m_ImgDe[4].load(":/images/des/5.png");
    m_ImgDe[5].load(":/images/des/6.png");
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

    this->m_ExecEffet.lock()->GestionTransition();

    return true;
}

shared_ptr<ExecNoeud> ExecLancerDe::GetExecNoeud()
{
    return std::static_pointer_cast<ExecNoeud>(ExecLancerDe::shared_from_this());
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
    for ( shared_ptr<ExecChoix> choix: this->m_ExecChoix ) {
        this->layout()->removeWidget(choix.get());
    }
}


void ExecLancerDe::AjouterAuxBoutonsHoriz(shared_ptr<ExecNoeud> execNoeud)
{
    ui->horizontalLayoutBoutons->layout()->addWidget(execNoeud.get());
}

void ExecLancerDe::AjouterAuxBoutonsVertic(shared_ptr<ExecNoeud> execNoeud)
{
    ui->layoutBoutons->layout()->addWidget(execNoeud.get());
}

shared_ptr<ExecChoix> ExecLancerDe::AjoutChoixGoToEffet(QString texte, QString idDest)
{
    shared_ptr<Choix> choix = make_shared<Choix>(texte);
    choix->m_GoToEffetId = idDest;
    shared_ptr<ExecChoix> exec = make_shared<ExecChoix>(ExecNoeud::shared_from_this(), choix, this);
    this->m_ExecChoix.push_back(exec);
    return exec;
}

void ExecLancerDe::ChangerIntituleBouton(QString texte)
{
    ui->bouton->setText(texte);
}

void ExecLancerDe::ExecuterNoeudSlot()
{
    this->ExecuterActionsNoeud();

    // calcul des résultats de dés
    m_Res.clear();
    for ( int i = 0 ; i < m_LancerDe->m_NbDes ; ++i) {
        m_Res.push_back(Aleatoire::GetAl()->EntierEntreAEtB(1, 6));
    }

    QString resExec = ui->texteLancerDe->text();
    if ( resExec != "" )
        resExec += "\n";
        //resExec = "";

    // nettoyage des restes des lancers précédents
    /*if ( m_ResExecution != nullptr)
        delete m_ResExecution;*/
    m_ResExecution = nullptr;

    ui->de1->clear();
    ui->de2->clear();
    ui->de3->clear();
    ui->de4->clear();
    ui->de5->clear();
    ui->de6->clear();

    m_ResExecution = m_LancerDe->m_Callback(GetTotalRes()/*, args*/);
    resExec += m_ResExecution->m_TexteRes;

    // afficher le résultat
    ui->texteLancerDe->setText(resExec);
    for ( int i = 0 ; i < m_Res.length() ; ++i) {
        int res = m_Res[i];
        switch(i+1) {
        case 1 : ui->de1->setPixmap(m_ImgDe[res-1]); break;
        case 2 : ui->de2->setPixmap(m_ImgDe[res-1]); break;
        case 3 : ui->de3->setPixmap(m_ImgDe[res-1]); break;
        case 4 : ui->de4->setPixmap(m_ImgDe[res-1]); break;
        case 5 : ui->de5->setPixmap(m_ImgDe[res-1]); break;
        case 6 : ui->de6->setPixmap(m_ImgDe[res-1]); break;
        }
    }

    // l'exécution est terminée : on cache le bouton et on repasse la responsabilité de la transition à l'effet "père"
    if ( !m_ResExecution->m_RestAffiche) {
        ui->bouton->hide();
        this->GestionTransition();
    }
}
