#include "selectionneurdenoeud.h"
#include "noeud.h"
#include "evt.h"
#include <QDebug>

QList<SelectionneurDeNoeud*> SelectionneurDeNoeud::s_TousLesSelectionneurs = {};

SelectionneurDeNoeud::SelectionneurDeNoeud(QString intitule, int bdd_id):m_BddId(bdd_id), m_Intitule(intitule)
{}

Noeud* SelectionneurDeNoeud::DeterminerEvtSuivant()
{
    QList<NoeudProbable*> noeudsPossibles;
    double totalDesProbas = 0;
    for ( int i = 0 ; i < m_NoeudsProbables.size() ; ++i)
    {
        if ( m_NoeudsProbables[i]->m_Noeud->TesterConditions() )
        {
            double proba = m_NoeudsProbables[i]->m_PoidsProba->CalculerProbaFinale();
            /*Q_ASSERT_X(proba >= 0,
                       "Attention un événement inclu dans un sélectionneur d'événement doit avoir une proba pour être sélectionnable !",
                       "SelectionneurDEvenement::DeterminerEvtSuivant");*/
            if ( proba > 0)
            {
                noeudsPossibles.append(m_NoeudsProbables[i]);
                totalDesProbas += proba;
            }
        }/* else {
            qDebug()<<"Cet événement ne peut pas arriver : " << m_Noeuds[i]->m_Id;
        }*/
    }

    QString txt = "Aucun evt trouvé dans le sélectionneur d'événement ";
    Q_ASSERT_X(  noeudsPossibles.size() > 0,
                 "DeterminerEvtAleatoire",
                 txt.toStdString().c_str() );

    // sélectionner un de ces événements (en fonction de leur proba) et l'exécuter :
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);// entre 0 et 1
    double probaIndicator = static_cast<double>(r) * totalDesProbas;
    double totalCourantDesProbas = 0;
    Noeud* noeudChoisi = nullptr;

    for ( int j = 0; j < noeudsPossibles.size() ; ++j )
    {
        totalCourantDesProbas += noeudsPossibles[j]->m_PoidsProba->CalculerProbaFinale();
        if ( probaIndicator < totalCourantDesProbas)
        {
            // cet événement est sélectionné :
            noeudChoisi = noeudsPossibles[j]->m_Noeud;
            break;
        }
    }

    return noeudChoisi;
}
