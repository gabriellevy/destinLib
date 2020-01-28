#ifndef NOEUD_H
#define NOEUD_H

#include "setcarac.h"
#include "condition.h"
#include "selectionneurdenoeud.h"
#include <memory>


/**
 * @brief référence à un appel d'une fonction. Cette fonction est récupéré dans
 * la map des fonctions callback  du noeud actuel et on lui applique les arguments associés
 */
struct AppelCallback {
    int m_BDD_FonctId;

    AppelCallback(QString fonction):m_NomFonction(fonction) {}
    AppelCallback(QString fonction, QVector<QString> argumentsCaracId, QVector<QString> argumentsParValeur)
        :m_NomFonction(fonction), m_ArgumentsCaracId(argumentsCaracId), m_ArgumentsParValeur(argumentsParValeur)
    {}

    QString m_NomFonction;
    QVector<QString> m_ArgumentsCaracId;
    QVector<QString> m_ArgumentsParValeur;

    void ChargerArgumentsBdd();

};

enum ModeDeroulement : int;
enum PhaseDeroulement : int;

enum TypeNoeud {
    etn_Noeud,
    etn_NoeudNarratif,
    etn_Effet,
    etn_Evt,
    etn_Choix,
    etn_LancerDe
};

class Noeud// : public QWidget
{
protected:


public:

    explicit Noeud(QString id);
    explicit Noeud();
    virtual ~Noeud();

    TypeNoeud m_TypeNoeud;
    QString m_ChangePerso = ""; // à l'exécution de ce noeud on change de personnage principal joué par le joueur pour le remplacer par celui qui a cet id
    QString m_Id = "";
    //QString m_ImgPath = "";
    QString m_GoToEvtId = "";
    std::shared_ptr<SelectionneurDeNoeud> m_SelectionneurDeNoeud = nullptr;
    QString m_GoToEffetId = "";
    QList<std::shared_ptr<SetCarac>> m_SetCaracs;
    ModeDeroulement m_ChangeurModeDeroulement; // ce noeud une fois exécuté change le mode de déroulement de l'histoire
    PhaseDeroulement m_ChangeurPhaseDeroulement; // ce noeud une fois exécuté change la phase de déroulement de l'histoire

    // condition à respecter pour exécuter ce noeud (si il y en a une)
    QList<std::shared_ptr<Condition>> m_Conditions;

    // renvoit la proba asscié au noeud (via if_proba et les modif_proba) si il y en a une
    double GetProba();
    /**
     * @brief stocke le rsultat de la condition de ce noeud durant cette itération (au cas où le résultat de ce test est variable)
     * Cette valeur est remise à non testé quand on quitte le noeud, pour le cas où on y reviendrait
     */
    //EtatCondition m_EtatCondition = ec_NonTeste;
    virtual bool TesterConditions();

    // modifier caracs
    std::shared_ptr<SetCarac> AjouterRetireurACarac(QString id, QString valeur);
    std::shared_ptr<SetCarac> AjouterRetireurACarac(QString id, int valeur);
    std::shared_ptr<SetCarac> AjouterAjouteurACarac(QString id, QString valeur);
    std::shared_ptr<SetCarac> AjouterAjouteurACarac(QString id, int valeur);
    std::shared_ptr<SetCarac> AjouterChangeurDeCarac(QString id, QString valeur);
    std::shared_ptr<SetCarac> AjouterSetCaracTrue(QString id);
    std::shared_ptr<Condition> AjouterCondition( QString caracId, Comparateur comparateur, QString valeur);
    std::shared_ptr<Condition> AjouterConditionProbaPure( double proba);
    std::shared_ptr<Condition> AjouterConditionProbaRelative( double proba);

    QVector<std::shared_ptr<AppelCallback>> m_FonctionsAppellees;
    QVector<std::shared_ptr<AppelCallback>> m_FonctionsDeTest;

    std::function<void()> m_CallbackDisplay = nullptr; // callback de base appelée au début de l'exécution de l'effet et qui affecte l'affichage (texte, image...)


protected:
};

#endif // NOEUD_H
