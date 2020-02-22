#ifndef EXECCHOIX_H
#define EXECCHOIX_H

#include "execnoeud.h"
#include "../abs/choix.h"
#include "execlancerde.h"

class ExecEffet;
class LancerDe;

class ExecChoix : public ExecNoeud,
        my_enable_shared_from_this<ExecChoix>
{
    Q_OBJECT
 public:
    using my_enable_shared_from_this<ExecChoix>::shared_from_this;

    std::shared_ptr<Choix> m_Choix;

    explicit ExecChoix(std::shared_ptr<Choix> choix, QWidget *parent = nullptr);
    explicit ExecChoix(
            std::shared_ptr<ExecNoeud> execNoeud, std::shared_ptr<Choix> choix, QWidget *parent = nullptr);
    virtual ~ExecChoix();

    void AfficherNoeud();
    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    //void LancerNoeud();
    bool GestionTransition();

    std::shared_ptr<ExecNoeud> GetExecNoeud();

private:
    Ui::Choix *ui;

public slots:
    void ExecuterNoeudSlot();
};

#endif // EXECCHOIX_H
