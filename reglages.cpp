#include "reglages.h"
#include "univers.h"
#include "execeffet.h"
#include "execevt.h"

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
        if ( Univers::ME->GetExecHistoire()->ExecEffetActuel() )
            Univers::ME->GetExecHistoire()->ExecEffetActuel()->RafraichirAffichageLayouts(m_Largeur,m_Hauteur);

        if ( Univers::ME->GetExecHistoire()->ExecEvtActuel() )
            Univers::ME->GetExecHistoire()->ExecEvtActuel()->RafraichirAffichageLayouts(m_Largeur,m_Hauteur);

        Univers::ME->GetExecHistoire()->RafraichirAffichageLayouts(m_Largeur,m_Hauteur);
    }

    Univers::ME->RafraichirAffichageLayouts(m_Largeur,m_Hauteur);
}
