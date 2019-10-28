#include "effet.h"
#include "univers.h"
#include <QTimer>
#include <QDebug>
#include <QSqlQuery>
#include "../destinLib/choix.h"

Effet::Effet(Evt* evt) :
    m_Evt(evt)
{
    m_Glisseur = nullptr;
    m_TypeNoeud = TypeNoeud::etn_Effet;
}

Effet::Effet(Evt* evt, QString id,
         QString text,
         QString imgPath) :
    //QWidget(parent),
    NoeudNarratif(id, "", text),
    m_Evt(evt)
    //ui(new Ui::Effet)
{

    /*ui->setupUi(this);
    ui->boutonContinuer->hide();
    ui->glisseur->hide();
    ui->labelGlisseur->hide();*/
    m_Glisseur = nullptr;
    m_ImgPath = imgPath;
    m_TypeNoeud = TypeNoeud::etn_Effet;
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

/*Effet* Effet::AjouterElse(QString text)
{
    m_ElseNoeud = new Effet(this->m_Evt);
    m_ElseNoeud->m_Text = text;
    //m_ElseNoeud->ui = ui;
    return m_ElseNoeud;
}*/

Glisseur* Effet::AjouterGlisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id)
{
    m_Glisseur = new Glisseur(valeur_min, valeur_max, valeur_depart, carac_id);

    return m_Glisseur;

}

/*Effet* Effet::GetElse()
{
    return m_ElseNoeud;
}*/

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

void Effet::SupprimerTousLesChoix()
{
    qDeleteAll(this->m_Choix.begin(), this->m_Choix.end());
    this->m_Choix.clear();
}
