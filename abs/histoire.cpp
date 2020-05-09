#include "histoire.h"
#include "univers.h"

Hist::Hist(QString titre):m_Titre(titre) {}

std::shared_ptr<DPerso> Hist::GetPersoCourant()
{
    return Univers::ME->GetPersoInterface()->GetPersoCourant();
}

void Hist::SetModeDeroulement(ModeDeroulement modeDeroulement, int msDureeDefilement)
{
    if ( msDureeDefilement != -1 )
        m_MsDureeDefilement = msDureeDefilement;
    Q_ASSERT_X(m_MsDureeDefilement > 0 || modeDeroulement != ModeDeroulement::Automatique,
               "Hist::SetModeDeroulement",
               "Impossible d'être en mode de déroulement automatique sans avoir une durée de défilement supérieure à 0");
    m_ModeDeroulement = modeDeroulement;
}
