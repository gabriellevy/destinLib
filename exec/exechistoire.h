#ifndef EXECHISTOIRE_H
#define EXECHISTOIRE_H

#include "../abs/perso.h"
#include <memory>

class ExecNoeud;
class ExecEffet;
class ExecEvt;
class ExecLancerDe;
class Effet;
class Hist;
class Noeud;
class Evt;

namespace Ui {
class Histoire;
}

struct Historique {
    QList<QString> m_Textes;
    QString GetHistoriqueTotalAsStr() {
        QString final = "";
        for ( int i = m_Textes.length()-1 ; i >-1 ; --i) {
            final += m_Textes[i] + "\n ---- \n";
        }
        return final;
    }
};

/**
 * @brief Classe qui gère le déroulement et l'affichage de toute l'histoire du jeu
 */
class ExecHistoire : public QWidget
{
    Q_OBJECT

protected:

    std::shared_ptr<Evt> m_DernierEvtAffiche = nullptr;
    std::shared_ptr<Effet> m_DernierEffetAffiche = nullptr;

    int DeterminerIndexEvt(QString idEvt);
    int CalculerIndex(std::shared_ptr<Evt> evtATrouver);

    // repérage d'événement courant et effet courant en mode histoire normal
    //QString m_CurrentEvtId;
    //int m_EffetIndex; // index de l'effet actuel dans la liste des effets de l'événement actuel
    ExecNoeud* m_ExecNoeudActuel = nullptr; // 'curseur' pointant sur le noeud précisément exécuté à ce moment, que ce soit un événement, effet, choix
    ExecEvt* m_ExecEvtActuel = nullptr; // evt actuel (il doit toujours y en avoir un à partir de quand le destin est lancé


    //QString m_CurrentConditionnelEvtId;
    //int m_EffetConditionnelIndex;

public:
    explicit ExecHistoire(std::shared_ptr<Hist> histoire, QWidget *parent = nullptr);
    ~ExecHistoire();

    std::shared_ptr<Hist> m_Histoire;
    Historique m_Historique;

    virtual QString GetTitre();

    bool AppelerFonctionCallback(QString fonction, QVector<QString> caracs, QVector<QString> params);

    void PasserAEffetIndexSuivant();
    void PasserAEvtIndexSuivant();
    void GoToEffetId(QString idEffet);
    int DeterminerIndexEffet(QString idEffet);

    /**
     * @brief EvtActuel
     * @param forceHistoireMode : renvoie l'événement courant de l'histoire, excluant l'événement aléatoire secondaire même si on est en mode aléatoire
     * @return événement actuellement exécuté par le joueur
     */
    ExecEvt* GetExecEvtActuel(bool forceHistoireMode = false);
    static ExecEffet* GetExecEffetActuel(bool forceHistoireMode = false);
    ExecLancerDe* GetExecLancerDeActuel();
    std::shared_ptr<Evt> EvtActuel(bool forceHistoireMode = false);
    std::shared_ptr<Effet> EffetActuel(bool forceHistoireMode = false);
    ExecEvt* SetExecEvtActuel(std::shared_ptr<Evt> evt);

    ExecNoeud* DeterminerPuisLancerNoeudSuivant(ExecNoeud* noeudActuel = nullptr, bool noeudActuelEstValide = true);
    std::shared_ptr<Noeud> GetEffetDindexSuivant(std::shared_ptr<Noeud> noeudActuel);
    std::shared_ptr<Noeud> TesterSiEffetEstLancableOuSonElse(std::shared_ptr<Noeud> noeudActuel);
    // si les événements sont issues de la bdd ils ont un id qui permet de les extraire :
    std::shared_ptr<Evt> GetEvtSelonBddId(int id);
    std::shared_ptr<Evt> GetEvtSelonId(QString idATrouver);
    bool AppliquerGoTo(std::shared_ptr<Noeud> noeud);

    void RafraichirAffichageEvtEtOuEffet(std::shared_ptr<Evt> evt, std::shared_ptr<Effet> effet);

    void SetEffetIndex(int index);
    /**
     * @brief trouve l'événement correspondant à l'id en paramètre et en fait le noeud actuel
     */
    void SetCurrentEvtId(QString);
    void AjouterDureeAEffetHistoireCourant(float duree);

    void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    static std::shared_ptr<Effet> GetEffetActuel();

private:
    Ui::Histoire *ui;
};

#endif // EXECHISTOIRE_H
