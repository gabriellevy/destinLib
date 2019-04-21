#include "evt.h"
#include "ui_evt.h"
#include "univers.h"
#include <QDebug>

/*Evt::Evt(QJsonObject evtJson, QWidget *parent) :
    QWidget(parent),
    Noeud(evtJson),
    ui(new Ui::Evt)
{
    ui->setupUi(this);

    m_TypeEvenement = TE_Base;

    // logique gameplay :
    if ( !evtJson.contains("id") || !evtJson["id"].isString())
        QMessageBox::warning(Univers::ME, "erreur", "Un événement sans id !");

    if ( evtJson.contains("effets") && evtJson["effets"].isArray())
    {
        QJsonArray jsonArrayEffets = evtJson["effets"].toArray();

        for ( int i = 0; i < jsonArrayEffets.size(); ++i)
        {
            Effet* effet = new Effet(jsonArrayEffets[i].toObject());
            //this->layout()->addWidget(effet);
            m_Effets.append(effet);
        }
    }
    else if ( m_Id != "abandon" )
        QMessageBox::warning(Univers::ME, "erreur", "Un événement d'id " + m_Id + " sans effets !");

    // interface
    m_EffetActuel = nullptr;
}*/

Evt::Evt(QString id,
         QString nom,
         QWidget *parent) :
    QWidget(parent),
    Noeud(id,
          nom,
          ""),
    m_CheminImgFond(""),
    ui(new Ui::Evt)
{
    ui->setupUi(this);

    m_EffetActuel = nullptr;

    m_TypeEvenement = TE_Base;

    //ui->groupBox->setStyleSheet("background-color: rgba(0,0,0,0)");
    //ui->effetsWidget->setStyleSheet("background-color: rgba(0,0,0,0)");

    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details )
        ui->effetsWidget->setStyleSheet("background-color : rgb(0,0,255)");

    //ui->effetsWidget->layout()->setAlignment(Qt::AlignBottom);
    //((QVBoxLayout*)(ui->effetsWidget->layout()))->addStretch();
    m_TypeNoeud = TypeNoeud::etn_Evt;
}

void Evt::ChargerEffetsBdd()
{
    QSqlQuery query("SELECT * FROM d_Effet WHERE appartient_a_evt_id = " +
                    QString::number(this->m_BDD_EvtId) +
                    " ORDER BY ordre");
    while (query.next())
    {
       int bd_id = query.value("id").toInt();

       Effet* effet = this->AjouterEffetVide();
       effet->m_BDD_EffetId = bd_id;
       effet->ChargerImage(query.value("m_CheminImg").toString());

       // récupération de la partie noeud :
       effet->AppliquerValeurDeNoeudBDD( query.value("est_un_noeud_id").toInt());

       effet->ChargerChoixBdd();
    }
}

void Evt::AjouterImgFond(QString fond)
{
    if ( fond != "")
        m_CheminImgFond = fond;
}

void Evt::AfficherNoeud()
{
    if ( m_CheminImgFond != "")
        Univers::ME->AppliquerFond(m_CheminImgFond);
    ui->titreEvt->setText(m_Nom);
    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details && m_Nom != "")
    {
        ui->titreEvt->show();
        ui->titreEvt->setFont( *Univers::TITRE_FONT);
    }
    else
        ui->titreEvt->hide();

    ui->textEvt->setText( TexteAAfficher() );
    if ( TexteAAfficher() != "")
    {
        ui->textEvt->show();
        ui->textEvt->setFont( *Univers::BASE_FONT);
        ui->textEvt->setWordWrap(true);
    }
    else
        ui->textEvt->hide();

    RafraichirAffichageLayouts();
}

void Evt::RafraichirAffichageLayouts(int largeur, int )
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

void Evt::Clean()
{
    // nécessaire ?
    /*QLayoutItem* item;
    while ( ( item = this->layout()->takeAt(0) ) != 0 )
    {
        delete item;
    }*/
}

bool Evt::GestionTransition()
{
    Q_ASSERT_X(false, "Noeud::GestionTransition", "Je ne crois pas que la gestion de transition devrait se faire dans Evt si ?");
    return true;
}

void Evt::LancerNoeud()
{
    this->AfficherNoeud();

    this->ExecuterActionsNoeud();

    // un evt ne suffit pas à un affichage et une pause : il faut lancer l'effet suivant automatiquement
    Univers::ME->GetHistoire()->DeterminerPuisLancerNoeudSuivant(this);
}

void Evt::RafraichirAffichageEffet(Effet* effet)
{
    if ( m_EffetActuel != nullptr)
    {
        //ui->effetsWidget->layout()->removeWidget(m_EffetActuel);
        m_EffetActuel->hide();
    }
    m_EffetActuel = effet;
    //m_EffetActuel->LancerNoeud();

    ui->effetsWidget->layout()->addWidget(m_EffetActuel);

    m_EffetActuel->show();
    this->update();
    m_EffetActuel->update();
}

Evt::~Evt()
{
    delete ui;
}


Effet* Evt::TrouverEffet(QString id)
{
    foreach(Effet* effet, m_Effets)
    {
        if ( effet->m_Id == id)
            return effet;
    }
    return nullptr;
}

Effet* Evt::AjouterEffet(Effet* effet)
{
    // si l'événement a un chrono il le transmet à tous ses effets (pour annuler celà le chrono sur les effets doit être changé individuellement vers -1)
    if ( this->m_MsChrono != -1 )
        effet->ChangerChrono(this->m_MsChrono);
    m_Effets.push_back(effet);

    return effet;
}

Effet* Evt::AjouterEffetVide()
{
    Effet* effet = new Effet(this);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetRetireurACarac(QString caracId, QString valeurRetire, QString text, QString id)
{
    Effet* effet = new Effet(this,
                             id,
                            text,
                             "");
    effet->AjouterRetireurACarac(caracId, valeurRetire);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetModificateurCarac(QString caracId, QString nouvelleValeur, QString text, QString id)
{
    Effet* effet = new Effet(this,
                             id,
                            text,
                             "");
    effet->AjouterChangeurDeCarac(caracId, nouvelleValeur);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetAjouteurACarac(QString caracId, QString valeurAjoutee, QString id)
{
    Effet* effet = new Effet(this, id);
    effet->AjouterAjouteurACarac(caracId, valeurAjoutee);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetNarration(QString text, QString cheminImg, QString id)
{
    Effet* effet = new Effet(this,
                             id,
                            text,
                             cheminImg);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetGlisseur(QString text, QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id, QString cheminImg, QString id )
{
    Effet* effet = new Effet(this,
                             id,
                            text,
                             cheminImg);

    effet->AjouterGlisseur(valeur_min, valeur_max, valeur_depart, carac_id);

    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetChangementPerso(QString persoId, QString text, QString cheminImg, QString id)
{
    Effet* effet = new Effet(this,
                             id,
                            text,
                             cheminImg);
    effet->m_ChangePerso = persoId;
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetTest(QString caracId, Comparateur comparateur, QString valeur, QString id )
{
    Effet* effet = new Effet(this, id, "", "");
    effet->m_Conditions.push_back(new Condition(caracId, valeur, comparateur));
    AjouterEffet(effet);
    return effet;
}
