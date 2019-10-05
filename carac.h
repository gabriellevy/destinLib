#ifndef CARAC_H
#define CARAC_H

#include <QWidget>
#include "glisseur.h"

namespace Ui {
class Carac;
}

enum MODE_AFFICHAGE {
    Ma_Cache, // n'est pas affichée => automatique si elle n'est pas définie dans le json dans la partie 'héros'
    //ta_Deduit, // pas précisé dans le json donc déduit des autres caractéristiques (cf code)
    ma_Binaire, // seul l'intitulé est affiché si la carac n'est pas égale à 0 ou ""
    ma_Img, // image seulement (et infobulle si il y a une description)
    ma_ImgValeur, // image seulement (et infobulle si il y a une description) mais l'adresse de l'mage est dans la valeur
    ma_Texte, // mode par défaut
    ma_Nombre,
    ma_Jauge // sous forme de jauge => possible seulement si on a défini une valeur maximum de cette valeur
};

/**
 * @brief DCarac signifie "data carac". C'est un contenant des données principales d'une carac indépendandemment du système d'interface qui est l'essentiel de ce fichier
 */
class DCarac
{
public:
    DCarac(QString Id = "", QString Intitule = "", QString Valeur = "", QString Description = "", QString valeurMin = "", QString valeurMax = "");

    QString m_Id;
    QString m_Intitule;
    QString m_Description;
    QString m_ValeurMin = "";
    QString m_ValeurMax = "";
    QString GetDescription() {return m_Description;}

    double SetValeur(double nouvValeur);
    QString SetValeur(QString nouvValeur);
    QString GetValeur(){return m_Valeur;}
    int GetValeurInt(){return m_Valeur.toInt();}
    double GetValeurDouble(){return m_Valeur.toDouble();}
    bool AUneValeur() {return m_Valeur != "";}

private:
    QString m_Valeur;
};

class Carac : public QWidget
{
    Q_OBJECT

protected:

public:
    explicit Carac(QWidget *parent = nullptr);
    Carac(QString Id, QString Intitule, QString Valeur, QString Img, QString Description, MODE_AFFICHAGE ModeAffichage,
          QWidget *parent = nullptr, QString valeurMin = "", QString valeurMax = "");
    ~Carac();

    MODE_AFFICHAGE m_ModeAffichage; // de quelle manière est affichée cette carac dans l'interface personnage

    void DeterminerModeAffichage(QString modeAffichage);
    void Afficher();
    bool bAffichable();

    virtual QString GetCaracDescription();

    DCarac m_DataCarac;

    QPixmap m_Img;
    void SetImg(QString CheminImg);

private:
    Ui::Carac *ui;
    bool AfficherValeur();
    bool AfficherImage();
    bool AfficherIntitule();
};

class Jauge : public Carac
{
public:
    Jauge(QString Id, QString Intitule, double m_Minimum, double m_Maximum, double m_ValeurDepart, QString Img = "", QString Description = "", QWidget *parent = nullptr);

    Glisseur m_ValeursJauge;


    void SetValeursJauge(double Minimum, double Maximum);
};

#endif // CARAC_H
