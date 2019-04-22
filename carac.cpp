#include "carac.h"
#include "ui_carac.h"
#include "univers.h"
#include <QDebug>

DCarac::DCarac(QString Id, QString Intitule, QString Valeur, QString Description)
{
    m_Valeur = Valeur;
    m_Id = Id;
    m_Intitule = Intitule;
    m_Description = Description;
}

Carac::Carac(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Carac)
{
    ui->setupUi(this);

    //m_Img = NULL;
    m_ModeAffichage = MODE_AFFICHAGE::Ma_Cache;
}

Carac::Carac(QString Id, QString Intitule, QString Valeur, QString CheminImg, QString Description, MODE_AFFICHAGE ModeAffichage, QWidget *parent) :
    QWidget(parent),
    m_ModeAffichage(ModeAffichage),
    m_DataCarac(Id, Intitule, Valeur, Description),
    ui(new Ui::Carac)
{
    ui->setupUi(this);

    m_Img.load(CheminImg);
}

Jauge::Jauge(QString Id, QString Intitule, double Minimum, double Maximum, double ValeurDepart, QString Img, QString Description, QWidget *parent)
    : Carac(Id, Intitule, "", Img, Description, MODE_AFFICHAGE::ma_Jauge, parent)
{
    m_ValeursJauge.m_Minimum = Minimum;
    m_ValeursJauge.m_Maximum = Maximum;
    m_ValeursJauge.m_ValeurDepart = ValeurDepart;
    m_DataCarac.m_Valeur = QString::number(static_cast<int>(ValeurDepart));
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

void Jauge::SetValeursJauge(double Minimum, double Maximum)
{
    m_ValeursJauge.m_Minimum = Minimum;
    m_ValeursJauge.m_Maximum = Maximum;
    m_ValeursJauge.m_ValeurDepart = m_DataCarac.m_Valeur.toDouble();
    m_ValeursJauge.m_IdCaracAssociee = m_DataCarac.m_Id;
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
                qDebug()<<"La carac de cet id est censée avoir un affichage binaire mais n'a pas d'intitulé : " <<m_DataCarac.m_Id.toStdString().c_str();
            }

        }break;
        case MODE_AFFICHAGE::ma_Img:{
            if ( !AfficherImage() )
            {
                 qDebug()<<"La carac de cet id est censée avoir un affichage image mais n'a pas d'image : " <<m_DataCarac.m_Id.toStdString().c_str();
            }
            AfficherIntitule();

        }break;
        case MODE_AFFICHAGE::ma_Jauge :{
            bool afficheImage = AfficherImage();
            AfficherIntitule();

            ui->jaugeCarac->show();
            ui->jaugeCarac->setRange(
                        static_cast<int>((static_cast<Jauge*>(this))->m_ValeursJauge.m_Minimum),
                        static_cast<int>((static_cast<Jauge*>(this))->m_ValeursJauge.m_Maximum));
            ui->jaugeCarac->setValue(m_DataCarac.m_Valeur.toInt());
            if ( !afficheImage)
            {
                ui->jaugeCarac->update();
            }

        }break;
        case MODE_AFFICHAGE::ma_Nombre:{
        if (AfficherIntitule())
        {
            AfficherValeur();
        } else {
            qDebug()<<"La carac de cet id est censée avoir un affichage nombre mais n'a pas d'intitulé ce qui est nécessaire pour l'identifier : " <<m_DataCarac.m_Id.toStdString().c_str();
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
    if ( m_DataCarac.m_Intitule != "")
    {
        ui->caracBox->show();
        ui->caracBox->setFont( *Univers::BASE_FONT);
        ui->caracBox->setTitle(m_DataCarac.m_Intitule);
        ui->caracBox->setToolTip(m_DataCarac.m_Description);
        return true;
    }
    else
    {
        qDebug()<<"Tentative d'afficher une carac dénuée d'intitulé"<<endl;
        return false;
    }
}


QString Carac::GetCaracValue(QString id)
{
    return Univers::ME->GetHistoire()->GetCaracValue(id);
}

int Carac::GetCaracValueAsInt(QString id)
{
    return Univers::ME->GetHistoire()->GetCaracValue(id).toInt();
}

int Carac::AJouterValeurACaracId(QString idCarac, int valeurAjoutee)
{
    Univers::ME->GetHistoire()->AppliquerCarac(
                SetCarac(ModifCaracType::AddToCarac, idCarac, QString::number(valeurAjoutee)));
    return Carac::GetCaracValueAsInt(idCarac);
}

int Carac::RetirerValeurACaracId(QString idCarac, int valeurRetiree)
{
    Univers::ME->GetHistoire()->AppliquerCarac(
                SetCarac(ModifCaracType::RetireDeCarac, idCarac, QString::number(valeurRetiree)));
    return Carac::GetCaracValueAsInt(idCarac);
}

QString Carac::SetValeurACaracId(QString idCarac, QString valeurSet)
{
    Univers::ME->GetHistoire()->AppliquerCarac(SetCarac(ModifCaracType::SetCarac, idCarac, valeurSet));
    return Carac::GetCaracValue(idCarac);
}

bool Carac::AfficherValeur()
{
    if ( m_DataCarac.m_Valeur != "" )
    {
        ui->labelValeur->show();
        ui->labelValeur->setFont( *Univers::BASE_FONT);
        ui->labelValeur->setText(m_DataCarac.m_Valeur);
        ui->labelValeur->setToolTip(m_DataCarac.m_Description);
        return true;
    }
    else
    {
        qDebug() << "Pas de valeur à afficher pour cette carac d'ic :"<<m_DataCarac.m_Id.toStdString().c_str();
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
        ui->imageCarac->setToolTip(m_DataCarac.m_Description);
        return true;
    }
    else {
        ui->imageCarac->hide();
        QSize AdjustSize = QSize(0, 0);
        ui->imageCarac->setMinimumSize(AdjustSize);
        ui->imageCarac->setMaximumSize(AdjustSize);
        return false;
    }
}

bool Carac::bAffichable()
{
    // en mode "détail" on affiche toutes les caracs de toute façon :
    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details )
        return true;

    // est-ce qu'elle fait partie des caracs affichables par le perso actif ?
    for ( int i=0; i < IPerso::GetPersoInterface()->GetPersoCourant()->m_CaracsAAfficher.size() ; i++)
    {
        if ( IPerso::GetPersoInterface()->GetPersoCourant()->m_CaracsAAfficher[i] == this->m_DataCarac.m_Id)
            return (m_DataCarac.m_Valeur != "" /*&& m_DataCarac.m_Valeur != "0"*/ && m_ModeAffichage != MODE_AFFICHAGE::Ma_Cache);
    }
    return false;
}

Carac::~Carac()
{
    delete ui;
}
