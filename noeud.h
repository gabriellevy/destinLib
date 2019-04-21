#ifndef NOEUD_H
#define NOEUD_H

#include <QWidget>
#include <QMessageBox>
#include "setcarac.h"
#include "condition.h"
#include <QMovie>

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
    etn_Effet,
    etn_Evt,
    etn_Choix
};

class Noeud// : public QWidget
{
protected:
    QString m_NouvelEtatPartie = "";
    // temp écoulé durant l'exécution de ce noeud. Il s'agit de temps de jeu et ses unités sont à l'appréciation du créateur de l'aventure puisqu'il en détermine toutes les utilisations
    double m_TempEcoule;
    QPixmap m_Img;
    QMovie* m_Film;


public:
   // explicit Noeud(QJsonObject evtJson/*, QWidget *parent = 0*/);
    explicit Noeud(QString id, QString nom, QString text);
    explicit Noeud();
    virtual ~Noeud();

    TypeNoeud m_TypeNoeud;
    QString m_ChangePerso = ""; // à l'exécution de ce noeud on change de personnage principal joué par le joueur pour le remplacer par celui qui a cet id
    QString m_Id = "";
    QString m_Nom = "";
    QString m_Son = "";
    //QString m_ImgPath = "";
    QString m_GoToEvtId = "";
    QString m_GoToEffetId = "";
    QList<SetCarac*> m_SetCaracs;
    //QList<QString> m_Themes;
    float m_Duree;
    QString m_Text = "";
    // testé après exécution : tant que cette condition est vrai le noeud est réexécuté en boucle
    // si il n'y a pas de condition de répétition, on passe à l'effet suivant normalement
    QList<Condition*> m_RepeatWhileConditions;

    double GetTempEcoule();
    void AjouterDuree(float duree);

    // condition à respecter pour exécuter ce noeud (si il y en a une)
    QList<Condition*> m_Conditions;

    // renvoit la proba asscié au noeud (via if_proba et les modif_proba) si il y en a une
    double GetProba();

    /**
     * @brief TexteAAfficher
     * @return texte tel qu'il doit être affiché dans le jeu (avec des post traitements
     */
    QString TexteAAfficher();

    /**
     * @brief stocke le rsultat de la condition de ce noeud durant cette itération (au cas où le résultat de ce test est variable)
     * Cette valeur est remise à non testé quand on quitte le noeud, pour le cas où on y reviendrait
     */
    //EtatCondition m_EtatCondition = ec_NonTeste;
    bool TesterConditions();
    /**
     * @brief AQuelqueChoseAAfficher
     * @return true si ce noeud doit marquer une pause lors de son exécution pour afficher du texte ou une image à l'utilisateur
     */
    virtual bool AQuelqueChoseAAfficher();

    /**
     * @brief exécute tout ce qui est défini dans ce noeud (changements de caracs, go to un autre effet etc...
     * @param afficherNoeud : si true la fonction affiche le texte et les images du noeud
     * @param lancerNoeudSuivantSiRienAAfiicher : si true la fonction exécute automatiquement le noeud suivant si il n'y a rien à afficher
     */
    virtual void ExecuterActionsNoeud(/*bool afficherNoeud = true, bool lancerNoeudSuivantSiRienAAfiicher = true*/);

    /**
     * @brief S'exécute quand l'exécutyion de l'aventure arrive à ce noeud
     * Inclut l'affichage, puiséventuellement l'exécution des actions, puis la transition éventuelle vers l'action suivante
     */
    virtual void LancerNoeud();


    virtual void AfficherNoeud();

    /**
     * @brief Gère le passage au noeud suivant quand celui ci est exécuté
     * @return true si la transition est automatique sans besoin de clic de l'utilisateur, false sinon (pour éviter les rafraichissements multiples inutiles)
     */
    virtual bool GestionTransition();

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
    Noeud* AjouterElse(QString text = "");

    virtual void FinExecutionNoeud();

    void ChangerChrono( int ms );

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

    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1) = 0;

    // gestion de la bdd :
    int m_BDD_NoeudId;
    void AppliquerValeurDeNoeudBDD(int bd_id);
    // charger une condition peut s'appliquer à plusieurs types de noeuds
    // donc l'id est celui de la partie noeud mais il peut correspondre par exemple à un effet, un evt, un choix...
    void ChargerConditionsBdd();
    void ChargerSetCaracBdd();
    void ChargerFonctionsCallbacksBdd();
    void ChargerFonctionsTestCallbacksBdd();

private:
    //Ui::Noeud *ui;

protected:

    // nombre de millisecondes d'attente sur ce noeud avant qu'on passe automatiquement au suivant
    // si égal à -1, pas de chrono : on affiche un bouton "continuer" à la place
    // dans le cas d'un chrono sur un evt, tous les effets ont ce même chrono appliqué à eux
    // dans un effet contenant un choix, le chrono ne cache pas les boutons de choix mais, si il atteint son terme avant que le joueur n'ai choisi, un choix aléatoire est choisi.
    int m_MsChrono = -1;
};

#endif // NOEUD_H
