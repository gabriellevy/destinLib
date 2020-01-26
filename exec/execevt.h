#ifndef EXECEVT_H
#define EXECEVT_H

#include "../abs/evt.h"
#include "execnoeud.h"
#include <memory>

class ExecEffet;
class ExecLancerDe;

namespace Ui {
class Evt;
}

class ExecEvt : public ExecNoeud
{
    Q_OBJECT

    ExecEffet* m_ExecEffetActuel = nullptr;

public:
    ExecEvt(std::shared_ptr<Evt> evt, QWidget *parent = nullptr);
    virtual ~ExecEvt();

    void Clean();

    ExecEffet* GetExecEffetActuel();
    ExecLancerDe* GetExecLancerDeActuel();

    virtual bool GestionTransition();
    virtual void LancerNoeud();

    ExecEffet* SetEffetIndex(int index);
    ExecEffet* SetExecEffet(QString effetId);
    ExecEffet* SetExecEffet(std::shared_ptr<Effet> effet);
    ExecEffet* SetExecEffet(ExecEffet* exec_effet);
    void RafraichirAffichageEffet(ExecEffet* effet);
    void AfficherNoeud();

    void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    std::shared_ptr<Evt> GetEvt();

private:
    Ui::Evt *ui;
};

#endif // EXECEVT_H
