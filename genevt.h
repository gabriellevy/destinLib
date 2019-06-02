#ifndef GENEVT_H
#define GENEVT_H

#include <QString>

#include "evt.h"
#include "evtaleatoire.h"

class GenEvt
{
public:
    GenEvt();

    Evt* GenererEvt(QString id, QString nom);
    EvtAleatoire* GenererEvtAleatoire(QString id, QString nom);

    // fonction intermédiaire d'ajout d'effet qui doit être appelée par toutes les fonctions d'ajout d'effets plus spécifiques ci après
    Effet* AjouterEffet(Evt* evtDest, Effet* effet);
    Effet* AjouterEffetModificateurCarac(Evt* evtDest, QString caracId, QString nouvelleValeur, QString text = "", QString id = "");
    Effet* AjouterEffetRetireurACarac(Evt* evtDest, QString caracId, QString valeurRetire, QString text = "", QString id = "");
    Effet* AjouterEffetNarration(Evt* evtDest, QString text, QString cheminImg = "", QString id = "");
    Effet* AjouterEffetChangementPerso(Evt* evtDest, QString persoId, QString text, QString cheminImg = "", QString id = "");
    Effet* AjouterEffetTest(Evt* evtDest, QString caracId, Comparateur comparateur, QString valeur, QString id = "");
    Effet* AjouterEffetVide(Evt* evtDest);
    Effet* AjouterEffetGlisseur(Evt* evtDest, QString text, QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id, QString cheminImg = "", QString id = "" );
    Effet* AjouterEffetAjouteurACarac(Evt* evtDest, QString caracId, QString valeurAjoutee, QString id = "");

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
    /**
     * @brief dans le cas où cet événement appartiendrait à un système de sélection d'événement on l'y ajoute :
     */
    void AjouterASelectionneurEvt(Evt* evt, int selectionneur_bdd_id);

private:

    // événement en cours de génération par cet objet
    //Evt* m_EvtGenere;

};

#endif // GENEVT_H
