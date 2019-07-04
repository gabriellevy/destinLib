#include "histoire.h"
#include "univers.h"

Hist::Hist(QString titre):m_Titre(titre)
{

}

DPerso* Hist::GetPersoCourant()
{
    return Univers::ME->GetPersoInterface()->GetPersoCourant();
}
