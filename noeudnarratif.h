#ifndef NOEUDNARRATIF_H
#define NOEUDNARRATIF_H

#include "noeud.h"

class NoeudNarratif : public Noeud
{
protected:
    NoeudNarratif();
    explicit NoeudNarratif(QString id, QString nom, QString text);

public:
    virtual ~NoeudNarratif();

    virtual bool TesterConditions();

    QString m_Nom = "";
    QString m_Son = "";
    float m_Duree;
    QString m_Text = "";
    QString m_ImgPath = "";
    QString m_FilmPath = "";
    // testé après exécution : tant que cette condition est vrai le noeud est réexécuté en boucle
    // si il n'y a pas de condition de répétition, on passe à l'effet suivant normalement
    QList<Condition*> m_RepeatWhileConditions;

    /**
     * @brief TexteAAfficher
     * @return texte tel qu'il doit être affiché dans le jeu (avec des post traitements
     */
    QString TexteAAfficher();

    /**
     * @return true si ce noeud doit marquer une pause lors de son exécution pour afficher du texte ou une image à l'utilisateur
     */
    virtual bool AQuelqueChoseAAfficher();

    void ChangerChrono( int ms );

    // nombre de millisecondes d'attente sur ce noeud avant qu'on passe automatiquement au suivant
    // si égal à -1, pas de chrono : on affiche un bouton "continuer" à la place
    // dans le cas d'un chrono sur un evt, tous les effets ont ce même chrono appliqué à eux
    // dans un effet contenant un choix, le chrono ne cache pas les boutons de choix mais, si il atteint son terme avant que le joueur n'ai choisi, un choix aléatoire est choisi.
    int m_MsChrono = -1;
    // temp écoulé durant l'exécution de ce noeud. Il s'agit de temps de jeu et ses unités sont à l'appréciation du créateur de l'aventure puisqu'il en détermine toutes les utilisations
    double m_TempEcoule;

    double GetTempEcoule();
    void AjouterDuree(float duree);

    // gestion de la bdd :
    int m_BDD_NoeudId;
    void AppliquerValeurDeNoeudBDD(int bd_id);
    // charger une condition peut s'appliquer à plusieurs types de noeuds
    // donc l'id est celui de la partie noeud mais il peut correspondre par exemple à un effet, un evt, un choix...
    void ChargerConditionsBdd();
    void ChargerSetCaracBdd();
    void ChargerFonctionsCallbacksBdd();
    void ChargerFonctionsTestCallbacksBdd();
    void ChargerSelectionneurEvtBdd();


    friend class GenEvt;

};

#endif // NOEUDNARRATIF_H