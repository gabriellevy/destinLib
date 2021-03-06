#ifndef EXECNOEUD_H
#define EXECNOEUD_H

#include <QWidget>
#include <memory>

class NoeudNarratif;
class Noeud;
class ExecChoix;

class ExecNoeud : public QWidget
{
    Q_OBJECT

public:

    ExecNoeud(std::shared_ptr<NoeudNarratif> noeud, QWidget* parent = nullptr);
    virtual ~ExecNoeud() {}

    QPixmap m_Img;
    QMovie* m_Film = nullptr;

    QList<ExecChoix*> m_ExecChoix;

    /**
     * @brief exécute tout ce qui est défini dans ce noeud (changements de caracs, go to un autre effet etc...
     * @param afficherNoeud : si true la fonction affiche le texte et les images du noeud
     * @param lancerNoeudSuivantSiRienAAfiicher : si true la fonction exécute automatiquement le noeud suivant si il n'y a rien à afficher
     */
    virtual void ExecuterActionsNoeud(
            std::shared_ptr<Noeud> noeudAExecuter = nullptr/*bool afficherNoeud = true, bool lancerNoeudSuivantSiRienAAfiicher = true*/);

    /**
     * @brief S'exécute quand l'exécutyion de l'aventure arrive à ce noeud
     * Inclut l'affichage, puiséventuellement l'exécution des actions, puis la transition éventuelle vers l'action suivante
     */
    virtual void LancerNoeud();


    virtual void AfficherNoeud();

    /**
     * @brief Gère le passage au noeud suivant quand celui ci est exécuté
     * @return true si la transition est automatique sans besoin de clic de l'utilisateur, false sinon (pour éviter les rafraichissements multiples inutiles)
     */
    virtual bool GestionTransition();

    virtual void FinExecutionNoeud();
    virtual void GenerationExecChoix();

    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    std::weak_ptr<NoeudNarratif> m_Noeud; //  pointeur vers le noeud temporairement représenté via la classe Exec (il appartient à une structure Histoire qui en a la charge)

protected:
    virtual void AjouterAuxBoutonsHoriz(ExecNoeud* execNoeud);
    virtual void AjouterAuxBoutonsVertic(ExecNoeud* execNoeud);

private:

};

#endif // EXECNOEUD_H
