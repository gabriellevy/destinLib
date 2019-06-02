#include "execeffet.h"
#include "execchoix.h"
#include "execevt.h"
#include "ui_effet.h"
#include "univers.h"
#include <QMovie>
#include <QTimer>

ExecEffet::ExecEffet(ExecEvt* exec_evt, Effet* effet, QWidget *parent):
    ExecNoeud(),
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

void ExecEffet::FinExecutionNoeud()
{
    ExecNoeud::FinExecutionNoeud();

    /*if ( m_ElseNoeud != nullptr)
        m_ElseNoeud->FinExecutionNoeud();*/
}

Effet* ExecEffet::GetEffet()
{
    return static_cast<Effet*>(m_Noeud);
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

    // si l'effet contient un choix on l'affiche et on considère que le passage vers l'effet suivant est géré par les choix
    if ( m_ExecChoix.size() > 0 )
    {
        for ( int i = 0 ; i < m_ExecChoix.size() ; ++i)
        {
            m_ExecChoix[i]->GetExecNoeud()->hide();
            if ( m_ExecChoix[i]->m_Choix->TesterConditions())
            {
                if ( GetEffet()->m_OrientationAffichageChoix == OrientationAffichageChoix::oac_vertical)
                    ui->layoutBoutons->layout()->addWidget(m_ExecChoix[i]->GetExecNoeud());
                else
                    ui->horizontalLayoutBoutons->layout()->addWidget(m_ExecChoix[i]->GetExecNoeud());
                m_ExecChoix[i]->GetExecNoeud()->AfficherNoeud();
                m_ExecChoix[i]->GetExecNoeud()->show();

                est_ce_que_l_interface_vers_suite_est_affichee = true;
            }
        }
    }

    if ( !est_ce_que_l_interface_vers_suite_est_affichee &&
         (Univers::ME->EstEnModeHistoire()) )
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

void ExecEffet::AfficherNoeud()
{
    // cette fonction peut être appelée pour rafraichir un affichage donc on cache tout avant de le réafficher éventuellement
    ui->titreEffet->hide();
    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details && GetEffet()->m_Nom != "")
    {
        ui->titreEffet->show();
        ui->titreEffet->setFont( *Univers::TITRE_FONT);
        ui->titreEffet->setText(GetEffet()->m_Nom);
    }

    ui->texteEffet->hide();
    if ( GetEffet()->TexteAAfficher() != "")
    {
        ui->texteEffet->show();
        ui->texteEffet->setFont( *Univers::BASE_FONT);
        ui->texteEffet->setText(GetEffet()->TexteAAfficher());
        ui->texteEffet->setWordWrap(true);
    }

    ui->imageLabel->hide();

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
        int largeurImg = this->width() - 25; //25 pixels environ à cause de la barre de défilement à droite...
        if ( (afficheImg &&  m_Img.width() > largeurImg)
           || (afficheFilm &&  m_Film->scaledSize().width() > largeurImg) )
        {
            int w = largeurImg;
            int h = ui->imageLabel->heightForWidth(w);
            QSize AdjustSize = QSize(w, h);
            ui->imageLabel->setMinimumSize(AdjustSize);
            ui->imageLabel->setMaximumSize(AdjustSize);
        }
    }

    // déclenchement du chrono pour cet effet si il en a un :
    if ( GetEffet()->m_MsChrono != -1 )
    {
        QTimer::singleShot(GetEffet()->m_MsChrono, this, SLOT(FinChrono()));
    }

    Univers::ME->GetExecHistoire()->ExecEvtActuel()->RafraichirAffichageLayouts();
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
    SetCarac setCarac(ModifCaracType::SetCarac,
                      GetEffet()->m_Glisseur->m_IdCaracAssociee,
                      QString::number(ui->glisseur->value()));
    Univers::ME->GetExecHistoire()->AppliquerCarac(setCarac);
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
