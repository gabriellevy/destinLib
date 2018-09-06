#ifndef GLISSEUR_H
#define GLISSEUR_H

#include <QJsonObject>

class Glisseur
{

public:
    int m_Minimum;
    int m_Maximum;
    int m_ValeurDepart;
    QString m_IdCaracAssociee;

    Glisseur(QJsonObject json);
    Glisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id);
};

#endif // GLISSEUR_H
