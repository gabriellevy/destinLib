#ifndef GESTIONNAIRECARAC_H
#define GESTIONNAIRECARAC_H

#include "carac.h"
#include <QString>

class GestionnaireCarac
{
    GestionnaireCarac();
    static GestionnaireCarac* ME;

public:

    Carac* GetCarac(QString idCarac);

    // caracs actuelles du joueur
    QVector<Carac*> m_Caracs;

    // focntions raccourcis de convénience :
    static GestionnaireCarac* GetGestionnaireCarac();
    static QString GetCaracValue(QString id);
    static int GetCaracValueAsInt(QString id);
    static int AJouterValeurACaracId(QString idCarac, int valeurAjoutee);
    static int RetirerValeurACaracId(QString idCarac, int valeurRetiree);
    static QString SetValeurACaracId(QString idCarac, QString valeurSet);
};

#endif // GESTIONNAIRECARAC_H
