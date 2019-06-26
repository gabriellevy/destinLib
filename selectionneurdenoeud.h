#ifndef SELECTIONNEURDEVENEMENT_H
#define SELECTIONNEURDEVENEMENT_H
#include <QList>

class Noeud;
class Evt;
/**
 * Le sélectionneur de noeuds est une sorte de go to evt/effet spécial qui contient une liste d'événement ou effets
 * et en sélectionne un selon le système de proba qui est exécuté. Il est conçu pour être appliqué à un noeud (plutôt à un choix ou un effet essentiellement)
 */
class SelectionneurDeNoeud
{

public:
    SelectionneurDeNoeud(QString intitule, int bdd_id = -1);

    QList<Noeud*> m_Noeuds = {};
    Noeud* DeterminerEvtSuivant();

    // liste utilisées pour faciliter les chargements depuis la bdd
    static QList<SelectionneurDeNoeud*> s_TousLesSelectionneurs;
    int m_BddId;
    QString m_Intitule;
};

#endif // SELECTIONNEURDEVENEMENT_H
