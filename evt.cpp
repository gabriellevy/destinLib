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
    ui(new Ui::Evt)
{
    ui->setupUi(this);

    m_EffetActuel = nullptr;
}

void Evt::AfficherNoeud()
{
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
    Effet* effet = new Effet();
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetRetireurACarac(QString caracId, QString valeurRetire, QString text, QString id)
{
    Effet* effet = new Effet(id,
                            text,
                             "");
    effet->AjouterRetireurACarac(caracId, valeurRetire);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetModificateurCarac(QString caracId, QString nouvelleValeur, QString text, QString id)
{
    Effet* effet = new Effet(id,
                            text,
                             "");
    effet->AjouterChangeurDeCarac(caracId, nouvelleValeur);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetAjouteurACarac(QString caracId, QString valeurAjoutee, QString id)
{
    Effet* effet = new Effet(id);
    effet->AjouterAjouteurACarac(caracId, valeurAjoutee);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetNarration(QString text, QString cheminImg, QString id)
{
    Effet* effet = new Effet(id,
                            text,
                             cheminImg);
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetGlisseur(QString text, QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id, QString cheminImg, QString id )
{
    Effet* effet = new Effet(id,
                            text,
                             cheminImg);

    effet->AjouterGlisseur(valeur_min, valeur_max, valeur_depart, carac_id);

    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetChangementPerso(QString persoId, QString text, QString cheminImg, QString id)
{
    Effet* effet = new Effet(id,
                            text,
                             cheminImg);
    effet->m_ChangePerso = persoId;
    AjouterEffet(effet);
    return effet;
}

Effet* Evt::AjouterEffetTest(QString caracId, Comparateur comparateur, QString valeur, QString id )
{
    Effet* effet = new Effet(id, "", "");
    effet->m_Conditions.push_back(new Condition(caracId, valeur, comparateur));
    AjouterEffet(effet);
    return effet;
}
