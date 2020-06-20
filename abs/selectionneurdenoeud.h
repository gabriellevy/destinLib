#ifndef SELECTIONNEURDEVENEMENT_H
#define SELECTIONNEURDEVENEMENT_H
#include <QList>
#include <memory>

class Noeud;
class Evt;
class Effet;
class Condition;

struct NoeudProbable {
    std::weak_ptr<Noeud> m_Noeud;
    std::shared_ptr<Condition> m_PoidsProba;

    NoeudProbable(std::shared_ptr<Noeud> noeud, std::shared_ptr<Condition> poids):m_Noeud(noeud), m_PoidsProba(poids) {}
};

/**
 * Le sélectionneur de noeuds est une sorte de go to evt/effet spécial qui contient une liste d'événement ou effets
 * et en sélectionne un selon le système de proba qui est exécuté. Il est conçu pour être appliqué à un noeud (plutôt à un choix ou un effet essentiellement)
 */
class SelectionneurDeNoeud
{
    // valeurs liées au mode de choix qui peut remplacer le mode aléatoire si activé avec fonction "AppliquerModeCHoix"
    static int COMPTEUR_CHOIX;
    static int COMPTEUR_CHOIX_AUTO;
    std::shared_ptr<Effet> m_EffetQuiRecoitChoix = nullptr;
    int m_Frequence = -1;
    int m_MaxChoix = 6;
public:
    SelectionneurDeNoeud(QString intitule, int bdd_id = -1);

    void AppliquerModeCHoix(std::shared_ptr<Effet> effetQuiRecoitChoix, int frequence = -1);

    /**
    *  liste des noeuds parmi lesquels un seul sera sélectionné par le sélectionneur
    * la condition m_PoidsProba doit obligatoireement contenr une proba, c'est elle qui détermine le poids et donc els chances d'arriver de ce noeud par rapport aux autres
    * plus le poids est haut plus la proba est grande (proportionnellement)
    * note : les conditions classiques, qu'elles soient de proba ou pas, sont à mettre sur le noeud lui même, pas dans le poids associé
    * */
    QList<std::shared_ptr<NoeudProbable>> m_NoeudsProbables = {};
    std::shared_ptr<Noeud> DeterminerNoeudSuivant();

    // liste utilisées pour faciliter les chargements depuis la bdd
    static QList<std::shared_ptr<SelectionneurDeNoeud>> s_TousLesSelectionneurs;
    int m_BddId;
    QString m_Intitule;
};

#endif // SELECTIONNEURDEVENEMENT_H
