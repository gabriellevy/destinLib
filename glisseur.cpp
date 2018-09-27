#include "glisseur.h"

Glisseur::Glisseur(QJsonObject json)
{
    Q_ASSERT_X(
                !(!json.contains("valeur_min") ||
                !json["valeur_min"].isString() ||
                !json.contains("valeur_max") ||
                !json["valeur_max"].isString() ||
                !json.contains("carac_id") ||
                !json["carac_id"].isString()),
            "Création de slider",
            "Il manque un param"
            );

    m_Minimum = json["valeur_min"].toString().toDouble();
    m_Maximum = json["valeur_max"].toString().toDouble();
    m_ValeurDepart = json["valeur_depart"].toString().toDouble();
    m_IdCaracAssociee = json["carac_id"].toString();
}

Glisseur::Glisseur(QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id)
{
    m_Minimum = valeur_min.toDouble();
    m_Maximum = valeur_max.toDouble();
    m_ValeurDepart = valeur_depart.toDouble();
    m_IdCaracAssociee = carac_id;
}
