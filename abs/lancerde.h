#ifndef LANCERDE_H
#define LANCERDE_H

#include "noeudnarratif.h"

class Effet;
class ExecEffet;

/**
 * @brief ce qui est renvoyé par la fonction callback d'effet du lancer de dé
 */
struct ResExecutionLancerDe {
    QString m_TexteRes = "rien à afficher";
    // tant que ceci est true la lancer de dé se répète encore et encore sans passer à l'effet suivant
    bool m_RestAffiche = false;

    ResExecutionLancerDe(QString resultatTexte, bool resteAffiche)
        :m_TexteRes(resultatTexte), m_RestAffiche(resteAffiche) {}
};

class LancerDe : public NoeudNarratif
{
protected:

public:
    // à passer en protected quand la structure GenEvt sera finie :
    LancerDe(std::shared_ptr<Effet> ParentEffet, QString texteBouton, int nbDes,
          std::function<std::shared_ptr<ResExecutionLancerDe>(int resDe)> callback);

    virtual ~LancerDe() {}

    virtual bool AQuelqueChoseAAfficher();
    std::shared_ptr<Effet> m_ParentEffet;
    int m_NbDes;
    std::function<std::shared_ptr<ResExecutionLancerDe>(int resDe/*, QVector<QString> params*/)> m_Callback; // fonction exécutée en fonction du résultat du lancer

    //friend class GenEvt;
};

#endif // LANCERDE_H
