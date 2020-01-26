#ifndef EXECCHOIX_H
#define EXECCHOIX_H

#include "execnoeud.h"
#include "../abs/choix.h"
#include "execlancerde.h"
#include <memory>

class ExecEffet;
class LancerDe;

class ExecChoix : /*public QPushButton, */public ExecNoeud
{
    Q_OBJECT
 public:

    std::shared_ptr<Choix> m_Choix;
    ExecEffet* m_ExecEffet = nullptr;
    ExecLancerDe* m_ExecLancerDe = nullptr;

    explicit ExecChoix(
            std::shared_ptr<Choix> choix, QWidget *parent = nullptr);
    explicit ExecChoix(
            ExecEffet* ExecEffet, std::shared_ptr<Choix> choix, QWidget *parent = nullptr);
    explicit ExecChoix(
            ExecLancerDe* ExecEffet, std::shared_ptr<Choix> choix, QWidget *parent = nullptr);
    explicit ExecChoix(
            ExecNoeud* execNoeud, std::shared_ptr<Choix> choix, QWidget *parent = nullptr);
    virtual ~ExecChoix();

    void AfficherNoeud();
    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    //void LancerNoeud();
    bool GestionTransition();

    ExecNoeud* GetExecNoeud();

private:
    Ui::Choix *ui;

public slots:
    void ExecuterNoeudSlot(/*bool afficherNoeud = true, bool lancerNoeudSuivantSiRienAAfiicher = true*/);
};

#endif // EXECCHOIX_H
