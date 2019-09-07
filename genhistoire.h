#ifndef GENHISTOIRE_H
#define GENHISTOIRE_H

#include <QString>
#include "histoire.h"
#include "genevt.h"

/**
 * @brief Class servant à générer toute l'histoire du jeu destin. Une fois cette exécution terminée elle renvoie un objet ExecHistoire à exécutee t n'est plus utilisée.
 */
class GenHistoire
{
protected:

    // génère toutes les caracs qui peuvent être visualisées par le joueur (d'autres caracs peuvent être générées et invisibles n'importe quand dans l'aventure)
    // les caracs peuvent aussi être initialisée via  la fonction DPerso::InitialiserPerso()
    // essayer de créer les caracs de perso dans InitialiserPerso et celle du monde ici (même si d'un point de vue technique elles sont identiques au final)
    virtual void GenererCaracs() = 0;

public:
    GenHistoire();
    virtual ~GenHistoire();

    GenEvt* m_GenerateurEvt = nullptr;
    Hist* m_HistoireGeneree = nullptr;
    virtual Hist* GenererHistoire() = 0;
    virtual void FinGenerationHistoire();
    virtual void GenererPersos() = 0;
    virtual void GenererFonctionsCallback() {} // cette fonction a de bonnes chances d'être vides. Je la laisse en abstraite à implémenter comme pense-bête

    // fonctions racourci :
    Effet* AjouterEffetNarration(QString text, QString cheminImg = "", QString id = "", Evt* evt = nullptr);
    Choix* AjouterChoixAjouteurACarac(QString texte, QString carac, QString valeur, QString go_to_effet_id = "", Effet* effet = nullptr);
    Choix* AjouterChoixGoToEffet(QString texte, QString go_to_effet_id, QString cheminImg = "", Effet* effetDest = nullptr);

    Evt* AjouterEvt(QString id, QString nom = "");
    EvtAleatoire* AjouterEvtAleatoire(QString id, QString nom);
};

#endif // GENHISTOIRE_H
