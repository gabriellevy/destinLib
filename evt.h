#ifndef EVT_H
#define EVT_H

#include "effet.h"

namespace Ui {
class Evt;
}


enum TypeEvt {
    TE_Base,
    TE_Conditionnel,
    TE_Aleatoire // quand l'histoire prend une pause le temps d'un ou plusieurs événements aléatoires
};

class Evt : public QWidget, public Noeud
{
    Q_OBJECT
    Effet* m_EffetActuel;

    QString m_CheminImgFond;

public:
    //explicit Evt(QJsonObject evtJson, QWidget *parent = nullptr);
    explicit Evt(QString id,
                      QString nom,
                      QWidget *parent = nullptr);
    ~Evt();

    TypeEvt m_TypeEvenement;
    void Clean();

    virtual bool GestionTransition();
    virtual void LancerNoeud();

    void RafraichirAffichageEffet(Effet* effet);
    void AfficherNoeud();

    // fonction intermédiaire d'ajout d'effet qui doit être appelée par toutes les fonctions d'ajout d'effets plus spécifiques ci après
    Effet* AjouterEffet(Effet* effet);
    Effet* AjouterEffetModificateurCarac(QString caracId, QString nouvelleValeur, QString text = "", QString id = "");
    Effet* AjouterEffetRetireurACarac(QString caracId, QString valeurRetire, QString text = "", QString id = "");
    Effet* AjouterEffetNarration(QString text, QString cheminImg = "", QString id = "");
    Effet* AjouterEffetChangementPerso(QString persoId, QString text, QString cheminImg = "", QString id = "");
    Effet* AjouterEffetTest(QString caracId, Comparateur comparateur, QString valeur, QString id = "");
    Effet* AjouterEffetVide();
    Effet* AjouterEffetGlisseur(QString text, QString valeur_min, QString valeur_max, QString valeur_depart, QString carac_id, QString cheminImg = "", QString id = "" );
    Effet* AjouterEffetAjouteurACarac(QString caracId, QString valeurAjoutee, QString id = "");

    /**
     * @brief AjouterImgFond
     * @param fond : chemin vers l'image à utiliser en fond. Exemple : ':/images/fond.jpg'
     */
    void AjouterImgFond(QString fond);

    QVector<Effet*> m_Effets;
    Effet* TrouverEffet(QString id);

private:
    Ui::Evt *ui;
};

#endif // EVT_H
