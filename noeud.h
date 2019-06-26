#ifndef NOEUD_H
#define NOEUD_H

#include "setcarac.h"
#include "condition.h"
#include "selectionneurdenoeud.h"

/*namespace Ui {
class Noeud;
}*/

/**
 * @brief référence à un appel d'une fonction. Cette fonction est récupéré dans
 * la map des fonctions callback  du noeud actuel et on lui applique les arguments associés
 */
struct AppelCallback {
    int m_BDD_FonctId;
    QString m_NomFonction;
    QVector<QString> m_ArgumentsCaracId;
    QVector<QString> m_ArgumentsParValeur;

    void ChargerArgumentsBdd();

};

enum TypeNoeud {
    etn_Noeud,
    etn_NoeudNarratif,
    etn_Effet,
    etn_Evt,
    etn_Choix
};

class Noeud// : public QWidget
{
protected:


public:
   // explicit Noeud(QJsonObject evtJson/*, QWidget *parent = 0*/);
    explicit Noeud(QString id);
    explicit Noeud();
    virtual ~Noeud();

    QString m_NouvelEtatPartie = "";
    TypeNoeud m_TypeNoeud;
    QString m_ChangePerso = ""; // à l'exécution de ce noeud on change de personnage principal joué par le joueur pour le remplacer par celui qui a cet id
    QString m_Id = "";
    //QString m_ImgPath = "";
    QString m_GoToEvtId = "";
    SelectionneurDeNoeud* m_SelectionneurDeNoeud = nullptr;
    QString m_GoToEffetId = "";
    QList<SetCarac*> m_SetCaracs;

    // condition à respecter pour exécuter ce noeud (si il y en a une)
    QList<Condition*> m_Conditions;

    // renvoit la proba asscié au noeud (via if_proba et les modif_proba) si il y en a une
    double GetProba();
    /**
     * @brief stocke le rsultat de la condition de ce noeud durant cette itération (au cas où le résultat de ce test est variable)
     * Cette valeur est remise à non testé quand on quitte le noeud, pour le cas où on y reviendrait
     */
    //EtatCondition m_EtatCondition = ec_NonTeste;
    virtual bool TesterConditions();

    /**
     * @return true si ce noeud possède dans m_Themes au moins u ndes thèems du paramètre
     */
    //bool AUnDeCesThemes(QList<QString> themes);

    // modifier caracs
    SetCarac* AjouterRetireurACarac(QString id, QString valeur);
    SetCarac* AjouterAjouteurACarac(QString id, QString valeur);
    SetCarac* AjouterChangeurDeCarac(QString id, QString valeur);
    SetCarac* AjouterSetCaracTrue(QString id);
    Condition* AjouterCondition( QString caracId, Comparateur comparateur, QString valeur);
    Condition* AjouterConditionProba( double proba);
    //Noeud* AjouterElse(QString text = "");

    // function qui sera exécutée lors de l'exécution de ce noeud
    //void AjouterCallback(std::function<void(QVector<QString>)> callback, QVector<QString> arg);

    // function qui déterminera si ce noeud est exécuté (runtime)
    //void AjouterCallbackDeTest(std::function<bool(QVector<QString>)> callback, QVector<QString> arg);

    // fonctions runtime callbacks. Ce sont des fonctions personnalisables qui sont exécutées lors du jeu, pas à sa préparation
    /*QVector<std::function<void(QVector<QString>)>> m_CallbackFunctions;
    QVector<QVector<QString>> m_CallbackArguments;
    QVector<std::function<bool(QVector<QString>)>> m_CallbackTestFunctions;
    QVector<QVector<QString>> m_CallbackTestArguments;*/

    QVector<AppelCallback*> m_FonctionsAppellees;
    QVector<AppelCallback*> m_FonctionsDeTest;


protected:
};

#endif // NOEUD_H
