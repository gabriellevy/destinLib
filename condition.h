#ifndef CONDITION_H
#define CONDITION_H
#include <QString>
#include <QJsonObject>
#include <QList>

enum Comparateur {
    c_Egal,
    c_Superieur,
    c_SuperieurEgal,
    c_InferieurEgal,
    c_Inferieur,
    c_Different,
    c_Aucun
};

enum TypeProba {
    // doit forcément être entre 0 et 1, est utilisé comme condition de base pour l'exécution du noeud en question
    // cas d'un sélectionneur de noeuds probables : le total des proba pures d'un sélectionneur ne peut jamais
    // dépasser 1 en tout, car leur valeur est vraiment une proba fixe et qu'un seul résultat doit être possible
    // (ils sont donc mutuellement exclusifs)
    p_Pure,
    // peut avoir n'importe quelle valeur qui est en fait un poids relatif aux autres proba relatives du même sélectionneur
    // les proba pures sont prioritaires, si aucun n'est sélectionnée parmi elles, on détermine un résultat parmi les proba relatives
    // (et là une est toujours choisie)
    // utilisé seulement dans les sélectionneurs de noeuds
    p_Relative
};

/*enum EtatCondition {
    ec_NonTeste, // cette condition n'a pas encore été testée durant cette itération
    ec_True,
    ec_False
};*/

class Condition;

class ModifProba
{
public:
    double m_Valeur = 0.0;
    QList<Condition*> m_Conditions;
    int m_BDD_Id;

    ModifProba(double valeur);
    ModifProba();
    void ChargerConditionsBdd();
};

class Condition
{
    TypeProba m_TypeProba;
    double m_Proba = -1.0;

public:
    Condition();
    Condition(QString caracId, QString valeur, Comparateur comparateur);
    Condition(double proba, TypeProba typeProba);

    QList<ModifProba*> m_ModifsProba;
    QString m_CaracId;
    //double m_ValeurNombre; // valeur à comparer si il s'agit d'un nombre
    QString m_Valeur = "0"; // valeur à comparer si il s'agit d'une chaîne de caractères
    Comparateur m_Comparateur = c_Aucun;
    // proba (sur 1) que cette condition soit vérifiée

    /**
     * @brief Teste la condition avec les valeurs actuelles du personnage joueur
     * @return true si la condition est vraie...
     */
    bool Tester();

    bool IsProbaPure();

    double CalculerProbaFinale();

    /**
     * @brief Récupère toutes les conditions du noeud json et les ajoute dans la liste de conditions passée en référence
     * @return
     */
    static void RemplirListeCondition( QJsonObject objJson, QList<Condition*> &conditions, bool conditionsWhile);

    static Condition* CreerConditionDepuisObject(QString balise, QJsonObject obj);

    static bool TesterTableauDeConditions(QList<Condition*> &conditions);

    ModifProba* AjouterModifProba(double valeur, QList<Condition*> conditions);
    ModifProba* AjouterModifProbaVide();

    // concertisseur string/Comparateur
    static Comparateur GetComparateurFromStr(QString compStr);
    static QString GetStrFromComparateur(Comparateur comp);
    static Comparateur GetComparateurFromStrSigne(QString comp);

    // gestion de la bdd :
    int m_BDD_CondId;
    void ChargerModifProbaBdd();
};

#endif // CONDITION_H
