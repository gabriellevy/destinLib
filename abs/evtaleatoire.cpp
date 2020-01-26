#include "evtaleatoire.h"
#include <chrono>
#include <random>


EvtAleatoire::EvtAleatoire(QString id,
         QString nom) :
    Evt(id,
        nom)
{
    m_TypeEvenement = TE_Aleatoire;
}


std::shared_ptr<Effet> EvtAleatoire::DeterminerEffetAleatoire()
{
    QList<std::shared_ptr<Effet>> effetsPossibles;
    double totalDesProbas = 0;
    for ( int i = 0 ; i < m_Effets.size() ; ++i)
    {
        double proba = m_Effets[i]->GetProba();
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

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, RAND_MAX);

    // sélectionner un de ces événements (en fonction de leur proba) et l'exécuter :
    float r = static_cast <float> (distribution(generator)) / static_cast <float> (RAND_MAX);// entre 0 et 1
    double probaIndicator = static_cast<double>(r) * totalDesProbas;
    double totalCourantDesProbas = 0;
    std::shared_ptr<Effet> effetchoisi = nullptr;

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
