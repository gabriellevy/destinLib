#ifndef EXECCHOIX_H
#define EXECCHOIX_H

#include "execnoeud.h"
#include "choix.h"

class ExecEffet;

class ExecChoix : /*public QPushButton, */public ExecNoeud
{
    Q_OBJECT
 public:

    Choix* m_Choix;
    ExecEffet* m_ExecEffet = nullptr;

    explicit ExecChoix(ExecEffet* ExecEffet, Choix* choix, QWidget *parent = nullptr);
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
