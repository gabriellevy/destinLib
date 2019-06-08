#include "histoire.h"
#include "univers.h"

Hist::Hist(QString titre):m_Titre(titre)
{

}

void Hist::AppliquerTheme(Theme* theme)
{
    this->m_Themes.push_back(theme);

    for(Carac* carac: theme->m_CaracsDeBase)
    {
        this->m_Caracs.push_back(carac);
    }
}


DPerso* Hist::GetPersoCourant()
{
    return Univers::ME->GetPersoInterface()->GetPersoCourant();
}
