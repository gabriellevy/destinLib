#include "selectionneurdenoeud.h"
#include "noeud.h"
#include "evt.h"
#include <QDebug>
#include <chrono>
#include <random>
#include "univers.h"

QList<std::shared_ptr<SelectionneurDeNoeud>> SelectionneurDeNoeud::s_TousLesSelectionneurs = {};

SelectionneurDeNoeud::SelectionneurDeNoeud(QString intitule, int bdd_id):m_BddId(bdd_id), m_Intitule(intitule)
{}

std::shared_ptr<Noeud> SelectionneurDeNoeud::DeterminerNoeudSuivant()// pourquoi pas DeterminerNoeudSuivant en fait ?
{
    // mise en place système de nombre aléatoire
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, RAND_MAX);

    // 1. sélection d'un éventuel noeud qui aurait une proba pure (elle ont priorité et ne sont pas relatives aux autres,
    // leur valeur est vraiment une proba entre 0 et 1
    double proba;// entre 0 et 1
    double totalprobaParcouru = 0;
    for ( int i = 0 ; i < m_NoeudsProbables.size() ; ++i)
    {
        std::shared_ptr<Noeud> noeud = m_NoeudsProbables[i]->m_Noeud.lock();
        if ( m_NoeudsProbables[i]->m_PoidsProba->IsProbaPure() )
        {
            totalprobaParcouru += m_NoeudsProbables[i]->m_PoidsProba->CalculerProbaFinale();
            if ( totalprobaParcouru > 1 )
            {
                // interdit d'avoir une proba pure > 1 dans un mêm sélectionneur !
                Q_ASSERT_X(false, "interdit d'avoir une proba pure > 1 dans un mêm sélectionneur !", "SelectionneurDeNoeud::DeterminerNoeudSuivant");
            }
            proba = static_cast <double> (distribution(generator)) / static_cast <double> (RAND_MAX);
            if ( proba <= m_NoeudsProbables[i]->m_PoidsProba->CalculerProbaFinale())
            {
                if ( m_NoeudsProbables[i]->m_Noeud.lock()->TesterConditions() )
                {
                    return noeud;
                }
            }
        }

        if(Univers::LOG && m_NoeudsProbables[i]->m_Noeud.lock()->TesterConditions() &&
                m_NoeudsProbables[i]->m_PoidsProba->CalculerProbaFinale() > 0 &&
                Univers::FILE.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
          QTextStream stream(&Univers::FILE);
          stream << m_NoeudsProbables[i]->m_Noeud.lock()->m_Id
                 << " (" <<m_NoeudsProbables[i]->m_PoidsProba->CalculerProbaFinale()
                 << ((m_NoeudsProbables[i]->m_PoidsProba->IsProbaPure())?(" pure"):("")) << ")\n";
          Univers::FILE.close();
        }
    }


    //2. si aucun proba pure n'est trouvée, on cherche parmi les probas relatives
    // (et tant qu'il y en a au moins une il y aura bien un noeud sélectionné)
    QList<std::shared_ptr<NoeudProbable>> noeudsPossibles;
    double totalDesProbas = 0;
    for ( int i = 0 ; i < m_NoeudsProbables.size() ; ++i)
    {
        if ( m_NoeudsProbables[i]->m_Noeud.lock()->TesterConditions() )
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
        }
    }

    QString txt = "Aucun evt trouvé dans le sélectionneur d'événement : " + this->m_Intitule;
    Q_ASSERT_X(  noeudsPossibles.size() > 0,
                 "DeterminerEvtAleatoire",
                 txt.toStdString().c_str() );

    // sélectionner un de ces événements (en fonction de leur proba) et l'exécuter :
    float r = static_cast <float> (distribution(generator)) / static_cast <float> (RAND_MAX);// entre 0 et 1
    double probaIndicator = static_cast<double>(r) * totalDesProbas;
    double totalCourantDesProbas = 0;
    std::shared_ptr<Noeud> noeudChoisi = nullptr;

    for ( int j = 0; j < noeudsPossibles.size() ; ++j )
    {
        totalCourantDesProbas += noeudsPossibles[j]->m_PoidsProba->CalculerProbaFinale();
        if ( probaIndicator < totalCourantDesProbas)
        {
            // cet événement est sélectionné :
            noeudChoisi = noeudsPossibles[j]->m_Noeud.lock();
            break;
        }
    }

    return noeudChoisi;
}
