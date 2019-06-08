#ifndef EXECEVT_H
#define EXECEVT_H

#include "evt.h"
#include "execnoeud.h"

class ExecEffet;

namespace Ui {
class Evt;
}

class ExecEvt : public ExecNoeud
{
    Q_OBJECT

    ExecEffet* m_ExecEffetActuel = nullptr;

public:
    ExecEvt(Evt* evt, QWidget *parent = nullptr);
    virtual ~ExecEvt();

    void Clean();

    ExecEffet* GetExecEffetActuel();

    virtual bool GestionTransition();
    virtual void LancerNoeud();

    ExecEffet* SetEffetIndex(int index);
    ExecEffet* SetExecEffet(Effet* effet);
    ExecEffet* SetExecEffet(ExecEffet* exec_effet);
    void RafraichirAffichageEffet(ExecEffet* effet);
    void AfficherNoeud();

    void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    Evt* GetEvt();

private:
    Ui::Evt *ui;
};

#endif // EXECEVT_H
