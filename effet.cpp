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
    Noeud(),
    ui(new Ui::Effet)
{
    m_Id = id;
    m_Text = text;
    m_Img.load(imgPath);

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

    if ( effetJson.contains("choix") && effetJson["choix"].isArray())
    {
        QJsonArray jsonArrayChoix = effetJson["choix"].toArray();

        for ( int i = 0; i < jsonArrayChoix.size(); ++i)
        {
            Choix* choix = new Choix(jsonArrayChoix[i].toObject());
            m_Choix.append(choix);
        }
    }

    // A FAIRE transférer ça dans afficher noeud ?
    if ( effetJson.contains("glisseur") && effetJson["glisseur"].isObject())
    {
        QJsonObject jsonGlisseur = effetJson["glisseur"].toObject();

        ui->glisseur->show();
        ui->labelGlisseur->show();

        m_Glisseur = new Glisseur(jsonGlisseur);

        ui->glisseur->setMinimum(m_Glisseur->m_Minimum);
        ui->glisseur->setMaximum(m_Glisseur->m_Maximum);
        ui->glisseur->setSliderPosition(m_Glisseur->m_ValeurDepart);
        ui->labelGlisseur->setNum(m_Glisseur->m_ValeurDepart);
        connect(ui->glisseur,SIGNAL(valueChanged(int)),this,SLOT(valeurGlisseurAChange(int)));
    }
}

void Effet::valeurGlisseurAChange()
{
    SetCarac setCarac(ModifCaracType::SetCarac, m_Glisseur->m_IdCaracAssociee, QString::number(ui->glisseur->value()));
    IPerso::GetPersoInterface()->AppliquerCarac(setCarac);
}

void Effet::AfficherNoeud()
{
    // cette fonction peut être appelée pour rafraichir un affichage donc on cache tout avant de le réafficher éventuellement
    ui->titreEffet->hide();
    if ( m_Nom != "")
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
    if ( !m_Img.isNull() )
    {
        ui->imageLabel->show();
         ui->imageLabel->setPixmap(m_Img);

        // réduction de l'image à la taille de la fenêtre si elle est plus large :
        int largeurImg = this->width() - 25; //25 pixels environ à cause de la barre de défilement à droite...
        if ( m_Img.width() > largeurImg)
        {
            int w = largeurImg;
            int h = ui->imageLabel->heightForWidth(w);
            QSize AdjustSize = QSize(w, h);
            ui->imageLabel->setMinimumSize(AdjustSize);
            ui->imageLabel->setMaximumSize(AdjustSize);
        }
    }

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
                ui->layoutBoutons->layout()->addWidget(m_Choix[i]);
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

    ui->glisseur->setMinimum(m_Glisseur->m_Minimum);
    ui->glisseur->setMaximum(m_Glisseur->m_Maximum);
    ui->glisseur->setSliderPosition(m_Glisseur->m_ValeurDepart);
    ui->labelGlisseur->setNum(m_Glisseur->m_ValeurDepart);
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

Choix* Effet::AjouterChoixGoToEffet(QString text, QString go_to_effet_id)
{
    Choix* choix = new Choix(text);
    choix->m_GoToEffetId = go_to_effet_id;
    m_Choix.push_back(choix);
    return choix;
}
