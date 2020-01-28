#ifndef EXECNOEUD_H
#define EXECNOEUD_H

#include <QWidget>
#include <memory>

class NoeudNarratif;
class Noeud;
class ExecChoix;

/**
 * cette manière tordue d'ajouter enable_shared_from_this à toutes classes était nécessaire pour éviter
 * que la fonction shared-from_this soit ambigue (vu qu'héritée et redéfinie pour chaque classe héritant de ExecNoeud)
 */
class virtual_enable_shared_from_this:
   public std::enable_shared_from_this<virtual_enable_shared_from_this>
{
public:
   virtual ~virtual_enable_shared_from_this() {}

    friend class ExecNoeud;
};

template<class T> class my_enable_shared_from_this :
    public virtual virtual_enable_shared_from_this
{
public:
   std::shared_ptr<T> shared_from_this()
   {
      return std::dynamic_pointer_cast<T>(
         virtual_enable_shared_from_this::shared_from_this());
   }
   virtual ~my_enable_shared_from_this() {}
};

class ExecNoeud : public QWidget,
        public my_enable_shared_from_this<ExecNoeud>
{
    Q_OBJECT

public:
    using my_enable_shared_from_this<ExecNoeud>::shared_from_this;

    ExecNoeud(std::shared_ptr<NoeudNarratif> noeud, QWidget* parent = nullptr);
    virtual ~ExecNoeud() {}

    QPixmap m_Img;
    QMovie* m_Film = nullptr;

    QList<std::shared_ptr<ExecChoix>> m_ExecChoix;

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

    std::shared_ptr<NoeudNarratif> m_Noeud = nullptr; //  pointeur vers le noeud temporairement représenté via la classe Exec (il appartient à une structure Histoie qui en a la charge)

protected:
    virtual void AjouterAuxBoutonsHoriz(std::shared_ptr<ExecNoeud> execNoeud);
    virtual void AjouterAuxBoutonsVertic(std::shared_ptr<ExecNoeud> execNoeud);

private:

};

#endif // EXECNOEUD_H
