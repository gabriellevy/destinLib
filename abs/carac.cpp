#include "carac.h"
#include "ui_carac.h"
#include "univers.h"
#include <QDebug>
#include "../gestcarac.h"

bool Carac::CHANGT_CARAC_CE_TOUR = true;

DCarac::DCarac(QString Id, QString Intitule, QString Valeur, QString Description, QString valeurMin, QString valeurMax) :
    m_Id(Id), m_Intitule(Intitule), m_Description(Description), m_ValeurMin(valeurMin), m_ValeurMax(valeurMax),
    m_Valeur(Valeur)
{}

double DCarac::SetValeur(double nouvValeur)
{
    SetValeur(QString::number(nouvValeur));

    return GetValeurDouble();
}

QString DCarac::SetValeur(QString nouvValeur)
{
    m_Valeur = nouvValeur;
    Carac::CHANGT_CARAC_CE_TOUR = true;

    // valeurs affectables limitées par valeur min et valeur max
    if ( m_ValeurMax != "" ) {
        if ( m_Valeur.toDouble() > m_ValeurMax.toDouble())
            m_Valeur = m_ValeurMax;
    }
    if ( m_ValeurMin != "" ) {
        if ( m_Valeur.toDouble() < m_ValeurMin.toDouble())
            m_Valeur = m_ValeurMin;
    }

    return m_Valeur;
}

Carac::Carac(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Carac)
{
    ui->setupUi(this);

    //m_Img = NULL;
    m_ModeAffichage = MODE_AFFICHAGE::Ma_Cache;
}

Carac::Carac(QString Id, QString Intitule, QString Valeur, QString CheminImg, QString Description, MODE_AFFICHAGE ModeAffichage,
             QWidget *parent, QString valeurMin, QString valeurMax) :
    QWidget(parent),
    m_ModeAffichage(ModeAffichage),
    m_DataCarac(Id, Intitule, Valeur, Description, valeurMin, valeurMax),
    ui(new Ui::Carac)
{
    ui->setupUi(this);

    if ( CheminImg != "")
     m_Img.load(CheminImg);
}

bool Carac::SetImg(QString CheminImg)
{
    if ( CheminImg != "")
    {
        m_Img.load(CheminImg);
        return true;
    }
    return false;
}

