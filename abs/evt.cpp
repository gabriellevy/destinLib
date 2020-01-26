#include "evt.h"
#include "univers.h"
#include <QDebug>

Evt::Evt(QString id,
         QString nom) :
    NoeudNarratif(id,
          nom,
          ""),
    m_CheminImgFond("")
{

    m_TypeEvenement = TE_Base;
    m_TypeNoeud = TypeNoeud::etn_Evt;
}


std::shared_ptr<Effet> Evt::TrouverEffet(QString id)
{
    foreach(std::shared_ptr<Effet> effet, m_Effets)
    {
        if ( effet->m_Id == id)
            return effet;
    }
    return nullptr;
}
