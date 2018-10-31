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
    ma_Img, // image seulement
    ma_Texte, // mode par défaut
    ma_Nombre,
    ma_Jauge // sous forme de jauge => possible seulement si on a défini une valeur maximum de cette valeur
};

class Carac : public QWidget
{
    Q_OBJECT

protected:
    QPixmap m_Img;

public:
    explicit Carac(QWidget *parent = 0);
    Carac(QString Id, QString Intitule, QString Valeur, QString Img, QString Description, MODE_AFFICHAGE ModeAffichage, QWidget *parent = 0);
    ~Carac();

    QString m_Id;
    QString m_Intitule;
    QString m_Valeur;
    QString m_Description;
    MODE_AFFICHAGE m_ModeAffichage; // de quelle manière est affichée cette carac dans l'interface personnage

    void DeterminerModeAffichage(QString modeAffichage);
    void Afficher();
    bool bAffichable();

private:
    Ui::Carac *ui;
    bool AfficherValeur();
    bool AfficherImage();
    bool AfficherIntitule();
};

class Jauge : public Carac
{
public:
    Jauge(QString Id, QString Intitule, double m_Minimum, double m_Maximum, double m_ValeurDepart, QString Img, QString Description, QWidget *parent = 0);

    Glisseur m_ValeursJauge;


    void SetValeursJauge(double Minimum, double Maximum);
};

#endif // CARAC_H
