#include "execevt.h"
#include "univers.h"
#include "ui_evt.h"
#include "execeffet.h"

ExecEvt::ExecEvt( Evt* evt, QWidget *parent) :
    ExecNoeud (parent),
    ui(new Ui::Evt)
{
    ui->setupUi(this);

    m_ExecEffetActuel = nullptr;

    m_Noeud = evt;

    //ui->groupBox->setStyleSheet("background-color: rgba(0,0,0,0)");
    //ui->effetsWidget->setStyleSheet("background-color: rgba(0,0,0,0)");

    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details )
        ui->effetsWidget->setStyleSheet("background-color : rgb(0,0,255)");

    //ui->effetsWidget->layout()->setAlignment(Qt::AlignBottom);
    //((QVBoxLayout*)(ui->effetsWidget->layout()))->addStretch();

}

ExecEvt::~ExecEvt()
{
    delete ui;
}

ExecEffet* ExecEvt::SetEffetIndex(int index)
{
    return this->SetExecEffet(this->GetEvt()->m_Effets[index]);
}

ExecEffet* ExecEvt::SetExecEffet(Effet* effet)
{
    if ( m_ExecEffetActuel!= nullptr)
        delete this->m_ExecEffetActuel;
    this->m_ExecEffetActuel = new ExecEffet(this, effet);

    return this->m_ExecEffetActuel;
}


void ExecEvt::Clean()
{
    // nécessaire ?
    /*QLayoutItem* item;
    while ( ( item = this->layout()->takeAt(0) ) != 0 )
    {
        delete item;
    }*/
}

void ExecEvt::AfficherNoeud()
{
    if ( GetEvt()->m_CheminImgFond != "")
        Univers::ME->AppliquerFond(GetEvt()->m_CheminImgFond);
    ui->titreEvt->setText(GetEvt()->m_Nom);
    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details && GetEvt()->m_Nom != "")
    {
        ui->titreEvt->show();
        ui->titreEvt->setFont( *Univers::TITRE_FONT);
    }
    else
        ui->titreEvt->hide();

    ui->textEvt->setText( GetEvt()->TexteAAfficher() );
    if ( GetEvt()->TexteAAfficher() != "")
    {
        ui->textEvt->show();
        ui->textEvt->setFont( *Univers::BASE_FONT);
        ui->textEvt->setWordWrap(true);
    }
    else
        ui->textEvt->hide();

    RafraichirAffichageLayouts();
}

void ExecEvt::RafraichirAffichageLayouts(int largeur, int )
{
    if( largeur != -1)
    {

    }
    ui->EvtWidget->layout()->update();
    ui->EvtWidget->layout()->activate();
    ui->effetsWidget->adjustSize();

    ui->effetsWidget->layout()->update();
    ui->effetsWidget->layout()->activate();
    ui->effetsWidget->adjustSize();
}

bool ExecEvt::GestionTransition()
{
    Q_ASSERT_X(false, "Noeud::GestionTransition", "Je ne crois pas que la gestion de transition devrait se faire dans Evt si ?");
    return true;
}

void ExecEvt::LancerNoeud()
{
    this->AfficherNoeud();

    this->ExecuterActionsNoeud();

    // un evt ne suffit pas à un affichage et une pause : il faut lancer l'effet suivant automatiquement
    Univers::ME->GetExecHistoire()->DeterminerPuisLancerNoeudSuivant(this);
}

Evt* ExecEvt::GetEvt()
{
    return static_cast<Evt*>(m_Noeud);
}

void ExecEvt::RafraichirAffichageEffet(ExecEffet* effet)
{
    if ( m_ExecEffetActuel != nullptr)
    {
        //ui->effetsWidget->layout()->removeWidget(m_EffetActuel);
        m_ExecEffetActuel->hide();
    }
    m_ExecEffetActuel = effet;
    //m_EffetActuel->LancerNoeud();

    ui->effetsWidget->layout()->addWidget(m_ExecEffetActuel);

    m_ExecEffetActuel->show();
    this->update();
    m_ExecEffetActuel->update();
}
