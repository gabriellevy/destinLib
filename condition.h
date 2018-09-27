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

enum EtatCondition {
    ec_NonTeste, // cette condition n'a pas encore été testée durant cette itération
    ec_True,
    ec_False
};

class Condition;

class ModifProba
{
public:
    double m_Valeur = 0.0;
    QList<Condition*> m_Conditions;

    ModifProba(double valeur);
    ModifProba();
};

class Condition
{
    QString m_CaracId;
    //double m_ValeurNombre; // valeur à comparer si il s'agit d'un nombre
    QString m_Valeur = "0"; // valeur à comparer si il s'agit d'une chaîne de caractères
    Comparateur m_Comparateur = c_Aucun;
    // proba (sur 1) que cette condition soit vérifiée
    double m_Proba = -1.0;

public:
    Condition();
    Condition(QString caracId, QString valeur, Comparateur comparateur);
    Condition(double proba);

    QList<ModifProba*> m_ModifsProba;

    /**
     * @brief Teste la condition avec les valeurs actuelles du personnage joueur
     * @return true si la condition est vraie...
     */
    bool Tester();

    double CalculerProbaFinale();

    /**
     * @brief Récupère toutes les conditions du noeud json et les ajoute dans la liste de conditions passée en référence
     * @return
     */
    static void RemplirListeCondition( QJsonObject objJson, QList<Condition*> &conditions, bool conditionsWhile);

    static Condition* CreerConditionDepuisObject(QString balise, QJsonObject obj);

    static bool TesterTableauDeConditions(QList<Condition*> &conditions);

    ModifProba* AjouterModifProba(double valeur, QList<Condition*> conditions);
};

#endif // CONDITION_H
