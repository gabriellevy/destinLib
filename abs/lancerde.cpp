#include "lancerde.h"

LancerDe::LancerDe(std::shared_ptr<Effet> ParentEffet, QString texteBouton, int nbDes,
    std::function<ResExecutionLancerDe*(int resDe/*, QVector<QString> params*/)> callback) :
    NoeudNarratif("", "", texteBouton),
    m_ParentEffet(ParentEffet),
    m_NbDes(nbDes),
    m_Callback(callback)
{
    m_TypeNoeud = TypeNoeud::etn_LancerDe;
}

bool LancerDe::AQuelqueChoseAAfficher()
{
    return true;
}
