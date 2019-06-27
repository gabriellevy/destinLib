#ifndef PERSO_H
#define PERSO_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include "carac.h"
#include "aspectratiolabel.h"
#include <QPixmap>
#include <QVector>
#include "theme.h"

namespace Ui {
class Perso;
}

class SetCarac;

/**
 * @brief DPerso signifie "data perso". C'est un contenant des données principales d'un personnage indépendandemment du système d'interface qui est l'essentiel de ce fichier
 */
class DPerso
{
protected :
    virtual void RafraichirAffichage();

public:
    DPerso(QString id = "", QString nom = "", QString description = "", QString CheminImagePortrait = "");
    // c'estd ans cette focntion qu'il faut initialiser les caracs du perso, pas dans le constructeur
    virtual void InitialiserPerso();

    QString m_Id;
    QString m_Nom;
    QString m_Description = "";
    QString m_CheminImagePortrait = "";
    QPixmap m_ImagePortrait;
    QVector<QString> m_CaracsAAfficher = {};

    QVector<Theme*> m_Themes;

    friend class IPerso;
};

class ExecHistoire;

/**
 * @brief Class permettant d'afficher le persos mais contenant aussi toutes les données et caractéristiques de tous les personnages affichables
 */
class IPerso : public QWidget
{
    Q_OBJECT
    AspectRatioLabel* myImageLabel = nullptr;
    static IPerso* s_PersosInterface; // singleton
    static QString s_IdPersoActif;

protected:
    QHash<QString, DPerso*> m_Persos;

public:
    explicit IPerso(QWidget *parent = nullptr);
    ~IPerso();

    //void Rafraichir(QJsonArray persos);
    void ChangerPersoCourant(QString changePerso);
    void InitialiserPerso();
    void RafraichirAffichage();

    // accesseur vers le perso courant (qui est toujours unique)
    DPerso* GetPersoCourant();
    const DPerso* GetPerso(QString id);
    static IPerso* GetPersoInterface();
    static void AjouterPersoJouable(DPerso* perso);

private:
    Ui::Perso *ui;
};

#endif // PERSO_H
