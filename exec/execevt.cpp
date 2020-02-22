#include "execevt.h"
#include "../abs/univers.h"
#include "ui_evt.h"
#include "execeffet.h"

using std::shared_ptr;
using std::make_shared;

ExecEvt::ExecEvt( shared_ptr<Evt> evt, QWidget *parent) :
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

shared_ptr<ExecEffet> ExecEvt::GetExecEffetActuel()
{
    if ( this->m_ExecEffetActuel == nullptr)
        this->SetEffetIndex(0);

    return this->m_ExecEffetActuel;
}

shared_ptr<ExecLancerDe> ExecEvt::GetExecLancerDeActuel()
{
    if ( this->m_ExecEffetActuel == nullptr)
        this->SetEffetIndex(0);

    return this->m_ExecEffetActuel->m_ExecLancerDe;
}

shared_ptr<ExecEffet> ExecEvt::SetEffetIndex(int index)
{
    return this->SetExecEffet(this->GetEvt()->m_Effets[index]);
}

shared_ptr<ExecEffet> ExecEvt::SetExecEffet(QString effetId)
{
    std::shared_ptr<Effet> effet = this->GetEvt()->TrouverEffet(effetId);
    return SetExecEffet(effet);
}

shared_ptr<ExecEffet> ExecEvt::SetExecEffet(shared_ptr<Effet> effet)
{
   if ( this->m_ExecEffetActuel == nullptr || this->m_ExecEffetActuel->GetEffet() != effet)
        return this->SetExecEffet(make_shared<ExecEffet>(effet));
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
    Univers::ME->GetExecHistoire()->DeterminerPuisLancerNoeudSuivant(ExecNoeud::shared_from_this());
}

shared_ptr<Evt> ExecEvt::GetEvt()
{
    return std::static_pointer_cast<Evt>(m_Noeud.lock());
}



shared_ptr<ExecEffet> ExecEvt::SetExecEffet(shared_ptr<ExecEffet> exec_effet)
{
    if ( this->m_ExecEffetActuel == nullptr ||  this->m_ExecEffetActuel != exec_effet) {
        if ( m_ExecEffetActuel != nullptr)
        {
            m_ExecEffetActuel->NettoyageAffichage();
            m_ExecEffetActuel->hide();
            ui->effetsWidget->layout()->removeWidget(m_ExecEffetActuel.get());
        }
        m_ExecEffetActuel = exec_effet;
        //m_EffetActuel->LancerNoeud();

        ui->effetsWidget->layout()->addWidget(m_ExecEffetActuel.get());

        m_ExecEffetActuel->show();
        this->update();
        m_ExecEffetActuel->update();
    }

    return this->m_ExecEffetActuel;
}
