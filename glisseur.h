#ifndef GLISSEUR_H
#define GLISSEUR_H

#include <QJsonObject>

class Glisseur
{

public:
    double m_Minimum;
    double m_Maximum;
    double m_ValeurDepart;
    QString m_IdCaracAssociee;

    //Glisseur(QJsonObject json);
    Glisseur(QString valeur_min = "0", QString valeur_max = "1", QString valeur_depart = "0", QString carac_id = "");
};

#endif // GLISSEUR_H
