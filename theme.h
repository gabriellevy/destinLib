#ifndef THEME_H
#define THEME_H
#include <QString>
#include <QMap>
#include "carac.h"

/**
 * @brief Un thème est un groupement de caracs et de fonctions associés
 *
 * ils correspondent à une partie de l'univers d'une histoire
 * ils sont associables à différents éléments, en particulier les histoires et les événements mais pas seulement
 */
class Theme
{
    QString m_Intitule;
    QString m_Description;

public:
    Theme(QString intitule, QString description="");

    // caracs associées à ce thème et leur valeur par défaut à l'instanciation du thème
    QVector<Carac*> m_CaracsDeBase;
    // callbacks de fonctions liées au thème appelables au runime
    QMap<QString, std::function<void()>> m_Callbacks;
};

#endif // THEME_H
