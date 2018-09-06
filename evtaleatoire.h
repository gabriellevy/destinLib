#ifndef EVTALEATOIRE_H
#define EVTALEATOIRE_H

#include "evt.h"

/**
 * @brief The EvtAleatoire class
 * Les événemùents aléatoires sont des événements qui quand ils sont lancés lancent un de leurs effets aléatoirement (en folnction du poids random de chaque effet)
 * En conséquence les effets de ces événements ne se suivent pas, seulement un est exécuté.
 * De plus les attributs suivant sont indispensables pour les effets contenus :
 *  - au moins une condition (contenant au moins une proba)
 *  - soit un m_GoToEvtId soit un m_GoToEffetId car l'effet suivant ne peut pas être celui qui suit
 */
class EvtAleatoire : public Evt
{
    Q_OBJECT
public:
    explicit EvtAleatoire(QString id,
                      QString nom,
                      QWidget *parent = nullptr);

    Effet* DeterminerEffetAleatoire();
};

#endif // EVTALEATOIRE_H
