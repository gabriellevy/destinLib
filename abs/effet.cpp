#include "effet.h"
#include "univers.h"
#include <QTimer>
#include <QDebug>
#include <QSqlQuery>
#include "choix.h"

Effet::Effet(std::shared_ptr<Evt> evt) :
    m_Evt(evt)
{
    m_Glisseur = nullptr;
    m_TypeNoeud = TypeNoeud::etn_Effet;
}

Effet::Effet(std::shared_ptr<Evt> evt, QString id,
         QString text,
         QString imgPath) :
    NoeudNarratif(id, "", text),
    m_Evt(evt)
{
    m_Glisseur = nullptr;
    m_ImgPath = imgPath;
    m_TypeNoeud = TypeNoeud::etn_Effet;
}

Glisseur* Effet::AjouterGlisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id)
{
    m_Glisseur = new Glisseur(valeur_min, valeur_max, valeur_depart, carac_id);

    return m_Glisseur;

}

int Effet::CalculerIndex()
{
    int index =0;
    for ( std::shared_ptr<Effet> effet: this->m_Evt->m_Effets)
    {
        if ( effet.get() == this)
            return index;
        index++;
    }
    Q_ASSERT_X(true, "Effet introuvable dans son propre evt !", "Effet::CalculerIndex");
    return -1;
}

void Effet::SupprimerTousLesChoix()
{
    //qDeleteAll(this->m_Choix.begin(), this->m_Choix.end());
    this->m_Choix.clear();
}
