#include "effet.h"
#include "ui_effet.h"
#include "aventure.h"
#include <QDebug>

Effet::Effet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Effet)
{
    ui->setupUi(this);
    ui->boutonContinuer->hide();
    ui->glisseur->hide();
    ui->labelGlisseur->hide();
    m_Glisseur = nullptr;
}

Effet::Effet(QString id,
         QString text,
         QString imgPath,
         QWidget *parent) :
    QWidget(parent),
    Noeud(id, "", text),
    ui(new Ui::Effet)
{
    if ( imgPath != "" )
    {
        // si l'image est un gif je pars du principe que c'est un gif potentiellement animé et ça devient un movie au lieu d'une image
        if ( imgPath.endsWith(".gif"))
        {
            m_Film = new QMovie(imgPath);
        }
        else
        {
            m_Img.load(imgPath);
        }
    }


    ui->setupUi(this);
    ui->boutonContinuer->hide();
    ui->glisseur->hide();
    ui->labelGlisseur->hide();
    m_Glisseur = nullptr;
}

Effet::Effet(QJsonObject effetJson, QWidget *parent) :
    QWidget(parent),
    Noeud(effetJson/*, parent*/),
    ui(new Ui::Effet)
{
    ui->setupUi(this);
    ui->boutonContinuer->hide();
    ui->glisseur->hide();
    ui->labelGlisseur->hide();
    m_Glisseur = nullptr;

    /*if ( effetJson.contains("choix") && effetJson["choix"].isArray())
    {
        QJsonArray jsonArrayChoix = effetJson["choix"].toArray();

        for ( int i = 0; i < jsonArrayChoix.size(); ++i)
        {
            Choix* choix = new Choix(jsonArrayChoix[i].toObject());
            m_Choix.append(choix);
        }
    }*/

    // A FAIRE transférer ça dans afficher noeud ?
    if ( effetJson.contains("glisseur") && effetJson["glisseur"].isObject())
    {
        QJsonObject jsonGlisseur = effetJson["glisseur"].toObject();

        ui->glisseur->show();
        ui->labelGlisseur->show();

        m_Glisseur = new Glisseur(jsonGlisseur);

        ui->glisseur->setMinimum(static_cast<int>(m_Glisseur->m_Minimum));
        ui->glisseur->setMaximum(static_cast<int>(m_Glisseur->m_Maximum));
        ui->glisseur->setSliderPosition(static_cast<int>(m_Glisseur->m_ValeurDepart));
        ui->labelGlisseur->setNum(m_Glisseur->m_ValeurDepart);
        connect(ui->glisseur,SIGNAL(valueChanged(int)),this,SLOT(valeurGlisseurAChange(int)));
    }


    if ( effetJson.contains("else") && effetJson["else"].isObject())
    {
        m_ElseNoeud = new Effet(effetJson["else"].toObject());
        m_ElseNoeud->ui = ui;
    }
}

Effet* Effet::AjouterElse(QString text)
{
    m_ElseNoeud = new Effet();
    m_ElseNoeud->m_Text = text;
    m_ElseNoeud->ui = ui;
    return m_ElseNoeud;
}

Effet* Effet::GetElse()
{
    return m_ElseNoeud;
}

void Effet::valeurGlisseurAChange()
{
    SetCarac setCarac(ModifCaracType::SetCarac, m_Glisseur->m_IdCaracAssociee, QString::number(ui->glisseur->value()));
    Aventure::ME->GetHistoire()->AppliquerCarac(setCarac);
}

