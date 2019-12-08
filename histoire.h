#ifndef HISTOIRE_H
#define HISTOIRE_H

#include "evt.h"
#include "evtaleatoire.h"
#include <QVector>
#include "perso.h"


enum ModeDeroulement : int {
    Aucun,
    Normal, // un bouton à cliquer pour apsser à l'effet suivant
    // les effets défilent selon la valeur m_MsDuree qui leur est affectée (il faudrait leur en affecter une de base ?)
    // les boutons sont cachés
    Automatique
};

enum PhaseDeroulement : int {
    epd_Aucun,
    epd_Construction,
    epd_Deroulement,
    epd_Fini // plus de défilement ni de boutons
};

/**
 * @brief Cette classe est une pure structure de données. Elle fait le lien entre GenHistoire qui la créée et ExecHistoire qui l'exécute
 * elle est donc tout aussi indépendante de la manière dont elle est générée ou exécutée
 */
class Hist
{
public:
    Hist(QString titre);

    QString m_Titre = "Titre inconnu";

    QVector<Evt*> m_Evts;// événements de base (aventure elle-même)
    QVector<Evt*> m_EvtsConditionnels; // événements déclenchés automatiquement dès qu'on remplit leurs conditions
    QVector<Evt*> m_EvtsAleatoires; // événements qui peuvent être appelés par des effets particuliers nécessitant des événements aléatoires durant une certaine période

    ModeDeroulement m_ModeDeroulement = Normal;
    PhaseDeroulement m_PhaseDeroulement = epd_Aucun;
    int m_MsDureeDefilement = -1; // utilisé seulement en mode de déroulement automatique

    /**
     * @brief fonctions spéciales associées à cette histoire et appellables par les noeuds au runtime
     * Elle ont un identifiant unique (égal à leur nom) qui permet de les référence en base de données par exemple
     * le premier paramètre QVector<QString> correspond à l'id des carac qu'on doit passer à la fonction en paramètre
     * le 2ème correspond à des valeurs "brutes"
     *
     * exemple d'utilisation "élaborée" :
     * this->m_HistoireGeneree->m_CallbackFunctions["NomArbitraireDeFonction"] = [](QVector<QString>, QVector<QString>) {
        Effet* effetActuel = UMo::ME->GetExecHistoire()->GetExecEffetActuel()->GetEffet();
        effetActuel->m_Text = "Nouveau texte hahahah";
        effetActuel->m_GoToEffetId = "UneAutreFonction";
        return true;
    };
     */
    QMap<QString, std::function<bool(QVector<QString>, QVector<QString>)>> m_CallbackFunctions;

    DPerso* GetPersoCourant();
};

#endif // HISTOIRE_H
