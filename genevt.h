#ifndef GENEVT_H
#define GENEVT_H

#include <QString>

#include "evt.h"
#include "evtaleatoire.h"

struct ResExecutionLancerDe;

/**
 * @brief cette classe contient toutes les fonctions pour créer un événement et ses effets internes.
 * Elle ne doit être utilisée que dans la pahse génération de l'aventure donc au tout début.
 */
class GenEvt
{
private:
    Evt* GenererEvt(QString id, QString nom);

public:
    GenEvt();

    EvtAleatoire* GenererEvtAleatoire(QString id, QString nom);

    // pointeur sur le dernier événement généré. Par défaut, tous les effets ajoutés le seront à cet événement sauf si on ajoute un autre événement en paramètre
    Evt* m_DernierEvtGenere;
    // pointeur sur le dernier effet généré. Par défaut les ajouts à un effet (choix par exemple) le seront à celui ci sauf si un autre est précisé en paramètre
    Effet* m_DernierEffetGenere;

    Effet* AjouterEffetModificateurCarac(QString caracId, QString nouvelleValeur, QString text = "", QString id = "", Evt* evtDest = nullptr);
    Effet* AjouterEffetRetireurACarac(QString caracId, QString valeurRetire, QString text = "", QString id = "", Evt* evtDest = nullptr);
    Effet* AjouterEffetNarration(QString text, QString cheminImg = "", QString id = "", Evt* evtDest = nullptr);
    Effet* AjouterEffetCallbackDisplay(std::function<void()> callbackDisplay, QString text = "", QString cheminImg = "", QString id = "", Evt* evtDest = nullptr);
    Effet* AjouterEffetChangementPerso(QString persoId, QString text, QString cheminImg = "", QString id = "", Evt* evtDest = nullptr);
    Effet* AjouterEffetTest(QString caracId, Comparateur comparateur, QString valeur, QString id = "", Evt* evtDest = nullptr);
    Effet* AjouterEffetVide(Evt* evtDest = nullptr);
    Effet* AjouterEffetGlisseur(QString text, QString valeur_min, QString valeur_max, QString valeur_depart,
                                QString carac_id, QString cheminImg = "", QString id = "", Evt* evtDest = nullptr );
    Effet* AjouterEffetAjouteurACarac(QString caracId, QString valeurAjoutee, QString id = "", Evt* evtDest = nullptr);
    Effet* AjouterEffetSelectionneurDeNoeud(QString id, QString text, Evt* evtDest);

    // fonctions intermédiaires d'ajouts de choix dans les effets :
    Choix* AjouterChoixVide(Effet* effetDest = nullptr);
    Choix* AjouterChoixAjouteurACarac(QString text, QString carac, QString valeur, QString go_to_effet_id = "", Effet* effetDest = nullptr);
    Choix* AjouterChoixChangeurDeCarac(QString text, QString carac, QString valeur, QString go_to_effet_id = "", Effet* effetDest = nullptr);
    Choix* AjouterChoixGoToEffet(QString text, QString go_to_effet_id, QString cheminImg = "", Effet* effetDest = nullptr);

    LancerDe* AjouterLancerDe(QString texte, int nbDes, std::function<ResExecutionLancerDe*(int/*, QVector<QString>*/)>,
               /*QVector<QString> params, */Effet* effet = nullptr);

    /**
     * @brief AjouterImgFond
     * @param fond : chemin vers l'image à utiliser en fond. Exemple : ':/images/fond.jpg'
     */
    void AjouterImgFond(Evt* evt, QString fond);

    // gestion de la bdd :
    /**
     * @brief charge les effets appartenent à cet événement depuis la bdd
     * @param evt_id
     */
    virtual void ChargerEffetsBdd(Evt* evtDest);
    void ChargerChoixBdd(Effet* effet = nullptr);
    /**
     * @brief dans le cas où cet événement appartiendrait à un système de sélection d'événement on l'y ajoute :
     */
    void AjouterASelectionneurEvt(Evt* evt, Condition* poids, int selectionneur_bdd_id);

    /**
     * @brief crée un effet qui lors de son exécution enverra vers un des noeuds passés en paramètres selon leurs probas
     * @param evt : événement auquel ajouter l'effet
     * @param noeudsDestination : noeuds vers lesquel l'effet redirigera. Ils doivent avoir une proba > 0
     * @param id : id du noeud à creéer
     * @return effet généré
     */
    Effet* AjouterEffetSelecteurDEvt(QVector<NoeudProbable*> noeudsDestination, QString id = "", QString text = "", Evt* evt = nullptr);

    Noeud* GenererNoeudModificateurCarac(QString caracId, QString nouvelleValeur, QList<Condition*> conditions = {});

private:

    // événement en cours de génération par cet objet
    //Evt* m_EvtGenere;

    friend class GenHistoire;

    // fonction intermédiaire d'ajout d'effet qui doit être appelée par toutes les fonctions d'ajout d'effets publiques plus spécifiques
    Effet* AjouterEffet(Effet* effet, Evt* evtDest = nullptr);
};

#endif // GENEVT_H
