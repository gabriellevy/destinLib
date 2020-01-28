#ifndef GENHISTOIRE_H
#define GENHISTOIRE_H

#include <QString>
#include "../abs/histoire.h"
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

    static GenHistoire* ME;

public:
    GenHistoire(QString titre = "");
    virtual ~GenHistoire();

    static GenHistoire* GetGenHistoire();

    std::shared_ptr<GenEvt> m_GenerateurEvt = nullptr;
    std::shared_ptr<Hist> m_HistoireGeneree = nullptr;
    virtual std::shared_ptr<Hist> GenererHistoire();
    virtual void FinGenerationHistoire();
    virtual void GenererPersos() = 0;
    virtual void GenererFonctionsCallback() {} // cette fonction a de bonnes chances d'être vides. Je la laisse en abstraite à implémenter comme pense-bête

    // fonctions racourci :
    std::shared_ptr<Effet> AjouterEffetGoToEffet(
            QString idEffetDest, QString id = "", std::shared_ptr<Evt> evt = nullptr);
    std::shared_ptr<Effet> AjouterEffetGoToEvt(
            QString idEvtDest, QString id = "", std::shared_ptr<Evt> evt = nullptr);
    std::shared_ptr<Effet> AjouterEffetNarration(
            QString text, QString cheminImg = "", QString id = "", std::shared_ptr<Evt> evt = nullptr);
    std::shared_ptr<Choix> AjouterChoixAjouteurACarac(
            QString texte, QString carac, QString valeur, QString go_to_effet_id = "", std::shared_ptr<Effet> effet = nullptr);
    std::shared_ptr<Choix> AjouterChoixGoToEffet(
            QString texte, QString go_to_effet_id, QString cheminImg = "", std::shared_ptr<Effet> effetDest = nullptr);

    std::shared_ptr<Evt> AjouterEvt(QString id, QString nom = "");
    std::shared_ptr<EvtAleatoire> AjouterEvtAleatoire(QString id, QString nom);
};

#endif // GENHISTOIRE_H
