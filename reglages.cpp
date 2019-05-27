#include "reglages.h"
#include "univers.h"

Reglages::Reglages()
{
    m_SonOn = true;
    //m_Hauteur = Univers::ME->height();
    //m_Largeur = Univers::ME->width();
    m_LargeurColonneGauche = 250;
}

void Reglages::ChangeTaille(int largeur, int hauteur)
{
    m_Hauteur = hauteur;
    m_Largeur = largeur;

    if ( !Univers::ME) return;

    if ( Univers::ME->GetHistoire() )
    {
        if ( Univers::ME->GetExecHistoire()->EffetActuel() )
            Univers::ME->GetExecHistoire()->EffetActuel()->RafraichirAffichageLayouts(m_Largeur,m_Hauteur);

        if ( Univers::ME->GetExecHistoire()->EvtActuel() )
            Univers::ME->GetExecHistoire()->EvtActuel()->RafraichirAffichageLayouts(m_Largeur,m_Hauteur);

        Univers::ME->GetExecHistoire()->RafraichirAffichageLayouts(m_Largeur,m_Hauteur);
    }

    Univers::ME->RafraichirAffichageLayouts(m_Largeur,m_Hauteur);
}
