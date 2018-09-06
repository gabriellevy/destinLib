#include "carac.h"
#include "ui_carac.h"
#include "aventure.h"
#include <QDebug>

Carac::Carac(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Carac)
{
    ui->setupUi(this);

    m_Valeur = "";
    m_Id = "";
    m_Intitule = "";
    //m_Img = NULL;
    m_Description = "";
    m_ModeAffichage = MODE_AFFICHAGE::Ma_Cache;
}

Carac::Carac(QString Id, QString Intitule, QString Valeur, QString CheminImg, QString Description, MODE_AFFICHAGE ModeAffichage, QWidget *parent) :
    QWidget(parent),
    m_Id(Id),
    m_Intitule(Intitule),
    m_Valeur(Valeur),
    m_Description(Description),
    m_ModeAffichage(ModeAffichage),
    ui(new Ui::Carac)
{
    ui->setupUi(this);

    m_Img.load(CheminImg);
}


void Carac::DeterminerModeAffichage(QString modeAffichage)
{
    if( modeAffichage == "")
    {
        // déduction du mode si il n'y a pas d'indication
        if ( modeAffichage == "" )
            m_ModeAffichage = MODE_AFFICHAGE::ma_Binaire;

        /*if ( m_Img != "")
            m_ModeAffichage = MODE_AFFICHAGE::ma_Img;*/
    }
    else if( modeAffichage == "caché") {
        m_ModeAffichage = MODE_AFFICHAGE::Ma_Cache;
    }
    else if( modeAffichage == "binaire") {
        m_ModeAffichage = MODE_AFFICHAGE::ma_Binaire;
    }
    else if( modeAffichage == "img") {
        m_ModeAffichage = MODE_AFFICHAGE::ma_Img;
    }
    else if( modeAffichage == "texte") {
        m_ModeAffichage = MODE_AFFICHAGE::ma_Texte;
    }
    else if( modeAffichage == "nombre") {
        m_ModeAffichage = MODE_AFFICHAGE::ma_Nombre;
    }
    else if( modeAffichage == "jauge") {
        m_ModeAffichage = MODE_AFFICHAGE::ma_Jauge;
    }
}

void Carac::Afficher()
{
    // A FAIRE gérer ici les valeurs qui utilisent la jauge...
    ui->jaugeCarac->hide();

    if ( bAffichable() )
    {
        switch (m_ModeAffichage)
        {
        case MODE_AFFICHAGE::ma_Binaire:{
            if ( !AfficherIntitule())
            {
                qDebug()<<"La carac de cet id est censée avoir un affichage binaire mais n'a pas d'intitulé : " <<m_Id.toStdString().c_str();
            }

        }break;
        case MODE_AFFICHAGE::ma_Img:{
            if ( !AfficherImage() )
            {
                 qDebug()<<"La carac de cet id est censée avoir un affichage image mais n'a pas d'image : " <<m_Id.toStdString().c_str();
            }
            AfficherIntitule();

        }break;
        case MODE_AFFICHAGE::ma_Jauge :{
            AfficherImage();
            AfficherIntitule();
                qDebug()<<"A FAIRE ";
        }break;
        case MODE_AFFICHAGE::ma_Nombre:{
        if (AfficherIntitule())
        {
            AfficherValeur();
        } else {
            qDebug()<<"La carac de cet id est censée avoir un affichage nombre mais n'a pas d'intitulé ce qui est nécessaire pour l'identifier : " <<m_Id.toStdString().c_str();
        }
        AfficherImage();
        }break;

        case MODE_AFFICHAGE::ma_Texte:{
        AfficherIntitule();
        AfficherValeur();
        AfficherImage();
        }break;

        case MODE_AFFICHAGE::Ma_Cache:
        {
            ui->caracBox->hide();
        }break;
        }

    }
    else
    {
        ui->caracBox->hide();
    }
}

bool Carac::AfficherIntitule()
{
    if ( m_Intitule != "")
    {
        ui->caracBox->show();
        ui->caracBox->setFont( *Aventure::BASE_FONT);
        ui->caracBox->setTitle(m_Intitule);
        ui->caracBox->setToolTip(m_Description);
        return true;
    }
    else
        return false;
}

bool Carac::AfficherValeur()
{
    if ( m_Valeur != "" )
    {
        ui->labelValeur->show();
        ui->labelValeur->setFont( *Aventure::BASE_FONT);
        ui->labelValeur->setText(m_Valeur);
        ui->labelValeur->setToolTip(m_Description);
        return true;
    }
    else
    {
        qDebug() << "Pas de valeur à afficher pour cette carac d'ic :"<<m_Id.toStdString().c_str();
        return false;
    }
}

bool Carac::AfficherImage()
{
    if ( !m_Img.isNull() )
    {
        ui->imageCarac->show();

        ui->imageCarac->setPixmap(m_Img);

        int hauteurImg = this->height() - 5;
        if ( m_Img.height() > hauteurImg )
        {
            int h = hauteurImg;
            int w = m_Img.width() * h / m_Img.height();
            QSize AdjustSize = QSize(w, h);
            ui->imageCarac->setMinimumSize(AdjustSize);
            ui->imageCarac->setMaximumSize(AdjustSize);
        }
        ui->imageCarac->setToolTip(m_Description);
        return true;
    }
    else {
        return false;
    }
}

bool Carac::bAffichable()
{
    // est-ce qu'elle fait partie des caracs affichables par le perso actif ?
    for ( int i=0; i < IPerso::GetPersoInterface()->GetPersoCourant().m_CaracsAAfficher.size() ; i++)
    {
        if ( IPerso::GetPersoInterface()->GetPersoCourant().m_CaracsAAfficher[i] == this->m_Id)
            return (m_Valeur != "" && m_Valeur != "0" && m_ModeAffichage != MODE_AFFICHAGE::Ma_Cache);
    }
    return false;
}

Carac::~Carac()
{
    delete ui;
}
