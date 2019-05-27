#include "effet.h"
#include "ui_effet.h"
#include "univers.h"
#include <QTimer>
#include <QDebug>
#include <QSqlQuery>

Effet::Effet(Evt* evt, QWidget *parent) :
    QWidget(parent),
    m_Evt(evt),
    ui(new Ui::Effet)
{
    ui->setupUi(this);
    ui->boutonContinuer->hide();
    ui->glisseur->hide();
    ui->labelGlisseur->hide();
    m_Glisseur = nullptr;
    m_TypeNoeud = TypeNoeud::etn_Effet;
}

Effet::Effet(Evt* evt, QString id,
         QString text,
         QString imgPath,
         QWidget *parent) :
    QWidget(parent),
    Noeud(id, "", text),
    m_Evt(evt),
    ui(new Ui::Effet)
{
    this->ChargerImage(imgPath);

    ui->setupUi(this);
    ui->boutonContinuer->hide();
    ui->glisseur->hide();
    ui->labelGlisseur->hide();
    m_Glisseur = nullptr;
    m_TypeNoeud = TypeNoeud::etn_Effet;
}

void Effet::ChargerImage(QString chemin)
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

/*Effet::Effet(QJsonObject effetJson, QWidget *parent) :
    QWidget(parent),
    Noeud(effetJson),
    ui(new Ui::Effet)
{
    ui->setupUi(this);
    ui->boutonContinuer->hide();
    ui->glisseur->hide();
    ui->labelGlisseur->hide();
    m_Glisseur = nullptr;

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
}*/

Effet* Effet::AjouterElse(QString text)
{
    m_ElseNoeud = new Effet(this->m_Evt);
    m_ElseNoeud->m_Text = text;
    m_ElseNoeud->ui = ui;
    return m_ElseNoeud;
}

void Effet::RafraichirAffichageLayouts(int largeur, int )
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

Effet* Effet::GetElse()
{
    return m_ElseNoeud;
}

int Effet::CalculerIndex()
{
    int index =0;
    for ( Effet* effet: this->m_Evt->m_Effets)
    {
        if ( effet == this)
            return index;
        index++;
    }
    Q_ASSERT_X(true, "Effet introuvable dans son propre evt !", "Effet::CalculerIndex");
    return -1;
}

void Effet::valeurGlisseurAChange()
{
    SetCarac setCarac(ModifCaracType::SetCarac, m_Glisseur->m_IdCaracAssociee, QString::number(ui->glisseur->value()));
    Univers::ME->GetExecHistoire()->AppliquerCarac(setCarac);
}

void Effet::AfficherNoeud()
{
    // cette fonction peut être appelée pour rafraichir un affichage donc on cache tout avant de le réafficher éventuellement
    ui->titreEffet->hide();
    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details && m_Nom != "")
    {
        ui->titreEffet->show();
        ui->titreEffet->setFont( *Univers::TITRE_FONT);
        ui->titreEffet->setText(m_Nom);
    }

    ui->texteEffet->hide();
    if ( TexteAAfficher() != "")
    {
        ui->texteEffet->show();
        ui->texteEffet->setFont( *Univers::BASE_FONT);
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

    // déclenchement du chrono pour cet effet si il en a un :
    if ( m_MsChrono != -1 )
    {
        QTimer::singleShot(m_MsChrono, this, SLOT(FinChrono()));
    }

    Univers::ME->GetExecHistoire()->EvtActuel()->RafraichirAffichageLayouts();
}

void Effet::FinChrono()
{
    Univers::ME->GetExecHistoire()->DeterminerPuisLancerNoeudSuivant(this);
}

void Effet::SupprimerTousLesChoix()
{
    qDeleteAll(this->m_Choix.begin(), this->m_Choix.end());
    this->m_Choix.clear();
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

    // si l'effet contient un choix on l'affiche et on considère que le passage vers l'effet suivant est géré par les choix
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
        if ( this->AQuelqueChoseAAfficher() )
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

/*bool Effet::AQuelqueChoseAAfficher()
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
}*/

Effet::~Effet()
{
    delete ui;
}

void Effet::AfficherBoutonSuivant()
{
    ui->boutonContinuer->show();
    ui->boutonContinuer->setCursor(Qt::PointingHandCursor);

    QObject::connect(ui->boutonContinuer, SIGNAL(clicked()), Univers::ME, SLOT(DeclencherEffetSuivant()));
}

void Effet::ChargerChoixBdd()
{
    QString req = "SELECT * FROM d_Choix WHERE appartient_a_effet_id = " +
            QString::number(this->m_BDD_EffetId) +
            " ORDER BY ordre";
    QSqlQuery query(req);
    while (query.next())
    {
       int bd_id = query.value("id").toInt();

       Choix* choix = this->AjouterChoixVide();
       choix->m_BDD_ChoixId = bd_id;

       // récupération de la partie noeud :
       choix->AppliquerValeurDeNoeudBDD( query.value("est_un_noeud_id").toInt());
    }
}

Choix* Effet::AjouterChoixVide()
{
    Choix* choix = new Choix(this);
    m_Choix.push_back(choix);
    return choix;
}


Choix* Effet::AjouterChoixChangeurDeCarac(QString text, QString carac, QString valeur)
{
    Choix* choix = new Choix(this, text);
    choix->AjouterChangeurDeCarac(carac, valeur);
    m_Choix.push_back(choix);
    return choix;
}

Choix* Effet::AjouterChoixGoToEffet(QString text, QString go_to_effet_id, QString cheminImg)
{
    Choix* choix = new Choix(this, text, cheminImg);
    choix->m_GoToEffetId = go_to_effet_id;
    m_Choix.push_back(choix);
    return choix;
}
