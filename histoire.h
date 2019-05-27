#ifndef HISTOIRE_H
#define HISTOIRE_H

#include "evt.h"
#include "evtaleatoire.h"
#include "carac.h"
#include <QVector>
#include "theme.h"
#include "perso.h"

/**
 * @brief Cette classe est une pure structure de données. Elle fait le lien entre GenHistoire qui la créée et ExecHistoire qui l'exécute
 * elle est donc tout aussi indépendante de la manière dont elle est générée ou exécutée
 */
class Hist
{
public:
    Hist();

    void AppliquerTheme(Theme* theme);

    // caracs actuelles du joueur
    QVector<Carac*> m_Caracs;

    QVector<Theme*> m_Themes;

    QVector<Evt*> m_Evts;// événements de base (aventure elle-même)
    QVector<Evt*> m_EvtsConditionnels; // événements déclenchés automatiquement dès qu'on remplit leurs conditions
    QVector<Evt*> m_EvtsAleatoires; // événements qui peuvent être appelés par des effets particuliers nécessitant des événements aléatoires durant une certaine période


    /**
     * @brief fonctions spéciales associées à cette histoire et appellables par les noeuds au runtime
     * Elle ont un identifiant unique (égal à leur nom) qui permet de les référence en base de données par exemple
     * le premier paramètre QVector<QString> correspond à l'id des carac qu'on doit passer à la fonction en paramètre
     * le 2ème correspond à des valeurs "brutes"
     */
    QMap<QString, std::function<bool(QVector<QString>, QVector<QString>)>> m_CallbackFunctions;

    DPerso* GetPersoCourant();
};

#endif // HISTOIRE_H
