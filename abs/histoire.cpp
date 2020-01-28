#include "histoire.h"
#include "univers.h"

Hist::Hist(QString titre):m_Titre(titre) {}

std::shared_ptr<DPerso> Hist::GetPersoCourant()
{
    return Univers::ME->GetPersoInterface()->GetPersoCourant();
}
