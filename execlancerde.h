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
    virtual bool GestionTransition();

    //virtual void FinExecutionNoeud();
    void NettoyageAffichage();

    void AfficherNoeud();

    LancerDe* m_LancerDe = nullptr;
    ExecEffet* m_ExecEffet = nullptr;
    ExecNoeud* GetExecNoeud();
    int GetTotalRes();

    // résultat final de l'exécution de la fonction callback du lancer de dé
    ResExecutionLancerDe* m_ResExecution = nullptr;

private:
    Ui::LancerDe *ui;

    // résultat final de chaque dé lancé
    QVector<int> m_Res;

public slots:
    void ExecuterNoeudSlot();
};

#endif // EXECLANCERDE_H
