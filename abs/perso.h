#ifndef PERSO_H
#define PERSO_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include "carac.h"
#include "../aspectratiolabel.h"
#include <QPixmap>
#include <QVector>
#include <memory>

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

    std::shared_ptr<GestionnaireCarac> m_GestionnaireCarac = nullptr;
    QHash<QString, std::shared_ptr<DPerso>> m_Persos;

public:
    explicit IPerso(QWidget *parent = nullptr);
    ~IPerso();

    //void Rafraichir(QJsonArray persos);
    void ChangerPersoCourant(QString changePerso);
    virtual void InitialiserPerso();
    void RafraichirAffichage();

    // accesseur vers le perso courant (qui est toujours unique)
    static std::shared_ptr<DPerso> GetPersoCourant();
    std::shared_ptr<DPerso> GetPerso(QString id) const;
    static IPerso* GetPersoInterface();
    static void AjouterPersoJouable(std::shared_ptr<DPerso> perso);

private:
    Ui::Perso *ui;
};

#endif // PERSO_H
