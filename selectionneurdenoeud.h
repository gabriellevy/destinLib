#ifndef SELECTIONNEURDEVENEMENT_H
#define SELECTIONNEURDEVENEMENT_H
#include <QList>

class Noeud;
class Evt;
class Condition;

struct NoeudProbable {
    Noeud* m_Noeud;
    Condition* m_PoidsProba;

    NoeudProbable(Noeud* noeud, Condition* poids):m_Noeud(noeud), m_PoidsProba(poids) {}
};

/**
 * Le sélectionneur de noeuds est une sorte de go to evt/effet spécial qui contient une liste d'événement ou effets
 * et en sélectionne un selon le système de proba qui est exécuté. Il est conçu pour être appliqué à un noeud (plutôt à un choix ou un effet essentiellement)
 */
class SelectionneurDeNoeud
{

public:
    SelectionneurDeNoeud(QString intitule, int bdd_id = -1);

    /**
    *  liste des noeuds parmi lesquels un seul sera sélectionné par le sélectionneur
    * la condition m_PoidsProba doit obligatoireement contenr une proba, c'est elle qui détermine le poids et donc els chances d'arriver de ce noeud par rapport aux autres
    * plus le poids est haut plus la proba est grande (proportionnellement)
    * note : les conditions classiques, qu'elles soient de proba ou pas, sont à mettre sur le noeud lui même, pas dans le poids associé
    * */
    QList<NoeudProbable*> m_NoeudsProbables = {};
    Noeud* DeterminerEvtSuivant();

    // liste utilisées pour faciliter les chargements depuis la bdd
    static QList<SelectionneurDeNoeud*> s_TousLesSelectionneurs;
    int m_BddId;
    QString m_Intitule;
};

#endif // SELECTIONNEURDEVENEMENT_H
