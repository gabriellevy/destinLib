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

class Noeud// : public QWidget
{
protected:
    QString m_NouvelEtatPartie = "";
    // temp écoulé durant l'exécution de ce noeud. Il s'agit de temps de jeu et ses unités sont à l'appréciation du créateur de l'aventure puisqu'il en détermine toutes les utilisations
    float m_TempEcoule;
    QPixmap m_Img;
    QMovie* m_Film;


public:
    explicit Noeud(QJsonObject evtJson/*, QWidget *parent = 0*/);
    explicit Noeud(QString id, QString nom, QString text);
    explicit Noeud();
    virtual ~Noeud();

    QString m_ChangePerso = ""; // à l'exécution de ce noeud on change de personnage principal joué par le joueur pour le remplacer par celui qui a cet id
    QString m_Id = "";
    QString m_Nom = "";
    QString m_Son = "";
    //QString m_ImgPath = "";
    QString m_GoToEvtId = "";
    QString m_GoToEffetId = "";
    QList<SetCarac> m_SetCaracs;
    QList<QString> m_Themes;
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

    EtatCondition m_EtatCondition = ec_NonTeste;
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
    bool AUnDeCesThemes(QList<QString> themes);

    // modifier caracs
    void AjouterRetireurACarac(QString id, QString valeur);
    void AjouterAjouteurACarac(QString id, QString valeur);
    void AjouterChangeurDeCarac(QString id, QString valeur);
    void AjouterSetCaracTrue(QString id);
    Condition* AjouterCondition( QString caracId, Comparateur comparateur, QString valeur);
    Condition* AjouterConditionProba( double proba);
    Noeud* AjouterElse(QString text = "");

    // function qui sera exécutée lors de l'exécution de ce noeud
    void AjouterCallback(std::function<void(QString)> callback, QString arg);

    // function qui déterminera si ce noeud est exécuté (runtime)
    void AjouterCallbackDeTest(std::function<bool(QString)> callback, QString arg);

    // callback functions
    std::function<void(QString)> m_CallbackFunction = nullptr;// PREMIER ESSAI tmp?
    QString m_CallbackArgument;
    std::function<bool(QString)> m_CallbackTest = nullptr;// PREMIER ESSAI tmp?
    QString m_CallbackArgumentTest;

    virtual void FinExecutionNoeud();

private:
    //Ui::Noeud *ui;
};

#endif // NOEUD_H
