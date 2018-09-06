#ifndef CULTEGENESTEALER_H
#define CULTEGENESTEALER_H

#include "histoire.h"


class CulteGenestealer : public Histoire
{
public:
    CulteGenestealer(QWidget *parent = nullptr);

    virtual void GenererHistoire();

    // callbacks spécifiques :
    static void TempPasse(QString nbJoursStr);
    static bool TestReactionImperium(QString modificateur_negatif);

    static QString repererImperium;
    static QString joursSansManger;
    static QString age_patriarche;
    static QString nombre_insemines;
    static QString nombre_hybrides_gen1;
    static QString nombre_hybrides_gen2;
    static QString nombre_hybrides_gen3;
    static QString patrouilles_humaines;// niveau de "" à n'importe quel chiffre
    static QString autorisation_insemines;// "enfermes_au_nid", "vont_et_viennent" ou "interdit_d_entrer"

private:
    // le genestealer arrive sur la planète et cherche un endroit où faire son nid
    void GenererInstallationGenestealer();
    // le genestealer a créé son nid et commence à inséminer des humains
    void GenererDebutReproductionGenestealer();

    void GenererPhasePatriarche();
    void GenererPhasePolitique();
    void GenererPhaseRebellion();


    // fonctions secondaires "raccourcis créées car de actions sont faites très souvent dans cette histoire
    /**
     * @brief GenererEffetTentativeInsemination
     * @param proba
     * @param nombreInsemines
     * @param id
     * @param repereImperium si true alors la proba s'applque aussi à la carac de repérage du culte par l'imperium
     * @return
     */
    Effet* GenererEffetTentativeInsemination(float proba, QString nombreInsemines, QString id, bool repereImperium = false);
};

#endif // HISTOIRETYRANIDES_H
