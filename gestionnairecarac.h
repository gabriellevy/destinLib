#ifndef GESTIONNAIRECARAC_H
#define GESTIONNAIRECARAC_H

#include "carac.h"
#include <QString>
#include <QMap>

class SetCarac;

/**
 * @brief Système de racourci utilisé pour setter une grande quantité de caracs d'un seul coup (par exemple pour changer de eprso, de lieu...)
 */
class MultiSetterDeCarac
{
protected:
    QVector<SetCarac*> m_SetCaracs = {};

public:
    MultiSetterDeCarac();

    void Appliquer();

};

/**
 * @brief DPerso signifie "data perso". C'est un contenant des caracs principales d'un personnage
 * C'est essentiellement utile pour gérer le changement de personnage en cours d'aventure et rendre plus intuitif l'initialisation
 * d'un personnage mais techniquement ce n'est qu'un intermédiaire avec le Gestionnaire de Carac
 */
class DPerso : public MultiSetterDeCarac
{
protected :
    virtual void RafraichirAffichage();
   /* QString m_Id;
    QString m_Nom;
    QString m_Description = "";
    QString m_CheminImagePortrait = "";*/

public:
    DPerso(QString id, QString nom, QString description, QString CheminImagePortrait);
    virtual ~DPerso() {}
    // c'estd ans cette focntion qu'il faut initialiser les caracs du perso, pas dans le constructeur
    // note : penser à utiliser m_CaracsAAfficher.push_back("id de la carac") quand on veut qu'une carac donnée soit affichée
    virtual void InitialiserPerso();
    QPixmap m_ImagePortrait;
    //QVector<QString> m_CaracsAAfficher = {};

    void MajNom(QString nouveauNom);
    void MajCheminImage(QString nouveauNom);
    QString GetId();

    friend class IPerso;
};

class GestionnaireCarac
{
    GestionnaireCarac();
    static GestionnaireCarac* ME;

    // caracs actuelles du joueur
    QMap<QString, Carac*> m_Caracs;

public:

    Carac* GetCarac(QString idCarac);

    // focntions raccourcis de convénience :
    static GestionnaireCarac* GetGestionnaireCarac();
    static QString GetCaracValue(QString id);
    static int GetCaracValueAsInt(QString id);
    static void AppliquerSetCarac(const SetCarac& setCarac);
    static int AJouterValeurACaracId(const QString& idCarac, const int& valeurAjoutee);
    static int RetirerValeurACaracId(const QString& idCarac, const int& valeurRetiree);
    static QString SetValeurACaracId(const QString& idCarac,const  QString& valeurSet);
    static QString SetValeurACaracId(const QString& idCarac,const int& valeurSet);
    bool CetteCaracExisteDeja(QString id);
    void AppliquerCarac(SetCarac setCarac);
    void AjouterCarac(Carac* carac);
    /**
     * @brief ajoute une carac affichable de valeur nulle avec son intitulé égal à son id
     */
    void AjouterCaracNombre(QString idCarac, int valeur = 0, int valMin = -999999, int valMax = 999999);

    QMap<QString, Carac*> GetCaracs();

    static QString CARAC_NOM;
    static QString CARAC_PERSO_ID;
    static QString CARAC_DESCRIPTION;
    static QString CARAC_CHEMIN_PORTRAIT;
};

#endif // GESTIONNAIRECARAC_H
