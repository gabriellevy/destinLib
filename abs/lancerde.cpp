#include "lancerde.h"

using std::shared_ptr;

LancerDe::LancerDe(QString texteBouton, int nbDes,
    std::function<shared_ptr<ResExecutionLancerDe>(int resDe/*, QVector<QString> params*/)> callback) :
    NoeudNarratif("", "", texteBouton),
    m_NbDes(nbDes),
    m_Callback(callback)
{
    m_TypeNoeud = TypeNoeud::etn_LancerDe;
}

bool LancerDe::AQuelqueChoseAAfficher()
{
    return true;
}