Jauge::Jauge(QString Id, QString Intitule, double Minimum, double Maximum, double ValeurDepart, QString Img, QString Description, QWidget *parent)
    : Carac(Id, Intitule, "", Img, Description, MODE_AFFICHAGE::ma_Jauge, parent, QString::number(Minimum), QString::number(Maximum))
{
    m_ValeursJauge.m_Minimum = Minimum;
    m_ValeursJauge.m_Maximum = Maximum;
    m_ValeursJauge.m_ValeurDepart = ValeurDepart;
    m_DataCarac.SetValeur(QString::number(static_cast<int>(ValeurDepart)));
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
    else if( modeAffichage == "texte avec intitulé") {
        m_ModeAffichage = MODE_AFFICHAGE::ma_Texte_intitule;
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
    m_ValeursJauge.m_ValeurDepart = m_DataCarac.GetValeurDouble();
    m_ValeursJauge.m_IdCaracAssociee = m_DataCarac.m_Id;
}

void Carac::Afficher()
{
    this->setStyleSheet("QWidget#Fond { background-color: rgba(" +
                             QString::number(Univers::COULEUR_FOND.red()) +
                             "," + QString::number(Univers::COULEUR_FOND.green()) +
                             "," + QString::number(Univers::COULEUR_FOND.blue()) +
                             "," + QString::number(Univers::COULEUR_FOND.alpha()) +
                             ") }");

    // A FAIRE gérer ici les valeurs qui utilisent la jauge...
    ui->jaugeCarac->hide();
    ui->labelValeur->hide();
    ui->imageCarac->hide();
    ui->imageDansBox->hide();

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
        case MODE_AFFICHAGE::ma_Img:
            ui->caracBox->hide();
        case MODE_AFFICHAGE::ma_ImgValeur:
        {
            if ( !AfficherImage() /*&& !SetImg(m_DataCarac.GetValeur())*/)
            {
                 qDebug()<<"La carac de cet id est censée avoir un affichage image mais n'a pas d'image. Id : " <<m_DataCarac.m_Id.toStdString().c_str()
                        << " - Valeur : " << m_DataCarac.GetValeur().toStdString().c_str();
            }
            //AfficherIntitule();

        }break;
        case MODE_AFFICHAGE::ma_Jauge :{
            bool afficheImage = AfficherImage();
            AfficherIntitule();

            ui->jaugeCarac->show();
            ui->jaugeCarac->setRange(
                        static_cast<int>((static_cast<Jauge*>(this))->m_ValeursJauge.m_Minimum),
                        static_cast<int>((static_cast<Jauge*>(this))->m_ValeursJauge.m_Maximum));
            ui->jaugeCarac->setValue(m_DataCarac.GetValeur().toInt());
            if ( !afficheImage)
            {
                ui->jaugeCarac->update();
            }

        }break;
        case MODE_AFFICHAGE::ma_NombreSupZero:
        case MODE_AFFICHAGE::ma_Nombre:{
        if ( m_ModeAffichage == ma_NombreSupZero && m_DataCarac.GetValeurInt() <= 0) {
            ui->caracBox->hide();
            return ;
        }

        if (AfficherIntitule())
        {
            AfficherValeur();
        } else {
            qDebug()<<"La carac de cet id est censée avoir un affichage nombre mais n'a pas d'intitulé ce qui est nécessaire pour l'identifier : " <<m_DataCarac.m_Id.toStdString().c_str();
        }
        AfficherImage();
        }break;

        case MODE_AFFICHAGE::ma_Texte_intitule:{
        ui->labelValeur->hide();
        AfficherIntitule();
        AfficherValeur();
        AfficherImage();
        }break;

        case MODE_AFFICHAGE::ma_Texte:{
        ui->caracBox->hide();
        AfficherValeur();
        AfficherImage(); // ??
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

QString Carac::GetCaracDescription()
{
    return m_DataCarac.GetDescription();
}

bool Carac::AfficherIntitule()
{
    if ( m_DataCarac.m_Intitule != "")
    {
        if ( m_ModeAffichage == MODE_AFFICHAGE::ma_Binaire ){
            ui->caracBox->hide();
            ui->labelValeur->show();
            ui->labelValeur->setFont( *Univers::BASE_FONT);
            ui->labelValeur->setText(m_DataCarac.m_Intitule);
            ui->labelValeur->setToolTip(this->GetCaracDescription());
        } else {
            ui->caracBox->show();
            ui->caracBox->setFont( *Univers::BASE_FONT);
            ui->caracBox->setTitle(m_DataCarac.m_Intitule);
            ui->caracBox->setToolTip(this->GetCaracDescription());
        }
        return true;
    }
    else
    {
        qDebug()<<"Tentative d'afficher une carac dénuée d'intitulé"<<endl;
        return false;
    }
}

bool Carac::AfficherValeur()
{
    if ( m_DataCarac.AUneValeur() )
    {
        if ( m_ModeAffichage == MODE_AFFICHAGE::ma_Texte_intitule ||
             m_ModeAffichage == MODE_AFFICHAGE::ma_Nombre ||
             m_ModeAffichage == MODE_AFFICHAGE::ma_NombreSupZero) {
            ui->labelValeurDansBox->show();
            ui->labelValeurDansBox->setFont( *Univers::BASE_FONT);
            ui->labelValeurDansBox->setText(m_DataCarac.GetValeur());
            ui->labelValeurDansBox->setToolTip(this->GetCaracDescription());
        } else {
            ui->labelValeur->show();
            ui->labelValeur->setFont( *Univers::BASE_FONT);
            ui->labelValeur->setText(m_DataCarac.GetValeur());
            ui->labelValeur->setToolTip(this->GetCaracDescription());
        }
        return true;
    }
    else
    {
        qDebug() << "Pas de valeur à afficher pour cette carac d'ic :"<<m_DataCarac.m_Id.toStdString().c_str();
        return false;
    }
}

bool Carac::AfficherImage(bool dansBox)
{
    if ( !m_Img.isNull() )
    {
        int hauteurImg = 0;
        if ( dansBox) {
            ui->imageCarac->hide();
            ui->imageDansBox->show();
            ui->imageDansBox->setPixmap(m_Img);
            hauteurImg = 80;
        }
        else {
            ui->imageDansBox->hide();
            ui->imageCarac->show();
            ui->imageCarac->setPixmap(m_Img);
            hauteurImg = this->height()/* - 5*/;
        }

        if ( m_Img.height() > hauteurImg )
        {
            int h = hauteurImg;
            int w = m_Img.width() * h / m_Img.height();
            QSize AdjustSize = QSize(w, h);

            if ( dansBox) {
                ui->imageDansBox->setMinimumSize(AdjustSize);
                ui->imageDansBox->setMaximumSize(AdjustSize);
            }
            else {
                ui->imageCarac->setMinimumSize(AdjustSize);
                ui->imageCarac->setMaximumSize(AdjustSize);
            }
        }
        if ( dansBox) {
            ui->imageDansBox->setToolTip(this->GetCaracDescription());
        }
        else {
            ui->imageCarac->setToolTip(this->GetCaracDescription());
        }
        return true;
    }
    else {
        ui->imageCarac->hide();
        ui->imageDansBox->hide();
        return false;
    }
}

bool Carac::bAffichable()
{
    // en mode "détail" on affiche toutes les caracs de toute façon :
    if ( Univers::ME->m_ModeAffichage == ModeAffichage::ema_Details )
        return true;

    return (m_DataCarac.AUneValeur() /*&& m_DataCarac.m_Valeur != "0"*/ && m_ModeAffichage != MODE_AFFICHAGE::Ma_Cache);
}

Carac::~Carac()
{
    delete ui;
}
