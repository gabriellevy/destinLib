#ifndef EXECEVT_H
#define EXECEVT_H

#include "../abs/evt.h"
#include "execnoeud.h"

class ExecEffet;
class ExecLancerDe;

namespace Ui {
class Evt;
}

class ExecEvt : public ExecNoeud,
        my_enable_shared_from_this<ExecEvt>
{
    Q_OBJECT

    std::shared_ptr<ExecEffet> m_ExecEffetActuel = nullptr;

public:
    using my_enable_shared_from_this<ExecEvt>::shared_from_this;
    ExecEvt(std::shared_ptr<Evt> evt, QWidget *parent = nullptr);
    virtual ~ExecEvt();

    void Clean();

    std::shared_ptr<ExecEffet> GetExecEffetActuel();
    std::shared_ptr<ExecLancerDe> GetExecLancerDeActuel();

    virtual bool GestionTransition();
    virtual void LancerNoeud();

    std::shared_ptr<ExecEffet> SetEffetIndex(int index);
    std::shared_ptr<ExecEffet> SetExecEffet(QString effetId);
    std::shared_ptr<ExecEffet> SetExecEffet(std::shared_ptr<Effet> effet);
    std::shared_ptr<ExecEffet> SetExecEffet(std::shared_ptr<ExecEffet> exec_effet);
    void RafraichirAffichageEffet(std::shared_ptr<ExecEffet> effet);
    void AfficherNoeud();

    void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);

    std::shared_ptr<Evt> GetEvt();

private:
    Ui::Evt *ui;
};

#endif // EXECEVT_H
