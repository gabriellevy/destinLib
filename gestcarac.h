#ifndef GESTIONNAIRECARAC_H
#define GESTIONNAIRECARAC_H

#include "abs/carac.h"
#include <QString>
#include <QMap>
#include <QHash>
#include <memory>

class SetCarac;

/**
 * @brief Système de racourci utilisé pour setter une grande quantité de caracs d'un seul coup (par exemple pour changer de eprso, de lieu...)
 */
class MultiSetterDeCarac
{
protected:
    QVector<std::shared_ptr<SetCarac>> m_SetCaracs = {};

public:
    MultiSetterDeCarac();

    void Appliquer();

};

/**
 * @brief DPerso signifie "data perso". C'est un contenant des caracs principales d'un personnage
 * C'est essentiellement utile pour gérer le changement de personnage en cours d'aventure et rendre plus intuitif l'initialisation
 * d'un personnage mais techniquement ce n'est qu'un intermédiaire avec le Gestionnaire de Carac
 * L 'avantage est qu'on peut le surclasser pour contrôler les getter et setter de caracs en overloadant les foncitons correspondantes
 */
class DPerso : public MultiSetterDeCarac
{
protected :
    virtual void RafraichirAffichage();

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

    virtual QString GetValeurCarac(QString id);
    virtual int GetValeurCaracAsInt(QString id);


    virtual void ExecutionPostChangeCarac(); //  fonction à surclasser pour effectuer des actions après une maj de carac
    virtual void ExecutionPreChangeCarac(); //  fonction à surclasser pour effectuer des actions avant une maj de carac
    void AppliquerSetCarac(const SetCarac& setCarac);
    int AJouterValeurACaracId(const QString& idCarac, const int& valeurAjoutee);
    int RetirerValeurACaracId(const QString& idCarac, const int& valeurRetiree);
    QString SetValeurACaracId(const QString& idCarac,const  QString& valeurSet);
    QString SetValeurACaracId(const QString& idCarac,const int& valeurSet);
    QString EffacerValeurACaracId(const QString& idCarac);
    bool IsCaracTrue(QString id);

    static std::shared_ptr<DPerso> GetDPersoJoue();

    friend class IPerso;
};

class GestCarac
{
private:
    GestCarac();
    static GestCarac* ME;

    // caracs actuelles du joueur
    QHash<QString, Carac*> m_Caracs;
    static QString GetCaracValue(QString id);
    static int GetCaracValueAsInt(QString id);
    static void AppliquerSetCarac(const SetCarac& setCarac);
    static int AJouterValeurACaracId(const QString& idCarac, const int& valeurAjoutee);
    static int RetirerValeurACaracId(const QString& idCarac, const int& valeurRetiree);
    static QString SetValeurACaracId(const QString& idCarac,const  QString& valeurSet);
    static QString SetValeurACaracId(const QString& idCarac,const int& valeurSet);
    static QString EffacerValeurACaracId(const QString& idCarac);
    static bool IsCaracTrue(QString id); // retourne true si cette carac est possédée par le personnage cad différente de null ou 0
    bool CetteCaracExisteDeja(QString id);
    void AppliquerCarac(SetCarac setCarac);

public:

    Carac* GetCarac(QString idCarac);

    // fonctions raccourcis de convénience :
    static GestCarac*  GetGestionnaireCarac();
    QVector<QString> m_CaracsAffichees; // id des Caracs affichées dans le profil joueur

    QHash<QString, Carac*> GetCaracs();

    static QString CARAC_NOM;
    static QString CARAC_PERSO_ID;
    static QString CARAC_DESCRIPTION;
    static QString CARAC_CHEMIN_PORTRAIT;

    // fonctions d'ajouts de caracs à l'aventure (essentiellement utile pour les paramétrer et déterminer sous quelle forme elles sont affichées)
    void AjouterCarac(Carac* carac);
    /**
     * @brief ajoute une carac affichable de valeur nulle avec son intitulé égal à son id
     */
    Carac* AjouterCaracNombre(QString idCarac, int valeur = 0, int valMin = -999999, int valMax = 999999);
    Carac* AjouterCaracNombreSupZero(QString idCarac, int valeur = 0, int valMin = -999999, int valMax = 999999);
    Carac* AjouterCaracString(QString idCarac, QString valeur = "");
    Carac* AjouterCaracStringIntitule(QString idCarac, QString valeur = "");
    Carac* AjouterCaracBinaire(QString idCarac, bool valeur = false);
    Carac* AjouterCaracImageValeur(QString idCarac, QString valeur = "");
    Carac* AjouterCaracImagePrimaire(QString idCarac, QString valeur = "");
    Carac* AjouterCaracImage(QString idCarac, QString valeur = "");

    friend class DPerso;
    friend class MultiSetterDeCarac;
};

#endif // GESTIONNAIRECARAC_H
