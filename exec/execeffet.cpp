#include "execeffet.h"
#include "execchoix.h"
#include "execevt.h"
#include "ui_effet.h"
#include "../abs/univers.h"
#include <QMovie>
#include <QTimer>
#include "../gestionnairecarac.h"
#include "execlancerde.h"
#include "../abs/lancerde.h"

ExecEffet::ExecEffet(ExecEvt* exec_evt, std::shared_ptr<Effet> effet, QWidget *parent):
    ExecNoeud(effet, parent),
    m_ExecEvt(exec_evt),
    ui(new Ui::Effet)
{
    m_Noeud = effet;

    if ( effet->m_ImgPath != "" )
        this->ChargerImage(effet->m_ImgPath);

    ui->setupUi(this);
    ui->boutonContinuer->hide();
    ui->glisseur->hide();
    ui->labelGlisseur->hide();

    if ( GetEffet()->m_Glisseur != nullptr) {
        this->AjouterGlisseur();
    }
}

void ExecEffet::NettoyageAffichage()
{
    /*while ( QWidget* w = findChild<QWidget*>() )
        delete w;*/
    for ( ExecChoix* choix: this->m_ExecChoix ) {
        this->layout()->removeWidget(choix);
    }
    ui->lancerDeWidget->layout()->removeWidget(m_ExecLancerDe);
}

void ExecEffet::FinExecutionNoeud()
{
    ExecNoeud::FinExecutionNoeud();

    /*if ( m_ElseNoeud != nullptr)
        m_ElseNoeud->FinExecutionNoeud();*/
}

std::shared_ptr<Effet> ExecEffet::GetEffet()
{
    if ( m_Noeud != nullptr)
        return std::static_pointer_cast<Effet>(m_Noeud);
    else return nullptr;
}

ExecEffet::~ExecEffet()
{
    delete ui;
}

void ExecEffet::FinChrono()
{
    Univers::ME->GetExecHistoire()->DeterminerPuisLancerNoeudSuivant(this);
}

void ExecEffet::AfficherBoutonSuivant()
{
    ui->boutonContinuer->show();
    ui->boutonContinuer->setCursor(Qt::PointingHandCursor);

    QObject::connect(ui->boutonContinuer, SIGNAL(clicked()), Univers::ME, SLOT(DeclencherEffetSuivant()));
}

bool ExecEffet::GestionTransition()
{
    bool est_ce_que_l_interface_vers_suite_est_affichee = false;

    // si il y a un lancer de dé dans cet effet, l'afficher : (prioritaire sur les choix)
    if ( !est_ce_que_l_interface_vers_suite_est_affichee && GetEffet()->m_LancerDe != nullptr )
    {
        // (sauf si il est déjà fini)
        if ( m_ExecLancerDe != nullptr && !m_ExecLancerDe->m_ResExecution->m_RestAffiche )
        {
            //delete m_ExecLancerDe;
            //m_ExecLancerDe->NettoyageAffichage();
            //ui->lancerDeWidget->layout()->removeWidget(m_ExecLancerDe);
            //m_ExecLancerDe = nullptr;
        }
        else {
            /*this->m_ExecNoeudActuel = */this->SetExecLancerDe(GetEffet()->m_LancerDe);
            est_ce_que_l_interface_vers_suite_est_affichee = true;
        }
    }

    if ( !est_ce_que_l_interface_vers_suite_est_affichee )
        est_ce_que_l_interface_vers_suite_est_affichee = !ExecNoeud::GestionTransition();

    if ( !est_ce_que_l_interface_vers_suite_est_affichee )
    {
        // si l'effet est dans une boucle while il doit s'afficher encore tant que la condition sera remplie
        /*if ( m_RepeatWhileConditions.size() > 0 &&
             Condition::TesterTableauDeConditions(m_RepeatWhileConditions) )
        {
            // A FAIRE MAT : ?? peut-être faudrait-t'il raffraichir l'affichage de l'effet ?
            LancerNoeud();
        }
        else*/


        if ( GetEffet()->AQuelqueChoseAAfficher() )
        {
            AfficherBoutonSuivant();
            est_ce_que_l_interface_vers_suite_est_affichee = true;
        }
        else
        {
            Univers::ME->GetExecHistoire()->DeterminerPuisLancerNoeudSuivant(this);
            return false;
        }
    }

    return !est_ce_que_l_interface_vers_suite_est_affichee;
}


void ExecEffet::AjouterAuxBoutonsHoriz(ExecNoeud* execNoeud)
{
    ui->horizontalLayoutBoutons->layout()->addWidget(execNoeud);
}

void ExecEffet::AjouterAuxBoutonsVertic(ExecNoeud* execNoeud)
{
    ui->layoutBoutons->layout()->addWidget(execNoeud);
}

ExecLancerDe* ExecEffet::SetExecLancerDe(std::shared_ptr<LancerDe> lancer_de)
{
   if ( this->m_ExecLancerDe == nullptr || this->m_ExecLancerDe->m_LancerDe != lancer_de)
        return this->SetExecLancerDe(new ExecLancerDe(this, lancer_de));
   return this->m_ExecLancerDe;
}

