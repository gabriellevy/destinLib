#include "execevt.h"
#include "univers.h"
#include "ui_evt.h"
#include "execeffet.h"

ExecEvt::ExecEvt( Evt* evt, QWidget *parent) :
    ExecNoeud (evt, parent),
    ui(new Ui::Evt)
{
    ui->setupUi(this);

    m_ExecEffetActuel = nullptr;

    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details )
        ui->effetsWidget->setStyleSheet("background-color : rgb(0,0,255)");
}

ExecEvt::~ExecEvt()
{
    delete ui;
}

ExecEffet* ExecEvt::GetExecEffetActuel()
{
    if ( this->m_ExecEffetActuel == nullptr)
        this->SetEffetIndex(0);

    return this->m_ExecEffetActuel;
}

ExecLancerDe* ExecEvt::GetExecLancerDeActuel()
{
    if ( this->m_ExecEffetActuel == nullptr)
        this->SetEffetIndex(0);

    return this->m_ExecEffetActuel->m_ExecLancerDe;
}

ExecEffet* ExecEvt::SetEffetIndex(int index)
{
    return this->SetExecEffet(this->GetEvt()->m_Effets[index]);
}

ExecEffet* ExecEvt::SetExecEffet(QString effetId)
{
    Effet* effet = this->GetEvt()->TrouverEffet(effetId);
    return SetExecEffet(effet);
}

ExecEffet* ExecEvt::SetExecEffet(Effet* effet)
{
   if ( this->m_ExecEffetActuel == nullptr || this->m_ExecEffetActuel->GetEffet() != effet)
        return this->SetExecEffet(new ExecEffet(this, effet));
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



ExecEffet* ExecEvt::SetExecEffet(ExecEffet* exec_effet)
{
    if ( this->m_ExecEffetActuel == nullptr ||  this->m_ExecEffetActuel != exec_effet) {
        if ( m_ExecEffetActuel != nullptr)
        {
            m_ExecEffetActuel->NettoyageAffichage();
            m_ExecEffetActuel->hide();
            ui->effetsWidget->layout()->removeWidget(m_ExecEffetActuel);
        }
        m_ExecEffetActuel = exec_effet;
        //m_EffetActuel->LancerNoeud();

        ui->effetsWidget->layout()->addWidget(m_ExecEffetActuel);

        m_ExecEffetActuel->show();
        this->update();
        m_ExecEffetActuel->update();
    }

    return this->m_ExecEffetActuel;
}
