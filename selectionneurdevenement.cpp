#include "selectionneurdevenement.h"
#include "noeud.h"
#include "evt.h"
#include <QDebug>

QList<SelectionneurDEvenement*> SelectionneurDEvenement::s_TousLesSelectionneurs = {};

SelectionneurDEvenement::SelectionneurDEvenement(QString intitule, int bdd_id):m_BddId(bdd_id), m_Intitule(intitule)
{}

Evt* SelectionneurDEvenement::DeterminerEvtSuivant()
{
    QList<Evt*> evtsPossibles;
    double totalDesProbas = 0;
    for ( int i = 0 ; i < m_Evts.size() ; ++i)
    {
        if ( m_Evts[i]->TesterConditions())
        {
            double proba = m_Evts[i]->GetProba();
            Q_ASSERT_X(proba >= 0,
                       "Attention un événement inclu dans un sélectionneur d'événement doit avoir une proba pour être sélectionnable !",
                       "SelectionneurDEvenement::DeterminerEvtSuivant");
            if ( proba != 0)
            {
                evtsPossibles.append(m_Evts[i]);
                totalDesProbas += proba;
            }
        } else {
            qDebug()<<"Cet événement ne peut pas arriver : " << m_Evts[i]->m_Id;
        }
    }

    QString txt = "Aucun evt trouvé dans le sélectionneur d'événement ";
    Q_ASSERT_X(  evtsPossibles.size() > 0,
                 "DeterminerEvtAleatoire",
                 txt.toStdString().c_str() );

    // sélectionner un de ces événements (en fonction de leur proba) et l'exécuter :
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);// entre 0 et 1
    double probaIndicator = static_cast<double>(r) * totalDesProbas;
    double totalCourantDesProbas = 0;
    Evt* evtchoisi = nullptr;

    for ( int j = 0; j < evtsPossibles.size() ; ++j )
    {
        totalCourantDesProbas += evtsPossibles[j]->GetProba();
        if ( probaIndicator < totalCourantDesProbas)
        {
            // cet événement est sélectionné :
            evtchoisi = evtsPossibles[j];
            break;
        }
    }

    return evtchoisi;
}