void ExecEffet::AfficherNoeud()
{
    ExecNoeud::AfficherNoeud();

    // cette fonction peut être appelée pour rafraichir un affichage donc on cache tout avant de le réafficher éventuellement
    ui->titreEffet->hide();
    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details && GetEffet()->m_Nom != "")
    {
        ui->titreEffet->show();
        ui->titreEffet->setFont( *Univers::TITRE_FONT);
        ui->titreEffet->setText(GetEffet()->m_Nom);
    }

    // affichage du texte
    QString prevTexte = ui->texteEffet->text();
    ui->texteEffet->hide();
    if ( GetEffet()->TexteAAfficher() != "")
    {
        ui->texteEffet->show();
        ui->texteEffet->setFont( *Univers::BASE_FONT);
        if ( Univers::ME->GetExecHistoire()->m_Histoire->m_ModeDeroulement == ModeDeroulement::Automatique ) {
            Univers::ME->GetExecHistoire()->m_Historique.m_Textes.push_back(GetEffet()->TexteAAfficher());
            ui->texteEffet->setText(
                        Univers::ME->GetExecHistoire()->m_Historique.GetHistoriqueTotalAsStr());
        }
        else {
            ui->texteEffet->setText(GetEffet()->TexteAAfficher());
        }
        ui->texteEffet->setWordWrap(true);
    }

    ui->imageLabel->hide();

    // affichage image/ film
    bool afficheImg = !m_Img.isNull();
    bool afficheFilm = (m_Film != nullptr);
    if ( afficheImg || afficheFilm )
    {
        ui->imageLabel->show();

        if ( afficheImg)
            ui->imageLabel->setPixmap(m_Img);
        else if ( afficheFilm)
        {
            ui->imageLabel->setMovie(m_Film);
            m_Film->start();
        }

        // réduction de l'image à la taille de la fenêtre si elle est plus large :
        /*int largeurImg = this->width() - 25; //25 pixels environ à cause de la barre de défilement à droite...
        if ( (afficheImg &&  m_Img.width() > largeurImg)
           || (afficheFilm &&  m_Film->scaledSize().width() > largeurImg) )
        {
            int w = largeurImg;
            int h = ui->imageLabel->heightForWidth(w);
            QSize AdjustSize = QSize(w, h);
            if ( w != -1 && h != -1 ) {
                ui->imageLabel->setMinimumSize(AdjustSize);
                ui->imageLabel->setMaximumSize(AdjustSize);
            }
            else {

            }
        }*/
    }

    // affichage du lancement de dé
    if ( GetEffet()->m_LancerDe != nullptr )
    {
        //this->SetExecLancerDe(exec_lancer_de);
    }

    Hist* hist = Univers::ME->GetExecHistoire()->m_Histoire;
    // déclenchement du chrono pour cet effet si il en a un :
    if ( GetEffet()->m_MsChrono == -1 &&
         (hist->m_ModeDeroulement == ModeDeroulement::Automatique && hist->m_MsDureeDefilement != -1 ))
    {
        GetEffet()->m_MsChrono = hist->m_MsDureeDefilement;
    }
    if ( GetEffet()->m_MsChrono != -1 )
    {
        QTimer::singleShot(GetEffet()->m_MsChrono, this, SLOT(FinChrono()));
    }

    Univers::ME->GetExecHistoire()->GetExecEvtActuel()->RafraichirAffichageLayouts();
}

ExecLancerDe* ExecEffet::SetExecLancerDe(ExecLancerDe* exec_lancer_de)
{
    if ( this->m_ExecLancerDe == nullptr ||  this->m_ExecLancerDe != exec_lancer_de) {
        if ( m_ExecLancerDe != nullptr)
        {
            //m_ExecLancerDe->NettoyageAffichage();
            m_ExecLancerDe->hide();
            ui->lancerDeWidget->layout()->removeWidget(m_ExecLancerDe);
        }
        m_ExecLancerDe = exec_lancer_de;
        m_ExecLancerDe->LancerNoeud();

        ui->lancerDeWidget->layout()->addWidget(m_ExecLancerDe);

        m_ExecLancerDe->show();
        this->update();
        m_ExecLancerDe->update();
    }

    return this->m_ExecLancerDe;
}


void ExecEffet::ChargerImage(QString chemin)
{
    if ( chemin != "" )
    {
        // si l'image est un gif je pars du principe que c'est un gif potentiellement animé et ça devient un movie au lieu d'une image
        if ( chemin.endsWith(".gif"))
        {
            m_Film = new QMovie(chemin);
        }
        else
        {
            m_Img.load(chemin);
        }
    }
}


void ExecEffet::valeurGlisseurAChange()
{
    IPerso::GetPersoCourant()->SetValeurACaracId(
                GetEffet()->m_Glisseur->m_IdCaracAssociee,
                QString::number(ui->glisseur->value()));
}



void ExecEffet::RafraichirAffichageLayouts(int largeur, int )
{
    if( largeur != -1)
    {
        this->setFixedWidth(largeur - Univers::ME->m_Reglages.m_LargeurColonneGauche);
    }
    ui->horizontalLayout->layout()->update();
    ui->horizontalLayout->layout()->activate();

    this->layout()->update();
    this->layout()->activate();
}

Glisseur* ExecEffet::AjouterGlisseur()
{
    // A FAIRE transférer ça dans afficher noeud ?
    ui->glisseur->show();
    ui->labelGlisseur->show();

    ui->glisseur->setMinimum(static_cast<int>(GetEffet()->m_Glisseur->m_Minimum));
    ui->glisseur->setMaximum(static_cast<int>(GetEffet()->m_Glisseur->m_Maximum));
    ui->glisseur->setSliderPosition(static_cast<int>(GetEffet()->m_Glisseur->m_ValeurDepart));
    ui->labelGlisseur->setNum(static_cast<int>(GetEffet()->m_Glisseur->m_ValeurDepart));
    connect(ui->glisseur,SIGNAL(valueChanged(int)),this,SLOT(valeurGlisseurAChange(int)));

    return GetEffet()->m_Glisseur;
}
