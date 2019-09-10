#include "lancerde.h"

LancerDe::LancerDe(Effet* ParentEffet, QString intitule, int nbDes) :
    NoeudNarratif(),
    m_ParentEffet(ParentEffet),
    m_NbDes(nbDes),
    m_Intitule(intitule)
{
    m_TypeNoeud = TypeNoeud::etn_LancerDe;
}

bool LancerDe::AQuelqueChoseAAfficher()
{
    return true;
}
