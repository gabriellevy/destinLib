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

namespace Ui {
class Perso;
}

class SetCarac;
class ExecHistoire;
class GestionnaireCarac;
class DPerso;

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

    GestionnaireCarac* m_GestionnaireCarac = nullptr;
    QHash<QString, DPerso*> m_Persos;

public:
    explicit IPerso(QWidget *parent = nullptr);
    ~IPerso();

    //void Rafraichir(QJsonArray persos);
    void ChangerPersoCourant(QString changePerso);
    void InitialiserPerso();
    void RafraichirAffichage();

    // accesseur vers le perso courant (qui est toujours unique)
    static DPerso* GetPersoCourant();
    DPerso* GetPerso(QString id) const;
    static IPerso* GetPersoInterface();
    static void AjouterPersoJouable(DPerso* perso);

private:
    Ui::Perso *ui;
};

#endif // PERSO_H
