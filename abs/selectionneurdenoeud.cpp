#include "selectionneurdenoeud.h"
#include "noeud.h"
#include "evt.h"
#include <QDebug>
#include <chrono>
#include <random>
#include "univers.h"
#include "genviehumain.h"

using std::shared_ptr;
using std::make_shared;

QList<std::shared_ptr<SelectionneurDeNoeud>> SelectionneurDeNoeud::s_TousLesSelectionneurs = {};

SelectionneurDeNoeud::SelectionneurDeNoeud(QString intitule, int bdd_id):m_BddId(bdd_id), m_Intitule(intitule)
{}

int SelectionneurDeNoeud::COMPTEUR_CHOIX = 0;
int SelectionneurDeNoeud::COMPTEUR_CHOIX_AUTO = 0;


void SelectionneurDeNoeud::AppliquerModeCHoix(std::shared_ptr<Effet> effetQuiRecoitChoix, int frequence)
{
    m_EffetQuiRecoitChoix = effetQuiRecoitChoix;
    m_Frequence = frequence;
}

std::shared_ptr<Noeud> SelectionneurDeNoeud::DeterminerNoeudSuivant()
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

        if( m_NoeudsProbables[i]->m_Noeud.lock()->TesterConditions() &&
                m_NoeudsProbables[i]->m_PoidsProba->CalculerProbaFinale() > 0 &&
                Univers::LOG &&
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

    // mode spécial : si AppliquerModeCHoix a été exécuté sur ce sélectionneur on peut avoir des moments réguliers ou au lieu d'être automatique
    //  la sélection devient un choix classique (avec un nombre limité)
    if ( m_EffetQuiRecoitChoix != nullptr && SelectionneurDeNoeud::COMPTEUR_CHOIX_AUTO >= m_Frequence)
    {
        // randomization de la liste (car seule une partie sera conservée)
        std::random_shuffle(noeudsPossibles.begin(), noeudsPossibles.end());

        m_EffetQuiRecoitChoix->SupprimerTousLesChoix();
        if ( m_EffetQuiRecoitChoix->m_Texte == "" )
            m_EffetQuiRecoitChoix->m_Texte = "Choisissez : ";

        for ( int j = 0; j < noeudsPossibles.size() && j < m_MaxChoix; ++j )
        {
            shared_ptr<Effet> effet = std::dynamic_pointer_cast<Effet>(noeudsPossibles[j]->m_Noeud.lock());
            if ( effet != nullptr) {
                double pb = noeudsPossibles[j]->m_PoidsProba->CalculerProbaFinale();
                QString txt = effet->GetNom() + " (proba : " + QString::number(pb) + ") ";
                shared_ptr<Choix> choix = GenHistoire::GetGenHistoire()->AjouterChoixGoToEffet(
                            txt, effet->m_Id, "", m_EffetQuiRecoitChoix);

            }
        }

        SelectionneurDeNoeud::COMPTEUR_CHOIX_AUTO = 0;
        return m_EffetQuiRecoitChoix;
    }
    else SelectionneurDeNoeud::COMPTEUR_CHOIX_AUTO++;

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
            return noeudsPossibles[j]->m_Noeud.lock();
        }
    }

    return noeudChoisi;
}
