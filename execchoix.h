#ifndef EXECCHOIX_H
#define EXECCHOIX_H

#include "execnoeud.h"
#include "choix.h"
#include "execlancerde.h"

class ExecEffet;
class LancerDe;

class ExecChoix : /*public QPushButton, */public ExecNoeud
{
    Q_OBJECT
 public:

    Choix* m_Choix;
    ExecEffet* m_ExecEffet = nullptr;
    ExecLancerDe* m_ExecLancerDe = nullptr;

    explicit ExecChoix(Choix* choix, QWidget *parent = nullptr);
    explicit ExecChoix(ExecEffet* ExecEffet, Choix* choix, QWidget *parent = nullptr);
    explicit ExecChoix(ExecLancerDe* ExecEffet, Choix* choix, QWidget *parent = nullptr);
    explicit ExecChoix(ExecNoeud* execNoeud, Choix* choix, QWidget *parent = nullptr);
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
