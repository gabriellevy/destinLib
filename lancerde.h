#ifndef LANCERDE_H
#define LANCERDE_H

#include "noeudnarratif.h"

class Effet;
class ExecEffet;

class LancerDe : public NoeudNarratif
{
protected:

public:
    virtual ~LancerDe() {}

    virtual bool AQuelqueChoseAAfficher();
    Effet* m_ParentEffet;
    int m_NbDes;
    QString m_Intitule; // intitulé bouton
    std::function<QString(int resDe, QVector<QString> params)> m_Callback; // fonction exécutée en fonction du résultat du lancer

    // à passer en protected quand la structure GenEvt sera finie :
    explicit LancerDe(Effet* ParentEffet, QString intitule, int nbDes);
    friend class GenEvt;
};

#endif // LANCERDE_H
