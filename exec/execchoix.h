#ifndef EXECCHOIX_H
#define EXECCHOIX_H

#include "execnoeud.h"
#include "../abs/choix.h"
#include "execlancerde.h"

class ExecEffet;
class LancerDe;

class ExecChoix : public ExecNoeud
{
    Q_OBJECT
 public:

    std::shared_ptr<Choix> m_Choix;

    explicit ExecChoix(std::shared_ptr<Choix> choix, QWidget *parent = nullptr);
    virtual ~ExecChoix();

    void AfficherNoeud();
    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    //void LancerNoeud();
    bool GestionTransition();

    ExecNoeud* GetExecNoeud();

private:
    Ui::Choix *ui;

public slots:
    void ExecuterNoeudSlot();
};

#endif // EXECCHOIX_H
