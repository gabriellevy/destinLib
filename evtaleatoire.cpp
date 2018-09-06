#include "evtaleatoire.h"


EvtAleatoire::EvtAleatoire(QString id,
         QString nom,
         QWidget *parent) :
    Evt(id,
        nom,
        parent)
{
    m_TypeEvenement = TE_Aleatoire;
}


Effet* EvtAleatoire::DeterminerEffetAleatoire()
{
    // select a random event using them
    QList<Effet*> effetsPossibles;
    float totalDesProbas = 0;
    for ( int i = 0 ; i < m_Effets.size() ; ++i)
    {
        float proba = m_Effets[i]->GetProba();
        if ( proba != 0)
        {
            effetsPossibles.append(m_Effets[i]);
            totalDesProbas += proba;
        }
    }

    QString txt = "Aucun effet trouvé dans l'événement aléatoire " + this->m_Id;
    Q_ASSERT_X(  m_Effets.size() > 0,
                 "DeterminerEvtAleatoire",
                 txt.toStdString().c_str() );

    // sélectionner un de ces événements (en fonction de leur proba) et l'exécuter :
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);// entre 0 et 1
    float probaIndicator = r * totalDesProbas;
    float totalCourantDesProbas = 0;
    Effet* effetchoisi = nullptr;

    for ( int j = 0; j < effetsPossibles.size() ; ++j )
    {
        totalCourantDesProbas += effetsPossibles[j]->GetProba();
        if ( probaIndicator < totalCourantDesProbas)
        {
            // cet événement est sélectionné :
            effetchoisi = effetsPossibles[j];
            break;
        }
    }

    return effetchoisi;
}
