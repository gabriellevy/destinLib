#ifndef SELECTIONNEURDEVENEMENT_H
#define SELECTIONNEURDEVENEMENT_H
#include <QList>

class Noeud;
class Evt;
/**
 * Le sélectionneur d'événement est une sorte de go to evt spécial qui contient une liste d'événement
 * et en sélectionne un selon le système de proba qui est exécuté. Il est conçu pour être appliqué à un noeud (plutôt à un choix ou un effet essentiellement)
 */
class SelectionneurDEvenement
{

public:
    SelectionneurDEvenement(QString intitule, int bdd_id = -1);

    QList<Evt*> m_Evts = {};
    Evt* DeterminerEvtSuivant();

    // liste utilisées pour faciliter les chargements depuis la bdd
    static QList<SelectionneurDEvenement*> s_TousLesSelectionneurs;
    int m_BddId;
    QString m_Intitule;
};

#endif // SELECTIONNEURDEVENEMENT_H
