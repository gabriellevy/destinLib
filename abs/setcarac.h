#ifndef SETCARAC_H
#define SETCARAC_H
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

enum class ModifCaracType {
    SetCarac,
    AddToCarac,
    RetireDeCarac
};

class SetCarac
{
private:
public:
    // Attention : si ces valeur restent à cette valeur de base (nexistepas*µ£$) cela signifie qu'elles ne sont pas utilisées par le SetCarac
    // les caractères étranges sont juste là pour être sûr que jamais une carac ne soit settée à cette valeur "défaut" qui ici équivaut à une sort de "null"
    QString m_Valeur = "nexistepas*µ£$";
    QString m_CaracId = "nexistepas*µ£$";
    //QString m_Intitule = "nexistepas*µ£$";
    ModifCaracType m_ModifCaracType;
    QString m_ValeurRandom = "nexistepas*µ£$";
    QString m_ValeurMin = "nexistepas*µ£$";
    QString m_ValeurMax = "nexistepas*µ£$";
    QString m_IdValeurCaracCopie = "nexistepas*µ£$";// quand la valeur du set carac est égale à la valeur d'une autre carac au moment de la modification

    /**
     * @brief retourne la valeur de l'objet. La calcule si c'est une valeur semi aléatoire
     */
    QString GetValeur();

    /**
     * @brief SetCarac constructeur à partir d'un QJsonObject
     * @param obj : json obj dont peut être déduit la valeur de SetCarac selon les nombreux types de valeur disponibles. Si nullptr, la valeur est à 0
     * @param modifCaracType
     * @param valeur : valeur par défaut si il n'y en a pas dans obj
     */
    SetCarac(QJsonObject obj, ModifCaracType modifCaracType, QString valeur = nullptr);
    SetCarac(ModifCaracType modifCaracType, QString caracId, QString valeur);
    SetCarac(ModifCaracType modifCaracType, QString caracId, int valeur);

    static ModifCaracType GetModifCaracTypeFromQString(QString str);
    static QString GetQStringFromModifCaracType(ModifCaracType mct);
};

#endif // SETCARAC_H