void Effet::AfficherNoeud()
{
    // cette fonction peut être appelée pour rafraichir un affichage donc on cache tout avant de le réafficher éventuellement
    ui->titreEffet->hide();
    if ( Aventure::ME->m_ModeAffichage == ModeAffichage::ema_Details && m_Nom != "")
    {
        ui->titreEffet->show();
        ui->titreEffet->setFont( *Aventure::TITRE_FONT);
        ui->titreEffet->setText(m_Nom);
    }

    ui->texteEffet->hide();
    if ( TexteAAfficher() != "")
    {
        ui->texteEffet->show();
        ui->texteEffet->setFont( *Aventure::BASE_FONT);
        ui->texteEffet->setText(TexteAAfficher());
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

}

void Effet::FinExecutionNoeud()
{
    Noeud::FinExecutionNoeud();

    if ( m_ElseNoeud != nullptr)
        m_ElseNoeud->FinExecutionNoeud();
}

bool Effet::GestionTransition()
{
    bool est_ce_que_l_interface_vers_suite_est_affichee = false;

    if ( m_Choix.size() > 0 )
    {
        for ( int i = 0 ; i < m_Choix.size() ; ++i)
        {
            m_Choix[i]->hide();
            if ( m_Choix[i]->TesterConditions())
            {
                if ( m_OrientationAffichageChoix == OrientationAffichageChoix::oac_vertical)
                    ui->layoutBoutons->layout()->addWidget(m_Choix[i]);
                else
                    ui->horizontalLayoutBoutons->layout()->addWidget(m_Choix[i]);
                m_Choix[i]->AfficherNoeud();
                m_Choix[i]->show();

                est_ce_que_l_interface_vers_suite_est_affichee = true;
            }
        }
    }

    if ( !est_ce_que_l_interface_vers_suite_est_affichee &&
         (Aventure::ME->EstEnModeHistoire()) )
    {
        // si l'effet est dans une boucle while il doit s'afficher encore tant que la condition sera remplie
        /*if ( m_RepeatWhileConditions.size() > 0 &&
             Condition::TesterTableauDeConditions(m_RepeatWhileConditions) )
        {
            // A FAIRE MAT : ?? peut-être faudrait-t'il raffraichir l'affichage de l'effet ?
            LancerNoeud();
        }
        else*/
        if ( this->AQuelqueChoseAAfficher() )
        {
            AfficherBoutonSuivant();
            est_ce_que_l_interface_vers_suite_est_affichee = true;
        }
        else
        {
            Aventure::ME->GetHistoire()->DeterminerPuisLancerEffetSuivant(this);
            return false;
        }
    }

    return !est_ce_que_l_interface_vers_suite_est_affichee;
}

/*void Effet::ExecuterNoeudSlot(bool afficherNoeud, bool lancerNoeudSuivantSiRienAAfiicher)
{
    this->ExecuterActionsNoeud(afficherNoeud, lancerNoeudSuivantSiRienAAfiicher);
}*/


Glisseur* Effet::AjouterGlisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id)
{
    // A FAIRE transférer ça dans afficher noeud ?
    ui->glisseur->show();
    ui->labelGlisseur->show();

    m_Glisseur = new Glisseur(valeur_min, valeur_max, valeur_depart, carac_id);

    ui->glisseur->setMinimum(static_cast<int>(m_Glisseur->m_Minimum));
    ui->glisseur->setMaximum(static_cast<int>(m_Glisseur->m_Maximum));
    ui->glisseur->setSliderPosition(static_cast<int>(m_Glisseur->m_ValeurDepart));
    ui->labelGlisseur->setNum(static_cast<int>(m_Glisseur->m_ValeurDepart));
    connect(ui->glisseur,SIGNAL(valueChanged(int)),this,SLOT(valeurGlisseurAChange(int)));

    return m_Glisseur;
}

bool Effet::AQuelqueChoseAAfficher()
{
    bool choixAAfficher = false;
    // vérifier que les choix ne sont pas bloqués par des conditions
    for ( int i = 0 ; i < m_Choix.size() ;++i)
    {
        if ( m_Choix[i]->TesterConditions())
        {
            choixAAfficher = true;
            break;
        }
    }

    return (Noeud::AQuelqueChoseAAfficher() || choixAAfficher);
}

Effet::~Effet()
{
    delete ui;
}

void Effet::AfficherBoutonSuivant()
{
    ui->boutonContinuer->show();
    ui->boutonContinuer->setCursor(Qt::PointingHandCursor);

    QObject::connect(ui->boutonContinuer, SIGNAL(clicked()), Aventure::ME, SLOT(DeclencherEffetSuivant()));
}


Choix* Effet::AjouterChoixChangeurDeCarac(QString text, QString carac, QString valeur)
{
    Choix* choix = new Choix(text);
    choix->AjouterChangeurDeCarac(carac, valeur);
    m_Choix.push_back(choix);
    return choix;
}

Choix* Effet::AjouterChoixGoToEffet(QString text, QString go_to_effet_id, QString cheminImg)
{
    Choix* choix = new Choix(text, cheminImg);
    choix->m_GoToEffetId = go_to_effet_id;
    m_Choix.push_back(choix);
    return choix;
}
