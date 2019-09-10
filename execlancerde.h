#ifndef EXECLANCERDE_H
#define EXECLANCERDE_H

#include "execnoeud.h"
#include "execeffet.h"
#include "lancerde.h"

namespace Ui {
class LancerDe;
}

class ExecLancerDe : public ExecNoeud
{
    Q_OBJECT

public:
    explicit ExecLancerDe(ExecEffet* exec_effet, LancerDe* effet, QWidget *parent = nullptr);
    virtual ~ExecLancerDe();

    virtual void RafraichirAffichageLayouts(int largeur = -1, int hauteur = -1);
    bool GestionTransition();

    //virtual void FinExecutionNoeud();
    void NettoyageAffichage();

    void AfficherNoeud();

    LancerDe* m_LancerDe = nullptr;
    ExecEffet* m_ExecEffet = nullptr;
    ExecNoeud* GetExecNoeud();

private:
    Ui::LancerDe *ui;

public slots:
    void ExecuterNoeudSlot();
};

#endif // EXECLANCERDE_H
